#pragma once

#include "stdio.h"
#include <windows.h>

enum reg : int
{
	eax,
	ebx,
	ecx,
	edx,
	edi,
	esi,
	esp
};

typedef struct
{
	reg reg;
	void* val;
} FuncArg;

typedef struct
{
	void* _eax;
	void* _ebx;
	void* _ecx;
	void* _edx;
	void* _edi;
	void* _esi;
} Regs;

/* A wrapper to call the functions which doesn't fit any calling convention.
 * This is usually caused by the "Link-Time Code Generation".
 *
 * Usage: After number of arguments, you need to push the register name (eax, ebx, etc.) and the argument itself.
 * If argument is pushed regularly, use "esp" as the register name.
 * Stack difference will get calculated by the function automatically.
 *
 * For example: MakeUserCall<int>(FEngPrintf, 3, eax, FEPkg, edx, buf, esp, str);
 *		<int>			// It's int by default. Don't use "<void>" in here as it will give you a compile-time error.
 *		FEngPrintf		// The name of the function we want to call.
 *		3				// Number of arguments taken by the function.
 *		eax, FEPkg		// "FEPkg" will get passed through "eax" register. (ASM: mov eax, FEPkg)
 *		edx, buf		// "buf" will get passed through "edx" register. (ASM: mov edx, buf)
 *		esp, str		// "str" will get passed through a regular "push" instruction. (ASM: push str)
*/
template <typename R = int>
R MakeUserCall(R _func, int _numArgs, ...)
{
	R _retval = 0;
	std::vector<FuncArg> _args;
	std::vector<FuncArg> _pushArgs;
	FuncArg _anArg;
	Regs _regs, _regs_backup;
	va_list ap;

	// Get current registers
	_asm
	{
		mov  _regs._eax, eax
		mov  _regs._ebx, ebx
		mov  _regs._ecx, ecx
		mov  _regs._edx, edx
		mov  _regs._edi, edi
		mov  _regs._esi, esi
	}

	// backup regs
	_regs_backup = _regs;

	va_start(ap, _numArgs);

	for (int i = 0; i < _numArgs; i++)
	{
		_anArg.reg = va_arg(ap, reg);
		_anArg.val = va_arg(ap, void*);

		switch (_anArg.reg)
		{
		case esp:
			_pushArgs.push_back(_anArg);
			break;
		default:
			_args.push_back(_anArg);
			break;
		}
	}

	va_end(ap);

	// move args to the register struct
	for (FuncArg i : _args)
	{
		switch (i.reg)
		{
		case eax:
			_regs._eax = i.val;
			break;
		case ebx:
			_regs._ebx = i.val;
			break;
		case ecx:
			_regs._ecx = i.val;
			break;
		case edx:
			_regs._edx = i.val;
			break;
		case edi:
			_regs._edi = i.val;
			break;
		case esi:
			_regs._esi = i.val;
			break;
		}
	}

	// push args in reverse order
	reverse(_pushArgs.begin(), _pushArgs.end());

	DWORD esp_target;
	_asm mov[esp_target], esp

	for (FuncArg s : _pushArgs)
	{
		_asm push s.val;
	}

	// Set registers before call
	_asm
	{
		mov eax, _regs._eax
		mov ebx, _regs._ebx
		mov ecx, _regs._ecx
		mov edx, _regs._edx
		mov edi, _regs._edi
		mov esi, _regs._esi

		// invoke the call
		call _func;
		mov esp, [esp_target]
			mov _retval, eax;
	}

	// Restore regs
	_asm
	{
		//mov eax, _regs_backup._eax
		mov ebx, _regs_backup._ebx
		mov ecx, _regs_backup._ecx
		mov edx, _regs_backup._edx
		mov edi, _regs_backup._edi
		mov esi, _regs_backup._esi
	}

	return _retval;
}