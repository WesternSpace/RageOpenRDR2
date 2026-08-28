#include "main.h"
#include "utils/config.h"
#include "utils/log.h"
#include "utils/memory.h"

#include <cstdio>

static bool bInited = false;
void (*origGetSystemTimeAsFileTime)(LPFILETIME lpSystemTimeAsFileTime);
void HookGetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
	if (!bInited)
	{
		bInited = true;

		// Don't hide the console
		auto addr = memory::scan("ff 15 ? ? ? ? 33 c9 ff 15 ? ? ? ? 45 33 c9");
		if (addr.address != 0) 
		{
			addr.nop(6);
		}

		// cache skip
		addr = memory::scan("74 ? e8 ? ? ? ? 48 8b c8 48 8d 15 ? ? ? ? e8 ? ? ? ? 66 83 7b");
		if (addr.address != 0) 
		{
			addr.put<uint8_t>(0xEB);
		}

		memory::InitFuncs::run();

		logger::write("info", " RageOpenRDR2 Inited!");
	}
	origGetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		config::load();

		logger::init();

		memory::init();

		if (!memory::HookApi(L"kernel32.dll", "GetSystemTimeAsFileTime", (PVOID)HookGetSystemTimeAsFileTime, (PVOID*)&origGetSystemTimeAsFileTime)) {
			logger::write("info", "Hooking failed error (%ld)", GetLastError());
		}
	}
	return TRUE;
}