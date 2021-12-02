#pragma once
#include <cstdint>
#include <wtypes.h>
#include <vector>

struct _globals
{
	HWND hWnd;
	uint64_t pID;

	uint64_t World;
	uint64_t Entitylist;
	uint64_t Entity;
	uint32_t Size;
	HWND GameWnd;

	bool menu = false;
	float width = 1920.f;
	float height = 1080.f;
};

typedef struct _player_t
{
	std::uint64_t EntityPtr;
	std::uint64_t TableEntry;
	int NetworkID;
} player_t;

typedef struct _item_t
{
	std::uint64_t ItemPtr;
	std::uint64_t ItemTable;
} item_t;

extern _globals globals;