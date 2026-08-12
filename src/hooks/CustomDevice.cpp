#include "main.h"

/*
	List of stuff you can replace with this: https://gist.github.com/martonp96/59f731446c7f17db3f400c2be458c4a4

	How to do it:
	Find something from the list, eg. common:/data/dlclist.xml

	Copy the original dlclist.xml from the files and place it in [your GTA5 folder]/newmods/common/data/dlclist.xml
	Edit the file, add a new entry like <Item>dlcpacks:/mrpd/</Item>
	Place the single player dlc.rpf like [your GTA5 folder]/newmods/dlcpacks/mrpd/dlc.rpf

	Start the game and the modded stuff should be loaded automatically.

	I used this for the example: https://hu.gta5-mods.com/maps/community-mission-row-pd
*/

rage::fiDeviceRelative* dlcPackDevice;

rage::fiDeviceRelative* commonDevice;
rage::fiDeviceRelative* commonDeviceCRC;
rage::fiDeviceRelative* platformDevice;
rage::fiDeviceRelative* platformDeviceCRC;

rage::fiDeviceRelative* audioDevice;
rage::fiDeviceRelative* audioSfxDevice;

rage::fiDeviceRelative* updateDevice;

bool IsCustomDevice(rage::fiDevice* dev)
{
	if (dev == platformDevice || dev == platformDeviceCRC || dev == commonDevice || dev == commonDeviceCRC)
		return true;
	return false;
}

static void MountMods() {
	dlcPackDevice->MountAs("dlcpacks:/");
	commonDevice->MountAs("common:/");
	commonDeviceCRC->MountAs("commoncrc:/");
	platformDevice->MountAs("platform:/");
	platformDeviceCRC->MountAs("platformcrc:/");
	audioDevice->MountAs("audio:/");
	audioSfxDevice->MountAs("audio:/sfx/");
	updateDevice->MountAs("update:/");
}

static void UnmountMods() {
	rage::fiDevice::Unmount(dlcPackDevice);
	rage::fiDevice::Unmount(commonDevice);
	rage::fiDevice::Unmount(commonDeviceCRC);
	rage::fiDevice::Unmount(platformDevice);
	rage::fiDevice::Unmount(platformDeviceCRC);
	rage::fiDevice::Unmount(audioDevice);
	rage::fiDevice::Unmount(audioSfxDevice);
	rage::fiDevice::Unmount(updateDevice);
}

void(*InitialMountOrig)();
static void InitialMountHook()
{
	InitialMountOrig();

	std::filesystem::path cwd = std::filesystem::current_path() / "newmods/";
	cwd.make_preferred();

	logger::write("device", "[%s] Mods path: %s", __FUNCTION__, cwd.string().c_str());

	rage::fiDeviceRelative* rootDevice = new rage::fiDeviceRelative();
	rootDevice->Init(cwd.string().c_str(), true, nullptr);

	if (rootDevice->MountAs("mods:/"))
		logger::write("device", "[%s] Root device mounted!", __FUNCTION__);

	dlcPackDevice = new rage::fiDeviceRelative();

	commonDevice = new rage::fiDeviceRelative();
	commonDeviceCRC = new rage::fiDeviceRelative();

	platformDevice = new rage::fiDeviceRelative();
	platformDeviceCRC = new rage::fiDeviceRelative();

	audioDevice = new rage::fiDeviceRelative();
	audioSfxDevice = new rage::fiDeviceRelative();

	updateDevice = new rage::fiDeviceRelative();

	dlcPackDevice->Init("mods:/x64/dlcpacks", true, rootDevice);
	commonDevice->Init("mods:/common", true, rootDevice);
	commonDeviceCRC->Init("mods:/common", true, rootDevice);
	platformDevice->Init("mods:/x64", true, rootDevice);
	platformDeviceCRC->Init("mods:/x64", true, rootDevice);
	audioDevice->Init("mods:/x64/audio", true, rootDevice);
	audioSfxDevice->Init("mods:/x64/audio/sfx", true, rootDevice);
	updateDevice->Init("mods:/update", true, rootDevice);

	MountMods();

	logger::write("device", "[%s] Mounted mod devices.", __FUNCTION__);
}

// The game remounts the update packfiles, which unintentionally disables our modified files from loading, 
// so we remount our mod devices at the same time to avoid this issue.
bool(*RemountUpdateOrig)();
static bool RemountUpdateHook()
{
	UnmountMods();
	bool result = RemountUpdateOrig();
	MountMods();
	return result;
}

static memory::InitFuncs CustomDevice([] {

		auto mem = memory::scan("0f b7 05 ? ? ? ? 48 03 c3 44 88 34 38 66 01 1d").add(21);
		InitialMountOrig = mem.add(1).rip().as<decltype(InitialMountOrig)>();
		mem.set_call(InitialMountHook);

		auto mem2 = memory::scan("E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 88 98 00 01 00 00");
		RemountUpdateOrig = mem2.add(1).rip().as<decltype(RemountUpdateOrig)>();
		mem2.set_call(RemountUpdateHook, true);
});