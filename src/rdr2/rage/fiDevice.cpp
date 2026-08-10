#include "main.h"

static memory::func<bool, rage::fiDevice*> g_Unmount("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 44 0F");

bool rage::fiDevice::Unmount(fiDevice* device)
{
	return g_Unmount(device);
}
