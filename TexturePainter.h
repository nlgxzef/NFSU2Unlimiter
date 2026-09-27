#pragma once

#define D3DFMT_A8R8G8B8 21
#define D3DFMT_X8R8G8B8 22
#define D3DFMT_DXT1 0x31545844 // 'DXT1'
#define D3DFMT_DXT2 0x32545844
#define D3DFMT_DXT3 0x33545844
#define D3DFMT_DXT4 0x34545844
#define D3DFMT_DXT5 0x35545844
#define D3DLOCK_READONLY 0x10

enum TexturePixelFormat
{
	PIXEL_FORMAT_NONE,
	PIXEL_FORMAT_32BIT,
	PIXEL_FORMAT_DXT1,
	PIXEL_FORMAT_DXT35
};

struct SurfaceDesc
{
	DWORD Format, Type, Usage, Pool, MultiSampleType, MultiSampleQuality;
	unsigned int Width, Height;
};

struct LockedRect
{ 
	int Pitch;
	BYTE* Bits;
};

// One mip level as this code walks it: pixel rows for 32 bit, rows of 4x4 blocks for DXT.
struct MipLevel
{
	int Kind;
	DWORD Format;
	int Width, Height;
	int Rows, RowBytes, BlockBytes;
};

bool Tex_ValidPtr(void* p)
{
	uintptr_t v = (uintptr_t)p;
	return v >= 0x00010000 && v <= 0xC0000000 && !(v & 3);
}

void* GetD3DTexture(void* TextureInfo)
{
	if (!Tex_ValidPtr(TextureInfo)) return nullptr;

	BYTE* Plat = *(BYTE**)TextureInfo;

	if (!Tex_ValidPtr(Plat)) return nullptr;

	void* Texture = *(void**)(Plat + 0x18);

	return Tex_ValidPtr(Texture) ? Texture : nullptr;
}

int GetMipLevelCount(void* Texture)
{
	typedef DWORD(__stdcall* GetLevelCountFn)(void*);

	return (int)((GetLevelCountFn)(*(void***)Texture)[0x34 / 4])(Texture);
}

bool MipLevelLayout(void* Texture, int Level, MipLevel& L)
{
	typedef long(__stdcall* GetLevelDescFn)(void*, UINT, SurfaceDesc*);

	SurfaceDesc Desc = {};
	L = {};

	if (((GetLevelDescFn)(*(void***)Texture)[0x44 / 4])(Texture, (UINT)Level, &Desc) < 0) return false;

	L.Format = Desc.Format;
	L.Width = (int)Desc.Width;
	L.Height = (int)Desc.Height;

	if (L.Width <= 0 || L.Height <= 0) return false;

	switch (Desc.Format)
	{
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
		L.Kind = PIXEL_FORMAT_32BIT;
		L.BlockBytes = 4;
		L.Rows = L.Height;
		L.RowBytes = L.Width * 4;
		return true;

	case D3DFMT_DXT1:
		L.Kind = PIXEL_FORMAT_DXT1;
		L.BlockBytes = 8;
		break;

	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		L.Kind = PIXEL_FORMAT_DXT35;
		L.BlockBytes = 16;
		break;

	default:
		return true;
	}

	L.Rows = (L.Height + 3) / 4;
	L.RowBytes = ((L.Width + 3) / 4) * L.BlockBytes;

	return true;
}

bool Lock(void* Texture, int Level, int RowBytes, LockedRect& Rect, DWORD Flags)
{
	typedef long(__stdcall* LockRectFn)(void*, unsigned int, LockedRect*, const void*, unsigned long);
	typedef long(__stdcall* UnlockRectFn)(void*, unsigned int);

	Rect.Pitch = 0;
	Rect.Bits = nullptr;

	if (((LockRectFn)(*(void***)Texture)[0x4C / 4])(Texture, (unsigned int)Level, &Rect, nullptr, Flags) < 0) return false;

	if (!Rect.Bits || Rect.Pitch < RowBytes)
	{
		((UnlockRectFn)(*(void***)Texture)[0x50 / 4])(Texture, (unsigned int)Level);
		return false;
	}

	return true;
}

void Unlock(void* Texture, int Level)
{
	typedef long(__stdcall* UnlockRectFn)(void*, unsigned int);

	((UnlockRectFn)(*(void***)Texture)[0x50 / 4])(Texture, (unsigned int)Level);
}

static inline int ClampColor(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }

struct RGB { int r, g, b; };

static inline bool operator==(const RGB& a, const RGB& b) { return a.r == b.r && a.g == b.g && a.b == b.b; }

static inline int Distance(const RGB& a, const RGB& b)
{
	int dr = a.r - b.r, dg = a.g - b.g, db = a.b - b.b;
	return dr * dr + dg * dg + db * db;
}

// The vinyl mix with only the first colour set: red becomes the colour, scaled by how red the
// texel was, green and blue carry on as themselves, and the mask's coverage blends it in.
static inline RGB Mix(const RGB& p, int Cover, int CR, int CG, int CB)
{
	if (!Cover) return p;

	int nr = ClampColor(p.r * CR / 255);
	int ng = ClampColor(p.r * CG / 255 + p.g);
	int nb = ClampColor(p.r * CB / 255 + p.b);

	return { p.r + (nr - p.r) * Cover / 255, p.g + (ng - p.g) * Cover / 255, p.b + (nb - p.b) * Cover / 255 };
}

static inline RGB Expand565(WORD c)
{
	int r = (c >> 11) & 31, g = (c >> 5) & 63, b = c & 31;

	return { (r << 3) | (r >> 2), (g << 2) | (g >> 4), (b << 3) | (b >> 2) };
}

static inline WORD Pack565(int r, int g, int b)
{
	r = (ClampColor(r) * 31 + 127) / 255;
	g = (ClampColor(g) * 63 + 127) / 255;
	b = (ClampColor(b) * 31 + 127) / 255;

	return (WORD)((r << 11) | (g << 5) | b);
}

static inline WORD Pack565(const double* c)
{
	return Pack565((int)(c[0] + 0.5), (int)(c[1] + 0.5), (int)(c[2] + 0.5));
}

static void Palette4(WORD c0, WORD c1, RGB Pal[4])
{
	Pal[0] = Expand565(c0);
	Pal[1] = Expand565(c1);
	Pal[2] = { (2 * Pal[0].r + Pal[1].r) / 3, (2 * Pal[0].g + Pal[1].g) / 3, (2 * Pal[0].b + Pal[1].b) / 3 };
	Pal[3] = { (Pal[0].r + 2 * Pal[1].r) / 3, (Pal[0].g + 2 * Pal[1].g) / 3, (Pal[0].b + 2 * Pal[1].b) / 3 };
}

// Decodes a colour half into Px. FourColour says the block is read as four colours, which is what
// makes its own endpoints worth reusing; Opaque is false for a DXT1 block using its transparent
// texel.
static void DecodeColour(const BYTE* Block, bool Dxt1, RGB Px[16], int Idx[16], bool& FourColour, bool& Opaque)
{
	WORD c0 = *(const WORD*)Block, c1 = *(const WORD*)(Block + 2);
	DWORD Bits = *(const DWORD*)(Block + 4);
	RGB Pal[4];

	FourColour = !Dxt1 || c0 > c1;
	Opaque = true;

	if (FourColour) Palette4(c0, c1, Pal);
	else
	{
		Pal[0] = Expand565(c0);
		Pal[1] = Expand565(c1);
		Pal[2] = { (Pal[0].r + Pal[1].r) / 2, (Pal[0].g + Pal[1].g) / 2, (Pal[0].b + Pal[1].b) / 2 };
		Pal[3] = { 0, 0, 0 };
	}

	for (int i = 0; i < 16; i++)
	{
		Idx[i] = (Bits >> (2 * i)) & 3;
		Px[i] = Pal[Idx[i]];

		if (!FourColour && Idx[i] == 3) Opaque = false;
	}
}

// The nearest of the four colours for every texel, and the total squared error.
static int PickIndices(const RGB Px[16], WORD c0, WORD c1, int Idx[16])
{
	RGB Pal[4];
	Palette4(c0, c1, Pal);

	int Error = 0;

	for (int i = 0; i < 16; i++)
	{
		int Best = 0, BestDistance = Distance(Px[i], Pal[0]);

		for (int k = 1; k < 4; k++)
		{
			int d = Distance(Px[i], Pal[k]);
			if (d < BestDistance) { Best = k; BestDistance = d; }
		}

		Idx[i] = Best;
		Error += BestDistance;
	}

	return Error;
}

// Moves both endpoints to where the chosen indices want them, least squares, and keeps the move
// while it helps.
static void Refine(const RGB Px[16], WORD& c0, WORD& c1, int Idx[16], int& Error)
{
	static const double Weight0[4] = { 1.0, 0.0, 2.0 / 3.0, 1.0 / 3.0 };

	for (int Pass = 0; Pass < 2; Pass++)
	{
		double A = 0, B = 0, C = 0, X0[3] = { 0, 0, 0 }, X1[3] = { 0, 0, 0 };

		for (int i = 0; i < 16; i++)
		{
			double w0 = Weight0[Idx[i]], w1 = 1.0 - w0;
			double p[3] = { (double)Px[i].r, (double)Px[i].g, (double)Px[i].b };

			A += w0 * w0; B += w0 * w1; C += w1 * w1;

			for (int k = 0; k < 3; k++) { X0[k] += w0 * p[k]; X1[k] += w1 * p[k]; }
		}

		double Det = A * C - B * B;

		if (Det < 1e-6 && Det > -1e-6) return;

		double E0[3], E1[3];

		for (int k = 0; k < 3; k++)
		{
			E0[k] = (C * X0[k] - B * X1[k]) / Det;
			E1[k] = (A * X1[k] - B * X0[k]) / Det;
		}

		WORD n0 = Pack565(E0), n1 = Pack565(E1);
		int NewIdx[16];
		int NewError = PickIndices(Px, n0, n1, NewIdx);

		if (NewError >= Error) return;

		c0 = n0; c1 = n1; Error = NewError;
		memcpy(Idx, NewIdx, sizeof(NewIdx));
	}
}

// A fresh fit: the endpoints at either end of the texels' spread along their principal axis.
static int FitColour(const RGB Px[16], WORD& c0, WORD& c1, int Idx[16])
{
	double Mean[3] = { 0, 0, 0 };

	for (int i = 0; i < 16; i++) { Mean[0] += Px[i].r; Mean[1] += Px[i].g; Mean[2] += Px[i].b; }
	for (int k = 0; k < 3; k++) Mean[k] /= 16.0;

	double Cov[3][3] = {};

	for (int i = 0; i < 16; i++)
	{
		double d[3] = { Px[i].r - Mean[0], Px[i].g - Mean[1], Px[i].b - Mean[2] };

		for (int a = 0; a < 3; a++)
			for (int b = 0; b < 3; b++)
				Cov[a][b] += d[a] * d[b];
	}

	// Power iteration from the covariance column with the most in it, which cannot be orthogonal to
	// the principal axis the way a fixed start like (1,1,1) can.
	int Start = 0;

	for (int k = 1; k < 3; k++)
		if (Cov[k][k] > Cov[Start][Start]) Start = k;

	double Axis[3] = { Cov[0][Start], Cov[1][Start], Cov[2][Start] };

	for (int Iteration = 0; Iteration < 8; Iteration++)
	{
		double v[3];
		double Length = 0;

		for (int a = 0; a < 3; a++)
		{
			v[a] = Cov[a][0] * Axis[0] + Cov[a][1] * Axis[1] + Cov[a][2] * Axis[2];
			Length = Length > (v[a] < 0 ? -v[a] : v[a]) ? Length : (v[a] < 0 ? -v[a] : v[a]);
		}

		if (Length < 1e-9) break;

		for (int a = 0; a < 3; a++) Axis[a] = v[a] / Length;
	}

	double Norm = Axis[0] * Axis[0] + Axis[1] * Axis[1] + Axis[2] * Axis[2];

	if (Norm < 1e-12)
	{
		// One colour throughout
		c0 = c1 = Pack565(Mean);
		return PickIndices(Px, c0, c1, Idx);
	}

	double Min = 1e30, Max = -1e30;

	for (int i = 0; i < 16; i++)
	{
		double t = ((Px[i].r - Mean[0]) * Axis[0] + (Px[i].g - Mean[1]) * Axis[1] + (Px[i].b - Mean[2]) * Axis[2]) / Norm;

		if (t < Min) Min = t;
		if (t > Max) Max = t;
	}

	double E0[3], E1[3];

	for (int k = 0; k < 3; k++)
	{
		E0[k] = Mean[k] + Axis[k] * Max;
		E1[k] = Mean[k] + Axis[k] * Min;
	}

	c0 = Pack565(E0);
	c1 = Pack565(E1);

	int Error = PickIndices(Px, c0, c1, Idx);

	Refine(Px, c0, c1, Idx, Error);

	return Error;
}

// Writes a four colour block. The larger endpoint has to come first for DXT1 to read four colours
// rather than three and a transparent one, so the ends are swapped where needed, which swaps index
// 0 with 1 and 2 with 3. Two equal ends leave nothing to choose between and take index 0 throughout.
static void WriteColour(BYTE* Block, WORD c0, WORD c1, const int Idx[16])
{
	bool Swap = c0 < c1;

	if (Swap) { WORD t = c0; c0 = c1; c1 = t; }

	DWORD Bits = 0;

	if (c0 != c1)
		for (int i = 0; i < 16; i++)
			Bits |= (DWORD)(Swap ? Idx[i] ^ 1 : Idx[i]) << (2 * i);

	*(WORD*)Block = c0;
	*(WORD*)(Block + 2) = c1;
	*(DWORD*)(Block + 4) = Bits;
}

// The mask's top level as one byte of coverage per texel, its brightest channel.
struct Coverage
{
	int Width = 0, Height = 0;
	std::vector<BYTE> Map;

	// A texel of a W by H level takes the average over the part of the mask it covers.
	int At(int x, int y, int W, int H) const
	{
		if (x >= W || y >= H) return 0;

		int x0 = x * Width / W, x1 = (x + 1) * Width / W;
		int y0 = y * Height / H, y1 = (y + 1) * Height / H;

		if (x1 <= x0) x1 = x0 + 1;
		if (y1 <= y0) y1 = y0 + 1;
		if (x1 > Width) x1 = Width;
		if (y1 > Height) y1 = Height;

		int Sum = 0;

		for (int yy = y0; yy < y1; yy++)
			for (int xx = x0; xx < x1; xx++)
				Sum += Map[(size_t)yy * Width + xx];

		return Sum / ((x1 - x0) * (y1 - y0));
	}
};

static inline BYTE Brightest(int r, int g, int b)
{
	return (BYTE)(r > g ? (r > b ? r : b) : (g > b ? g : b));
}

// Read only, so D3D does not count the mask as changed and send it to the card again.
bool ReadCoverage(void* Mask, const MipLevel& L, Coverage& Cover)
{
	LockedRect Rect;

	if (!Lock(Mask, 0, L.RowBytes, Rect, D3DLOCK_READONLY)) return false;

	Cover.Width = L.Width;
	Cover.Height = L.Height;
	Cover.Map.assign((size_t)L.Width * L.Height, 0);

	if (L.Kind == PIXEL_FORMAT_32BIT)
	{
		for (int y = 0; y < L.Height; y++)
		{
			const DWORD* Row = (const DWORD*)(Rect.Bits + (size_t)y * Rect.Pitch);

			for (int x = 0; x < L.Width; x++)
				Cover.Map[(size_t)y * L.Width + x] = Brightest((Row[x] >> 16) & 0xFF, (Row[x] >> 8) & 0xFF, Row[x] & 0xFF);
		}
	}
	else
	{
		int ColourOffset = L.Kind == PIXEL_FORMAT_DXT1 ? 0 : 8;
		int BlocksX = L.RowBytes / L.BlockBytes;

		for (int by = 0; by < L.Rows; by++)
			for (int bx = 0; bx < BlocksX; bx++)
			{
				RGB Px[16];
				int Idx[16];
				bool FourColour, Opaque;

				DecodeColour(Rect.Bits + (size_t)by * Rect.Pitch + bx * L.BlockBytes + ColourOffset,
					L.Kind == PIXEL_FORMAT_DXT1, Px, Idx, FourColour, Opaque);

				for (int i = 0; i < 16; i++)
				{
					int x = bx * 4 + (i & 3), y = by * 4 + (i >> 2);

					if (x < L.Width && y < L.Height)
						Cover.Map[(size_t)y * L.Width + x] = Brightest(Px[i].r, Px[i].g, Px[i].b);
				}
			}
	}

	Unlock(Mask, 0);

	return true;
}

struct TexPristine
{
	void* Texture;
	DWORD Hash;
	std::vector<MipLevel> Layout;
	std::vector<std::vector<BYTE>> Levels; // each level's rows, tightly packed
};

std::vector<TexPristine> PristineCopies;

void* LastPaintedTexture = nullptr;
DWORD* LastPaintPart = nullptr;

// Every level as it came out of the pack, taken the first time the texture is seen.
TexPristine* Pristine(void* Tex, DWORD Hash)
{
	int Levels = GetMipLevelCount(Tex);

	for (auto& C : PristineCopies)
		if (C.Texture == Tex && C.Hash == Hash && (int)C.Layout.size() == Levels) return &C;

	TexPristine Copy = { Tex, Hash };

	for (int Level = 0; Level < Levels; Level++)
	{
		MipLevel L;
		LockedRect Rect;

		if (!MipLevelLayout(Tex, Level, L) || L.Kind == PIXEL_FORMAT_NONE) return nullptr;
		if (!Lock(Tex, Level, L.RowBytes, Rect, D3DLOCK_READONLY)) return nullptr;

		std::vector<BYTE> Bytes((size_t)L.Rows * L.RowBytes);

		for (int y = 0; y < L.Rows; y++)
			memcpy(&Bytes[(size_t)y * L.RowBytes], Rect.Bits + (size_t)y * Rect.Pitch, L.RowBytes);

		Unlock(Tex, Level);

		Copy.Layout.push_back(L);
		Copy.Levels.push_back(std::move(Bytes));
	}

	PristineCopies.push_back(std::move(Copy));

	return &PristineCopies.back();
}

// Paints one level from its pristine bytes into the locked level.
void PaintLevel(const MipLevel& L, const BYTE* Src, LockedRect& Rect, const Coverage& Cover,
	int CR, int CG, int CB)
{
	if (L.Kind == PIXEL_FORMAT_32BIT)
	{
		for (int y = 0; y < L.Height; y++)
		{
			const DWORD* In = (const DWORD*)(Src + (size_t)y * L.RowBytes);
			DWORD* Out = (DWORD*)(Rect.Bits + (size_t)y * Rect.Pitch);

			for (int x = 0; x < L.Width; x++)
			{
				DWORD P = In[x];
				int c = Cover.At(x, y, L.Width, L.Height);

				if (!c) { Out[x] = P; continue; }

				// A8R8G8B8 in memory: blue, green, red, alpha
				RGB p = { (int)(P >> 16) & 0xFF, (int)(P >> 8) & 0xFF, (int)P & 0xFF };
				RGB n = Mix(p, c, CR, CG, CB);

				Out[x] = (P & 0xFF000000) | ((DWORD)n.r << 16) | ((DWORD)n.g << 8) | (DWORD)n.b;
			}
		}

		return;
	}

	bool Dxt1 = L.Kind == PIXEL_FORMAT_DXT1;
	int ColourOffset = Dxt1 ? 0 : 8;
	int BlocksX = L.RowBytes / L.BlockBytes;

	for (int by = 0; by < L.Rows; by++)
	{
		for (int bx = 0; bx < BlocksX; bx++)
		{
			const BYTE* In = Src + (size_t)by * L.RowBytes + bx * L.BlockBytes;
			BYTE* Out = Rect.Bits + (size_t)by * Rect.Pitch + bx * L.BlockBytes;

			// Back to the authored block first, alpha half included, so anything not painted below
			// is exactly what the pack held.
			memcpy(Out, In, L.BlockBytes);

			RGB Px[16], Target[16];
			int OwnIdx[16], Covers[16];
			bool FourColour, Opaque;

			DecodeColour(In + ColourOffset, Dxt1, Px, OwnIdx, FourColour, Opaque);

			int CoverSum = 0;
			bool Changed = false;

			for (int i = 0; i < 16; i++)
			{
				int x = bx * 4 + (i & 3), y = by * 4 + (i >> 2);

				Covers[i] = Cover.At(x, y, L.Width, L.Height);
				Target[i] = Mix(Px[i], Covers[i], CR, CG, CB);
				CoverSum += Covers[i];

				if (!(Target[i] == Px[i])) Changed = true;
			}

			if (!Changed || !Opaque) continue;

			// The block's own ends through the mix, at the block's average coverage
			WORD Best0 = 0, Best1 = 0;
			int BestIdx[16];
			int BestError = 0x7FFFFFFF;

			if (FourColour)
			{
				int c = CoverSum / 16;
				RGB E0 = Mix(Expand565(*(const WORD*)(In + ColourOffset)), c, CR, CG, CB);
				RGB E1 = Mix(Expand565(*(const WORD*)(In + ColourOffset + 2)), c, CR, CG, CB);

				Best0 = Pack565(E0.r, E0.g, E0.b);
				Best1 = Pack565(E1.r, E1.g, E1.b);
				BestError = PickIndices(Target, Best0, Best1, BestIdx);

				Refine(Target, Best0, Best1, BestIdx, BestError);
			}

			// A fresh fit, for when the mask does not cover the block evenly
			if (BestError)
			{
				WORD f0, f1;
				int FitIdx[16];
				int FitError = FitColour(Target, f0, f1, FitIdx);

				if (FitError < BestError)
				{
					Best0 = f0; Best1 = f1; BestError = FitError;
					memcpy(BestIdx, FitIdx, sizeof(FitIdx));
				}
			}

			WriteColour(Out + ColourOffset, Best0, Best1, BestIdx);
		}
	}
}

// Paint via RideInfo

void TexturePainter_PaintTireTexture(DWORD* RideInfo, DWORD TextureHash)
{
	if (!TextureHash || !RideInfo) return;

	DWORD* Part = (DWORD*)RideInfo[356 + CARSLOTID_UL_TIRE_TEXTURE];

	if (!Part || CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("UNPAINTABLE"), 1)) return;

	void* TexInfo = (void*)hb_GetTextureInfo.fun(TextureHash, 0, 0);
	void* MaskInfo = (void*)hb_GetTextureInfo.fun(bStringHash2("_MASK", TextureHash), 0, 0);

	if (!TexInfo || !MaskInfo) return;

	void* Tex = GetD3DTexture(TexInfo);
	void* Mask = GetD3DTexture(MaskInfo);

	if (!Tex || !Mask) return;

	DWORD* ColourPart = (DWORD*)RideInfo[356 + CARSLOTID_UL_TIRE_SMOKE];

	// Only redo it when something it depends on has changed. The texture is part of that: a pack
	// unloaded and loaded again hands back fresh pixels in a new texture.
	if (Tex == LastPaintedTexture && ColourPart == LastPaintPart) return;

	MipLevel MaskTop;

	if (!MipLevelLayout(Mask, 0, MaskTop) || MaskTop.Kind == PIXEL_FORMAT_NONE) return;

	TexPristine* Copy = Pristine(Tex, TextureHash);
	Coverage Cover;

	if (!Copy || !ReadCoverage(Mask, MaskTop, Cover)) return;

	// No colour defaults to white
	int CR = 255, CG = 255, CB = 255;
	bool HasColour = ColourPart && ColourPart[0] != CT_bStringHash("VINYL_L1_COLOR01");

	if (HasColour)
	{
		CR = ClampColor(CarPart_GetAppliedAttributeUParam(ColourPart, CT_bStringHash("RED"), 0));
		CG = ClampColor(CarPart_GetAppliedAttributeUParam(ColourPart, CT_bStringHash("GREEN"), 0));
		CB = ClampColor(CarPart_GetAppliedAttributeUParam(ColourPart, CT_bStringHash("BLUE"), 0));
	}

	for (int Level = 0; Level < (int)Copy->Layout.size(); Level++)
	{
		const MipLevel& L = Copy->Layout[Level];
		LockedRect Rect;

		// A level that will not lock keeps what it had.
		if (!Lock(Tex, Level, L.RowBytes, Rect, 0)) continue;

		PaintLevel(L, Copy->Levels[Level].data(), Rect, Cover, CR, CG, CB);
		Unlock(Tex, Level);
	}

	LastPaintedTexture = Tex;
	LastPaintPart = ColourPart;
}