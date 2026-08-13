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
rage::fiDeviceRelative* dlcSetupDevice;

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

uint32_t(*MountDlcContentOrig)(CMountableContent*, const char*);
void(*UnmountDlcContentOrig)(CMountableContent*, const char*);

static const char* GetDlcName(const char* path) {
	static char buffer[128];

	const char* firstSlash = std::strchr(path, '/');

	if (!firstSlash)
		return nullptr;

	const char* nameStart = firstSlash + 1;
	const char* secondSlash = static_cast<const char*>(std::memchr(nameStart, '/', std::strlen(nameStart)));

	if (!secondSlash)
		return nullptr;

	size_t len = static_cast<size_t>(secondSlash - nameStart);

	if (len >= sizeof(buffer))
		return nullptr; // too long, would truncate

	std::memcpy(buffer, nameStart, len);
	buffer[len] = '\0';

	return buffer;
}

static uint32_t MountDlcSetupFileHook(CMountableContent* self, const char* deviceName)
{
	uint32_t result = MountDlcContentOrig(self, deviceName);

	// Mounting failed
	if (result != 0)
		return result;

	if (dlcSetupDevice == nullptr)
		dlcSetupDevice = new rage::fiDeviceRelative();

	char namebuffer[64]{};

	// Handle special dlc devices
	if (std::strstr(self->m_filename, "update:/") != NULL)
		dlcSetupDevice->Init("mods:/update", true, dlcPackDevice);
	else if (std::strstr(self->m_filename, "rtp:/") != NULL)
		dlcSetupDevice->Init("dlcpacks:/rtp", true, dlcPackDevice);
	else if (std::strstr(self->m_filename, "/packs/base/") != NULL)
		dlcSetupDevice->Init("dlcpacks:/base", true, dlcPackDevice);
	else if (std::strstr(self->m_filename, "ROWAssetPack") != NULL)
		dlcSetupDevice->Init("dlcpacks:/rowassetpack", true, dlcPackDevice);
	else if (std::strstr(self->m_filename, "dlc_content_extra") != NULL)
		dlcSetupDevice->Init("dlcpacks:/dlc_content_extra", true, dlcPackDevice);
	else // Most common case
	{
		char namebuffer[64]{};
		sprintf_s(namebuffer, "dlcpacks:/%s", GetDlcName(self->m_filename));
		dlcSetupDevice->Init(namebuffer, true, dlcPackDevice);
	}

	dlcSetupDevice->MountAs("extra:/");

	return result;
}

static void UnmountDlcSetupFileHook(CMountableContent* self, const char* deviceName)
{
	if (dlcSetupDevice != nullptr)
		rage::fiDevice::Unmount(dlcSetupDevice);

	UnmountDlcContentOrig(self, deviceName);
}

static memory::InitFuncs CustomDevice([] {

		auto mem = memory::scan("0f b7 05 ? ? ? ? 48 03 c3 44 88 34 38 66 01 1d").add(21);
		InitialMountOrig = mem.add(1).rip().as<decltype(InitialMountOrig)>();
		mem.set_call(InitialMountHook);

		auto mem2 = memory::scan("E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 88 98 00 01 00 00");
		RemountUpdateOrig = mem2.add(1).rip().as<decltype(RemountUpdateOrig)>();
		mem2.set_call(RemountUpdateHook, true);

		auto mem3 = memory::scan("E8 ?? ?? ?? ?? 85 C0 75 6C 33 D2");
		MountDlcContentOrig = mem3.add(1).rip().as<decltype(MountDlcContentOrig)>();
		mem3.set_call(MountDlcSetupFileHook, true);

		auto mem4 = memory::scan("E8 ?? ?? ?? ?? 8B 8B F0 01 00 00 83 F9 01");
		UnmountDlcContentOrig = mem4.add(1).rip().as<decltype(UnmountDlcContentOrig)>();
		mem4.set_call(UnmountDlcSetupFileHook);
});