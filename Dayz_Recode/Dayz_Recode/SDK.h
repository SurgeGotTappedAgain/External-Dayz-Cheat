#pragma once
#include "Driver.h"
#include "Vector.h"
#include "Globals.h"

namespace Game
{
	uint64_t NearEntityTable()
	{
		return driver::Read<uint64_t>(globals.World + 0xEA8);
	}

	uint32_t NearEntityTableSize()
	{
		return driver::Read<uint32_t>(globals.World + 0xEA8 + 0x08);
	}

	uint64_t GetEntity(uint64_t PlayerList, uint64_t SelectedPlayer)
	{
		// Sorted Object
		return driver::Read<uint64_t>(PlayerList + SelectedPlayer * 0x8);
	}

	std::string GetEntityTypeName(uint64_t Entity)
	{
		// Render Entity Type + Config Name
		return driver::ReadArmaString(driver::Read<uint64_t>(driver::Read<uint64_t>(Entity + 0x148) + 0xA0)).c_str();
	}

	uint64_t FarEntityTable()
	{
		return driver::Read<uint64_t>(globals.World + 0xFF0);
	}

	uint32_t FarEntityTableSize()
	{
		return driver::Read<uint32_t>(globals.World + 0xFF0 + 0x08);
	}

	uint64_t GetCameraOn()
	{
		// Camera On 
		return driver::Read<uint64_t>(globals.World + 0x28A8);
	}

	uint64_t GetLocalPlayer()
	{
		// Sorted Entity Object
		return driver::Read<uint64_t>(driver::Read<uint64_t>(globals.World + 0x28A8) + 0x8) - 0xA8;
	}

	uint64_t GetLocalPlayerVisualState()
	{
		// Future Visual State
		return driver::Read<uint64_t>(GetLocalPlayer() + 0x198);
	}

	Vector3 GetCoordinate(uint64_t Entity)
	{
		while (true)
		{
			if (Entity == GetLocalPlayer())
			{
				return Vector3(driver::Read<Vector3>(GetLocalPlayerVisualState() + 0x2C));
			}
			else
			{
				return  Vector3(driver::Read<Vector3>(driver::Read<uint64_t>(Entity + 0x198) + 0x2C));
			}
		}
	}
	uint64_t GetCamera()
	{
		while (true)
		{
			return driver::Read<uint64_t>(globals.World + 0x1B8);
		}
	}

	Vector3 GetInvertedViewTranslation()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x2C));
	}
	Vector3 GetInvertedViewRight()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x8));
	}
	Vector3 GetInvertedViewUp()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x14));
	}
	Vector3 GetInvertedViewForward()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x20));
	}

	Vector3 GetViewportSize()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x58));
	}

	Vector3 GetProjectionD1()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0xD0));
	}

	Vector3 GetProjectionD2()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0xDC));
	}


	uint32_t GetSlowEntityTableSize()
	{
		return driver::Read<uint32_t>(globals.World + 0x1F40 + 0x08);
	}

	float GetDistanceToMe(Vector3 Entity)
	{
		return Entity.Distance(GetCoordinate(GetLocalPlayer()));
	}

	Vector3 WorldToScreen(Vector3 Position)
	{
		if (!GetCamera()) return Vector3();

		Vector3 temp = Position - GetInvertedViewTranslation();

		float x = temp.Dot(GetInvertedViewRight());
		float y = temp.Dot(GetInvertedViewUp());
		float z = temp.Dot(GetInvertedViewForward());

		int width = GetSystemMetrics(SM_CXSCREEN);

		if (z > 0)
		{
			return Vector3(
				GetViewportSize().x * (1 + (x / GetProjectionD1().x / z)),
				GetViewportSize().y * (1 - (y / GetProjectionD2().y / z)),
				z);
		}

	}

	std::vector<uint64_t> EntityList()
	{
		std::vector<uint64_t> arrayList;

		for (uint64_t playerId = NULL; playerId < NearEntityTableSize(); ++playerId)
		{
			if (playerId != 0)
			{
				uint64_t targetentity = GetEntity(NearEntityTable(), playerId);

				if (GetEntityTypeName(targetentity) == "dayzplayer")
				{
					arrayList.push_back(targetentity);
				}

				if (GetEntityTypeName(targetentity) == "dayzinfected")
				{
					arrayList.push_back(targetentity);
				}
			}
		}

		for (uint64_t playerId = NULL; playerId < FarEntityTableSize(); ++playerId)
		{
			uint64_t targetentity = GetEntity(FarEntityTable(), playerId);

			if (GetEntityTypeName(targetentity) == "dayzplayer")
			{
				arrayList.push_back(targetentity);
			}
		}

		return arrayList;
	}

}
