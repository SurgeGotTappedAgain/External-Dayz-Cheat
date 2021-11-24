#include "Driver.h"
#include "globals.h"
#include "Vectors.h"
#include "Overlay.h"
#include "Visuals.h"

HWND hwnd = nullptr;
HANDLE memoryRead = 0, memoryWrite = 0, memoryESPWrite = 0;
HDC hdc;


int main()
{
	if (driver::openMemoryHandle())
	{
		std::cout << "[-] Communication Established" << std::endl;
	}
	else
	{
		std::cout << "[!] Communication Failed" << std::endl;
		std::cin.get();
		return 0;
	}

	while (!hwnd)
	{
		hwnd = FindWindow(NULL, "DayZ");
	}

	std::cout << "[+] Found DayZ" << std::endl;

	driver::getProcessID("DayZ_x64.exe");

	 Base = driver::getModuleBase("DayZ_x64.exe");
	 std::cout << "[BASE] " << "0x" << std::hex << Base << std::endl;

	 globals.World = driver::Read<uint64_t>(Base + 0x40899A0);
	 if (!globals.World) return NULL;

	 std::cout << "[WORLD] " << "0x" << std::hex << globals.World << std::endl;

	// driver::Write<float>(globals.World + 0x28A8, 2.0f);

	 setup_window();
	 setup_directx(MyWnd);

	 while (true)
	 {
		main_loop();
	 }

	 std::thread Visuals(cheat::Cache);

	std::cin.get();

}