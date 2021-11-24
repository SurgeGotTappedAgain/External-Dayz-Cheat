#pragma once
#include "globals.h"
#include "Game.h"

ImFont* m_pFont;
LPDIRECT3DDEVICE9 d3ddev;
ID3DXLine* dx_Line;
ID3DXFont* dx_Font;
IDirect3DDevice9Ex* p_Device;

std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
void DrawStrokeText(int x, int y, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);

	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), utf_8_2.c_str());
}

namespace cheat
{
	void __fastcall Cache()
	{
		for (uint64_t Entity : Game::EntityList())
		{
			Vector3 worldPosition = Game::GetCoordinate(Entity);
			Vector3 screenPosition = Game::WorldToScreen(worldPosition);
			if (!Entity) continue;

			int distance = Game::GetDistanceToMe(worldPosition);

			if (distance < 2000.0f)
			{
				if (Game::GetEntityTypeName(Entity) == "dayzplayer")
				{
					ImGui::GetOverlayDrawList()->AddText({ screenPosition.x, screenPosition.y }, ImColor(0.f, 1.f, 0.f), "Player");
					DrawStrokeText(screenPosition.x, screenPosition.y + 10, ("[" + std::to_string(distance) + "m]").c_str());
				}
			}
			if (distance < 150.0f)
			{
				if (Game::GetEntityTypeName(Entity) == "dayzinfected")
				{
					ImGui::GetOverlayDrawList()->AddText({ screenPosition.x, screenPosition.y }, ImColor(1.f, 0.f, 0.f), "Zombie");
					DrawStrokeText(screenPosition.x, screenPosition.y + 10, ("[" + std::to_string(distance) + "m]").c_str());
				}
			}
		}
	}
}
			


