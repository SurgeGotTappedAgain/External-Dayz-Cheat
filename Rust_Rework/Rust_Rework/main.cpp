#include "Driver.h"
#include "globals.h"
#include "Vectors.h"
#include "Overlay.h"
#include "Visuals.h"

HWND hwnd = nullptr;

// Include Your Own driver (Read/Write Memory)

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

	 setup_window();
	 setup_directx(MyWnd);

	 while (true)
	 {
		main_loop();
	 }

	 std::thread Visuals(cheat::Cache);

	std::cin.get();

}
