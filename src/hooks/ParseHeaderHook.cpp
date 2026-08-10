#include "main.h"

bool(*ParseHeaderOrig)(void*, const char*, bool, void*, rage::fiResourceInfo*, bool);
bool ParseHeaderHook(void* a1, const char* name, bool readHeader, void* customHeader, rage::fiResourceInfo* a5, bool a6)
{
	logger::log("mods", "[%s] parsing header /%s", __FUNCTION__, name);
	bool ret = ParseHeaderOrig(a1, name, readHeader, customHeader, a5, a6);
	return ret;
}

static memory::InitFuncs PackfileEncryptionHooks([] {
	auto mem = memory::scan("48 8b ce 88 44 24 ? 4c 89 7c 24 ? e8").add(12);
	ParseHeaderOrig = mem.add(1).rip().as<decltype(ParseHeaderOrig)>();
	mem.set_call(ParseHeaderHook);
});