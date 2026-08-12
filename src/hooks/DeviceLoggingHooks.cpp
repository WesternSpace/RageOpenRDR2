#include "main.h"

inline void(*InitRelativeDevice)(void*, const char*, bool, void*);
inline bool(*MountRelativeDevice)(void*, const char*);
inline bool(*OpenPackfile)(void*, const char*, bool, uint32_t, char*, uint32_t*, bool);

void InitRelativeDeviceHook(void* self, const char* path, bool allowRoot, void* parent)
{
	logger::write("device", "[rage::fiDeviceRelative::Init] %s", path);
	InitRelativeDevice(self, path, allowRoot, parent);
}

bool MountRelativeDeviceHook(void* self, const char* mountPoint)
{
	logger::write("device", "[rage::fiDeviceRelative::MountAs] %s (%s)", mountPoint, reinterpret_cast<rage::fiDeviceRelative*>(self)->GetDeviceName());

	bool result = MountRelativeDevice(self, mountPoint);

	if (!result)
		logger::write("device", "[rage::fiDeviceRelative::MountAs] Failed to mount as %s", mountPoint);

	return result;
}

static bool OpenPackfileHook(void* self, const char* fileName, bool unused, uint32_t cacheMode, char* packFileData, uint32_t* a6, bool a7)
{
	logger::write("device", "[rage::fiPackfile::Init] %s", fileName);

	bool result = OpenPackfile(self, fileName, unused, cacheMode, packFileData, a6, a7);

	if (!result)
		logger::write("device", "[rage::fiPackfile::Init] Failed to open %s", fileName);

	return result;
}

static memory::InitFuncs DeviceLoggingHooks([] {
	memory::scan("48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 57 48 81 ec ? ? ? ? 49 8b f1 41 8a e8 48 8b da 48 8b f9 e8")
		.hook(InitRelativeDeviceHook, &InitRelativeDevice);

	memory::scan("48 89 5c 24 ? 57 48 81 ec ? ? ? ? 44 8a 81 ? ? ? ? 48 8b da 48 8b d1 48 8b f9 48 8b cb")
		.hook(MountRelativeDeviceHook, &MountRelativeDevice);

	memory::scan("48 89 5C 24 18 48 89 54 24 10 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 B0")
		.hook(OpenPackfileHook, &OpenPackfile);

	rage::fiDeviceCustom* logger = new rage::fiDeviceCustom();

	// Hook fiDeviceLocal VTable
	//memory::scan("48 8D 0D ?? ?? ?? ?? 45 33 C0 C7 44 24 20 00 20 00 00").add(3).rip().hook_vft(logger, 67); // This is currently disabled, as hooking this will lead to a game crash later.

	// Hook fiDeviceRelative VTable
	memory::scan("48 8D 05 ? ? ? ? 48 89 03 EB 02 33 DB E8 ? ? ? ? 4C 8B C8 48 8D 15").add(3).rip().hook_vft(logger, 67);

	// Hook fiPackfile VTable
	memory::scan("33 C9 48 8D 05 ? ? ? ? 48 89 03 48 83 C8 FF 48 89 43 18").add(5).rip().hook_vft(logger, 115);

	// We can unllocate the device as we only just needed it to get the VTable.
	delete logger;
});