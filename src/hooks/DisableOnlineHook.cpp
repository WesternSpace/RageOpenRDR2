#include "utils/log.h"
#include "utils/memory.h"

static bool CheckNetworkAccessHook(uint32_t accessArea, uint32_t* accessCode, time_t* endPosixTime)
{
	logger::write("info", "[%s] Blocked access to online area: 0X%x", __FUNCTION__, accessArea);
	*accessCode = 0x4;
	*endPosixTime = 0xFFFFFFFFFFFFFFFF;
	return false;
}

static memory::InitFuncs PackfileEncryptionHooks([] {
	memory::scan("48 89 5C 24 10 48 89 6C 24 18 56 57 41 54 41 55 41 57 48 83 EC 30 4D").hook(CheckNetworkAccessHook); 
});