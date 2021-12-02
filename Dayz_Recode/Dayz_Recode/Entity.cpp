#include "DirectX.h"
#include "SDK.h"
#include "Common.h"

std::vector<player_t> entities = {};
std::vector<item_t> items = {};

// this is just some driver stuff
void Addy()
{
	if (driver::openMemoryHandle())
	{
		driver::getProcessID("DayZ_x64.exe");
		Base = driver::getModuleBase("DayZ_x64.exe");

		// setting as global var so we can call it easy 
		globals.World = driver::Read<uint64_t>(Base + 0x40899A0);
		if (!globals.World) return;

		// run item and player functions
		std::thread(UpdateList).detach();
		std::thread(UpdateItems).detach();
	}
}

// this will loop and store ents in a vecotor/struct to save data
void UpdateList()
{
	while (true)
	{
		// stores all data
		std::vector<player_t> tmp{};

		uint64_t NearTableSize = driver::Read<uint32_t>(globals.World + 0xEA8 + 0x08);
		uint64_t FarTableSize = driver::Read<uint32_t>(globals.World + 0xFF0 + 0x08);

		for (int i = 0; i < NearTableSize; i++)
		{
			uint64_t EntityTable = driver::Read<uint64_t>(globals.World + 0xEA8);
			if (!EntityTable) continue;

			uint64_t Entity = driver::Read<uint64_t>(EntityTable + (i * 0x8));
			if (!Entity) continue;

			// checking if player even networked
			uintptr_t networkId = driver::Read<uintptr_t>(Entity + 0x634);
			if (networkId == 0) continue;

			player_t Player{};
			Player.NetworkID = networkId;
			Player.TableEntry = EntityTable;
			Player.EntityPtr = Entity;

			// adds info to the vector
			tmp.push_back(Player);
		}

		for (int i = 0; i < FarTableSize; i++)
		{
			uint64_t EntityTable = driver::Read<uint64_t>(globals.World + 0xFF0);
			if (!EntityTable) continue;

			uint64_t Entity = driver::Read<uint64_t>(EntityTable + (i * 0x8));
			if (!Entity) continue;

			uintptr_t networkId = driver::Read<uintptr_t>(Entity + 0x634);
			if (networkId == 0) continue;

			player_t Player{};
			Player.NetworkID = networkId;
			Player.TableEntry = EntityTable;
			Player.EntityPtr = Entity;
			tmp.push_back(Player);
		}
		entities = tmp;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void UpdateItems()
{
	while (true)
	{
		std::vector<item_t> tmp{};

		uint64_t ItemTableSize = driver::Read<uint32_t>(globals.World + 0x1FA8 + 0x08);
		for (int i = 0; i < ItemTableSize; i++)
		{
			uint64_t ItemTable = driver::Read<uint64_t>(globals.World + 0x1FA8);
			if (!ItemTable) continue;

			int Check = driver::Read<int>(ItemTable + (i * 0x18));
			if (Check != 1) continue;

			uint64_t ItemEntity = driver::Read<uint64_t>(ItemTable + ((i * 0x18) + 0x8));

			item_t Item{};
			Item.ItemTable = ItemTable;
			Item.ItemPtr = ItemEntity;
			tmp.push_back(Item);
		}
		items = tmp;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


void Items()
{
	for (int i = 0; i < items.size(); i++)
	{
		auto ItemEntities = items[i];

		Vector3 worldPosition = Game::GetCoordinate(ItemEntities.ItemPtr);
		Vector3 screenPosition = Game::WorldToScreen(worldPosition);
		if (screenPosition.z < 1.0f) continue;
		int distance = Game::GetDistanceToMe(worldPosition);

		if (screenPosition.z >= 1.0f)
		{
			if (distance <= 250)
			{
				DrawString("Item", screenPosition.x, screenPosition.y, 255, 255, 255, pFontSmall);
				DrawShadowString(("[" + std::to_string(distance) + "m]").c_str(), screenPosition.x, screenPosition.y + 15, 123, 20, 196, pFontSmall);
			}
		}

	}
}

// This is called from our overlay
void Hack()
{
	for (int i = 0; i < entities.size(); i++)
	{
		auto Entities = entities[i];

		Vector3 worldPosition = Game::GetCoordinate(Entities.EntityPtr);
		Vector3 screenPosition = Game::WorldToScreen(worldPosition);

		if (screenPosition.z < 1.0f) continue;

		int distance = Game::GetDistanceToMe(worldPosition);

		if (screenPosition.z >= 1.0f)
		{
			if (Game::GetEntityTypeName(Entities.EntityPtr) == "dayzplayer")
			{
				DrawString("Player", screenPosition.x, screenPosition.y, 255, 255, 255, pFontSmall);
				DrawShadowString(("[" + std::to_string(distance) + "m]").c_str(), screenPosition.x, screenPosition.y + 15, 168, 65, 5, pFontSmall);
			}
		}
	}
}