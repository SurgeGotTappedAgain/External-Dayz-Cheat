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

	bool menu = false;
	float width = 1920.f;
	float height = 1080.f;
};

extern _globals globals;