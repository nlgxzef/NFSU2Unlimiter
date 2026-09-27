#pragma once

#include "stdafx.h"
#include "GlobalVariables.h"

// A car sent over the network, read by a game whose car or part count is not the sender's.
//
// Online and LAN send a car as one bit stream. The lobby puts it in the player's "C" key as
// "%02X%03X" (format 28h, base64 length) followed by the base64 (sub_587630 writes it, sub_5854C0
// reads it), and a race start sends the same stream in a packet (sub_5FE570, sub_5F97D0). Both
// ends are sub_5F0320 and sub_5F04D0, and the stream is, in order:
//
//   car type              QuantCarType bits    (0x89D2D4)
//   32 bit value          physics checksum
//   10 flags              1 bit each
//   9 bytes               performance
//   16 bit length, bytes  the 41 byte record at RideInfo+398h, read by sub_584020
//   slot count            QuantSlots bits      (0x89D190), 170 when parts follow
//   one per slot          QuantPartIndex bits  (0x89D418)
//
// Nothing in it says how wide the first and the last fields are. Each end uses its own
// quantizers, and Unlimiter sizes those to the local car and part counts (Game.h): 66 cars and
// 18176 parts make 7 and 15 bits, where an unmodded game uses 6 and 14. A car from a game with
// other counts is then read with every field after the car type shifted.
//
// That is not a wrong car, it is a crash, because of sub_584020:
//
//   00584033  call sub_581D90        ; the length, 16 bits, straight off the stream
//   00584046  loop: read 8 bits, mov [esi+ebp], dl
//
// It copies as many bytes as the stream claims. The size its caller passes (29h, the record's
// buffer on sub_5F04D0's stack) is never looked at. Read one bit late, the record length 0029h
// comes back as 0052h, and 82 bytes go into 41: over sub_5F04D0's return address, which then
// returns into base64. Three crash dumps show exactly that, a joining unmodded player's car read
// as 7 bit car type and 15 bit parts. Read as 6 and 14 the same stream ends 4 bits before its
// last byte does, with a record length of 41.
//
// Two fixes, one for each half:
//
// sub_584020 now keeps to the size its caller gives. What does not fit is stepped over rather
// than written. Every caller passes its buffer's size (0x0C, 0x10, 0x20, 0x29, 0x224), so this
// holds for the lobby, the race start and the three other readers, whatever arrives.
//
// sub_5F04D0 is preceded by a look at the stream to find the widths it was written with. A
// layout is plausible when the car type is one this game has, the record length is exactly 41,
// the slot count is at most 170 and the parts fit in what arrived. Where the stream ends with the
// car, as in the lobby, the right layout also ends inside the last byte. The local widths are
// tried first, then the unmodded 6 and 14, then everything near them.
//
//   - the local widths fit: read exactly as before.
//   - other widths fit: the sender has another car and part list, so its part indices name
//     other parts here. The car is read with the sender's widths, its type kept, and its parts
//     replaced with that car's stock ones.
//   - nothing fits: the car becomes the replacement car with stock parts, rather than whatever
//     the bits happen to say.
//
// This makes a modded game safe to join. It does not make an unmodded game safe from a modded
// car: an unmodded game reads 7 and 15 bit cars with 6 and 14 bits and has no fix, and only the
// sender choosing unmodded widths when the car fits in them can help it, which needs every
// Unlimiter game it talks to to read those widths first.

#define NETCAR_QUANTIZERS 0x89CF48
#define NETCAR_QUANT_SLOTS (NETCAR_QUANTIZERS + 0x11C)
#define NETCAR_QUANT_TYPE (NETCAR_QUANTIZERS + 0x260)
#define NETCAR_QUANT_PART (NETCAR_QUANTIZERS + 0x3A4)
#define NETCAR_QUANT_BITS 0x12C // within a quantizer
#define NETCAR_QUANT_MIN 0x130  // added to what is read (sub_582160)
#define NETCAR_PART_BOUND 0x5F0675 // cmp eax, imm32 in sub_5F04D0: indices at or above it set no part

constexpr int NetCarRecordLength = 0x29;
constexpr int NetCarMaxSlots = 0xAA;
constexpr int NetCarVanillaTypeBits = 6;
constexpr int NetCarVanillaPartBits = 14;

// The stream sub_581D90 reads: data first, then the write and read positions, both in bits.
constexpr int NetStreamBytes = 0x400;
#define NetStream_Written(s) ((s)[0x400 / 4])
#define NetStream_ReadPos(s) ((s)[0x404 / 4])

void(__thiscall* NetStream_ReadBits)(DWORD* Stream, int* Out, int Bits) = (void(__thiscall*)(DWORD*, int*, int))0x581D90;
int(__thiscall* NetCar_Read_Game)(DWORD* Quantizers, DWORD* RideInfo, DWORD* Stream, int A4) = (int(__thiscall*)(DWORD*, DWORD*, DWORD*, int))0x5F04D0;
void(__thiscall* RideInfo_Init_Game)(DWORD* RideInfo, int CarType, int A4, int A5, int A6) = (void(__thiscall*)(DWORD*, int, int, int, int))0x610270;
void(__thiscall* RideInfo_SetCompositeNameHash_Game)(DWORD* RideInfo, int Hash) = (void(__thiscall*)(DWORD*, int))0x61C280;

// Where reading may go: what was written if that is believable, the buffer's end otherwise.
int NetStream_End(DWORD* Stream)
{
	int Written = NetStream_Written(Stream);

	return (Written > 0 && Written <= NetStreamBytes * 8) ? Written : NetStreamBytes * 8;
}

// sub_584020 as its callers already assume it works: at most Max bytes into Buffer.
int __fastcall NetStream_ReadBytes(DWORD* Stream, void* EDX_Unused, BYTE* Buffer, int Max)
{
	int Raw = 0;
	NetStream_ReadBits(Stream, &Raw, 16);

	int Length = (short)Raw; // movsx, as the game reads it

	// The game returned a negative length as it was, and sub_5840A0 then wrote its terminator
	// that far before the buffer.
	if (Length <= 0) return 0;

	int Keep = Length < Max ? Length : Max;

	for (int i = 0; i < Keep; i++)
	{
		int Byte = 0;
		NetStream_ReadBits(Stream, &Byte, 8);
		Buffer[i] = (BYTE)Byte;
	}

	// Step over the rest, but not past the end: a claimed 32767 bytes would otherwise send later
	// reads well beyond the stream.
	if (Length > Keep)
	{
		int Pos = NetStream_ReadPos(Stream) + (Length - Keep) * 8;
		int End = NetStream_End(Stream);

		NetStream_ReadPos(Stream) = Pos < End ? Pos : End;
	}

	return Keep;
}

struct NetCarLayout
{
	int TypeBits, PartBits;
	int CarType, Slots;
	int EndBit;
};

// Reads the stream without moving it.
DWORD NetCar_Peek(DWORD* Stream, int& Pos, int Bits)
{
	const BYTE* Data = (const BYTE*)Stream;
	DWORD Value = 0;

	for (int i = 0; i < Bits; i++, Pos++)
	{
		int Byte = Pos >> 3;
		int Bit = Byte < NetStreamBytes ? (Data[Byte] >> (7 - (Pos & 7))) & 1 : 0;

		Value = (Value << 1) | Bit;
	}

	return Value;
}

bool NetCar_TryLayout(DWORD* Stream, int Start, int End, int TypeBits, int PartBits, NetCarLayout& Out)
{
	int Pos = Start;

	int CarType = (int)NetCar_Peek(Stream, Pos, TypeBits) + *(int*)(NETCAR_QUANT_TYPE + NETCAR_QUANT_MIN);
	if (CarType < 0 || CarType >= CarCount) return false;

	Pos += 32 + 10 + 9 * 8;

	int Length = (short)NetCar_Peek(Stream, Pos, 16);
	if (Length != NetCarRecordLength) return false;

	Pos += Length * 8;

	int SlotBits = *(int*)(NETCAR_QUANT_SLOTS + NETCAR_QUANT_BITS);
	int Slots = (int)NetCar_Peek(Stream, Pos, SlotBits) + *(int*)(NETCAR_QUANT_SLOTS + NETCAR_QUANT_MIN);
	if (Slots < 0 || Slots > NetCarMaxSlots) return false;

	Pos += Slots * PartBits;
	if (Pos > End) return false;

	Out = { TypeBits, PartBits, CarType, Slots, Pos };

	return true;
}

// Of the given widths, the first that ends in the stream's last byte, else the first that fits.
bool NetCar_PickLayout(DWORD* Stream, const int (*Candidates)[2], int Count, NetCarLayout& Found)
{
	int Start = NetStream_ReadPos(Stream);
	int End = NetStream_End(Stream);
	bool HaveLoose = false;

	for (int i = 0; i < Count; i++)
	{
		NetCarLayout Layout;

		if (!NetCar_TryLayout(Stream, Start, End, Candidates[i][0], Candidates[i][1], Layout)) continue;

		if (End - Layout.EndBit < 8)
		{
			Found = Layout;
			return true;
		}

		if (!HaveLoose)
		{
			Found = Layout;
			HaveLoose = true;
		}
	}

	return HaveLoose;
}

// The widths this stream was written with. Where the car is all the stream holds, as in the
// lobby, only the right layout ends in its last byte. A race start packet carries a name after
// the car, so there the end proves nothing and the order decides: a car type and its parts come
// from one game, so this game's pair and the unmodded pair are asked first, together, and the
// odd mixtures only when neither fits.
bool NetCar_FindLayout(DWORD* Stream, NetCarLayout& Found)
{
	int LocalType = *(int*)(NETCAR_QUANT_TYPE + NETCAR_QUANT_BITS);
	int LocalPart = *(int*)(NETCAR_QUANT_PART + NETCAR_QUANT_BITS);

	const int Known[2][2] = { { LocalType, LocalPart }, { NetCarVanillaTypeBits, NetCarVanillaPartBits } };

	if (NetCar_PickLayout(Stream, Known, 2, Found)) return true;

	int Others[25][2];
	int Count = 0;

	for (int t = 5; t <= 9; t++)
		for (int p = 13; p <= 17; p++)
		{
			Others[Count][0] = t;
			Others[Count][1] = p;
			Count++;
		}

	return NetCar_PickLayout(Stream, Others, Count, Found);
}

int __fastcall NetCar_Read(DWORD* Quantizers, void* EDX_Unused, DWORD* RideInfo, DWORD* Stream, int A4)
{
	int* TypeBits = (int*)(NETCAR_QUANT_TYPE + NETCAR_QUANT_BITS);
	int* PartBits = (int*)(NETCAR_QUANT_PART + NETCAR_QUANT_BITS);

	NetCarLayout Layout;

	if (!NetCar_FindLayout(Stream, Layout))
	{
		// Nothing this game can make sense of. Leave a car that can be drawn rather than one built
		// from whatever the bits say.
		RideInfo_Init_Game(RideInfo, (ReplacementCar >= 0 && ReplacementCar < CarCount) ? ReplacementCar : 1, A4, 0, 0);
		RideInfo_SetCompositeNameHash_Game(RideInfo, A4);
		RideInfo_SetStockParts(RideInfo, nullptr, 0);
		return 0;
	}

	if (Layout.TypeBits == *TypeBits && Layout.PartBits == *PartBits)
		return NetCar_Read_Game(Quantizers, RideInfo, Stream, A4);

	// Another game's widths. Read it with them, but set none of its parts: the indices point into
	// the sender's part list, not this one.
	int SavedTypeBits = *TypeBits, SavedPartBits = *PartBits;
	int SavedBound = injector::ReadMemory<int>(NETCAR_PART_BOUND, true);

	*TypeBits = Layout.TypeBits;
	*PartBits = Layout.PartBits;
	injector::WriteMemory<int>(NETCAR_PART_BOUND, 0, true);

	int Result = NetCar_Read_Game(Quantizers, RideInfo, Stream, A4);

	*TypeBits = SavedTypeBits;
	*PartBits = SavedPartBits;
	injector::WriteMemory<int>(NETCAR_PART_BOUND, SavedBound, true);

	// With no slots the game has already done this.
	if (Layout.Slots) RideInfo_SetStockParts(RideInfo, nullptr, 0);

	return Result;
}

void InitNetCarData()
{
	injector::MakeJMP(0x584020, NetStream_ReadBytes, true);

	injector::MakeCALL(0x585622, NetCar_Read, true); // sub_5854C0, the lobby's "C" key
	injector::MakeCALL(0x5F982B, NetCar_Read, true); // sub_5F97D0, a race start
}