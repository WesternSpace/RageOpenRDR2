#include "main.h"

static memory::func<bool, rage::fiDevice*> unmount("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 44 0F");
static memory::func<bool, const char*> unmountByName("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 20 48 8D 15");

bool rage::fiDevice::Unmount(fiDevice* device)
{
	return unmount(device);
}

bool rage::fiDevice::UnmountByName(const char* deviceName)
{
	return unmountByName(deviceName);
}