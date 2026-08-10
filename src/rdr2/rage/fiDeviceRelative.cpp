#include "main.h"

rage::fiDeviceRelative::fiDeviceRelative()
{
	static uintptr_t relativeDeviceVMT = memory::scan("48 8d 05 ? ? ? ? 48 89 5e ? 48 89 06 eb ? 48 8b f3").add(3).rip().as<uintptr_t>();

	// This sets the VMT of this class
	*(uintptr_t*)this = relativeDeviceVMT;
}

void rage::fiDeviceRelative::Init(const char* path, bool allowRoot, rage::fiDevice* parent)
{
	InitRelativeDeviceHook(this, path, allowRoot, parent);
}

bool rage::fiDeviceRelative::MountAs(const char* mountPoint)
{
	return MountRelativeDeviceHook(this, mountPoint);
}