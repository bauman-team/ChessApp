#include "pch.h"
#include "Pos.h"
#include <iostream>

#define UseAsm

const Pos Pos::NULL_POS{ };

Pos::Pos(uint8_t x, uint8_t y) noexcept
{
#ifdef UseAsm
#ifdef _WIN32
	_asm {
			push ebx
			push ax
			mov ebx, this
			mov al, 255
			cmp x, 8
			jae FINISH
			cmp x, 0
			jb FINISH
			cmp y, 8
			jae FINISH
			cmp y, 0
			jb FINISH
			mov al, y
			shl x, 4
			or al, x
		FINISH: mov [ebx].xy, al
			pop ax
			pop ebx
	}
#endif
#ifdef __linux__
	asm(
		"mov $255, %%al\n\t"
		"mov %[x], %%bl\n\t"
		"cmp $8, %%bl\n\t"
		"jae FINISH\n\t"
		"cmp $0, %%bl\n\t"
		"jb FINISH\n\t"
		"mov %[y], %%bl\n\t"
		"cmp $8, %%bl\n\t"
		"jae FINISH\n\t"
		"cmp $0, %%bl\n\t"
		"jb FINISH\n\t"
		"mov %[x], %%al\n\t"
		"shl $4, %%al\n\t"
		"or %%bl, %%al\n\t"
		"FINISH:\n\t"
		"mov %%al, %[xy]\n\t"
		: [xy]"=m"(this->xy)
		: [x]"m"(x), [y]"m"(y)
		: "eax", "ebx", "ecx");
#endif
#endif
#ifndef UseAsm
	xy = 255;
	if (x < 8 && x >= 0 && y < 8 && y >= 0)
		xy = (x << 4) | y;
#endif
}

auto Pos::GetX() const noexcept -> uint8_t
{
	return xy >> 4;
}

auto Pos::GetY() const noexcept -> uint8_t
{
	return xy & 15;
}

auto Pos::ToIndex() const noexcept -> uint8_t
{
	return (xy & 15) * 8 + (xy >> 4);
}

auto Pos::ToBitboard() const noexcept -> uint64_t
{
#ifdef UseAsm
	uint32_t bitboardL;
	uint64_t bitboard;
	auto y{ GetY() }, x{ GetX() };
	auto offset = y > 3;
	if (offset)
		y -= 4;
#ifdef _WIN32
	_asm {
			push ecx
			push ebx
			mov ecx, 0
			mov ebx, 1
		SETY: cmp cl, y
			je FINY
			shl ebx, 8
			inc cl
			jmp SETY
		FINY: mov ecx, 0
		SETX: cmp cl, x
			je FINX
			shl ebx, 1
			inc cl
			jmp SETX
		FINX: mov bitboardL, ebx
			pop ebx 
			pop ecx
	}
#endif
#ifdef __linux__
	asm(
		"mov $0, %%ecx\n\t"
		"mov $1, %%ebx\n\t"
		"SETY: cmp %[y], %%cl\n\t"
		"je FINY\n\t"
		"shl $8, %%ebx\n\t"
		"inc %%cl\n\t"
		"jmp SETY\n\t"
		"FINY: mov $0, %%ecx\n\t"
		"SETX: cmp %[x], %%cl\n\t"
		"je FINX\n\t"
		"shl $1, %%ebx\n\t"
		"inc %%cl\n\t"
		"jmp SETX\n\t"
		"FINX: mov %%ebx, %[bitboardL]\n\t"
		: [bitboardL]"=m"(bitboardL)
		: [x]"m"(x), [y]"m"(y)
		: "ebx", "ecx"
		);
#endif
	bitboard = bitboardL;
	if (offset)
		bitboard <<= 32;
#endif
#ifndef UseAsm
	uint64_t bitboard = 1;
	for (auto i = 0; i != GetY(); ++i)
		bitboard <<= 8;
	for (auto i = 0; i != GetX(); ++i)
		bitboard <<= 1;
#endif
	return bitboard;
}

auto Pos::IsValid() const noexcept -> bool
{
	return xy != 255;
}

auto Pos::operator==(const Pos& coords) const noexcept -> bool
{
	return xy == coords.xy;
}

auto Pos::operator!=(const Pos& coords) const noexcept -> bool
{
	return xy != coords.xy;
}

auto Pos::ToString() const -> std::string
{
	return static_cast<char>((xy >> 4) + 'A') + std::to_string((xy & 15) + 1);
}

auto Pos::Add(int8_t x, int8_t y) const noexcept -> Pos
{
	return Pos{ static_cast<uint8_t>((xy >> 4) + x), static_cast<uint8_t>((xy & 15) + y) };
}

auto Pos::BitboardToPosition(uint64_t bitboard) noexcept -> Pos
{
	uint8_t x = 0, y = 0;
#ifdef UseAsm
	uint32_t bitboardL;
	if (bitboard > 4'294'967'295)
	{
		y += 4;
		bitboard >>= 32;
	}
	bitboardL = bitboard;
#ifdef _WIN32
	_asm {
		push ebx
		mov ebx, bitboardL
		GETY: cmp ebx, 128
		jbe GETX
		shr ebx, 8
		inc y
		jmp GETY
		GETX: cmp ebx, 1
		jbe FIN
		shr ebx, 1
		inc x
		jmp GETX
		FIN:
		pop ebx
	}
#endif
#ifdef __linux__
	asm(
		"mov %[bitboardL], %%ebx\n\t"
		"mov %[y], %%cl\n\t"
		"GETY: cmp $128, %%ebx\n\t"
		"jbe FINGETY\n\t"
		"shr $8, %%ebx\n\t"
		"inc %%cl\n\t"
		"jmp GETY\n\t"
		"FINGETY: mov %%cl, %[y]\n\t"
		"mov $0, %%cl\n\t"
		"GETX: cmp $1, %%ebx\n\t"
		"jbe FINGETX\n\t"
		"shr $1, %%ebx\n\t"
		"inc %%cl\n\t"
		"jmp GETX\n\t"
		"FINGETX: mov %%cl, %[x]\n\t"
		: [x]"=m"(x), [y]"=m"(y)
		: [bitboardL]"m"(bitboardL)
		: "ebx", "ecx"
		);
#endif
#endif
#ifndef UseAsm
	while (bitboard > 128)
	{
		++y;
		bitboard >>= 8;
	}
	while (bitboard > 1)
	{
		++x;
		bitboard >>= 1;
	}
#endif
	return Pos{ x, y };
}

auto Pos::IndexToPosition(uint8_t index) noexcept -> Pos
{
	return Pos{ static_cast<uint8_t>(index % 8), static_cast<uint8_t>(index / 8) };
}
