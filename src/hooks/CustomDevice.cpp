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

static rage::fiDeviceRelative* commonDevice;
static rage::fiDeviceRelative* commonDeviceCrc;

static rage::fiDeviceRelative* platformDevice;
static rage::fiDeviceRelative* platformDeviceCrc;

static rage::fiDeviceRelative* updateDevice;
static rage::fiDeviceRelative* updateCommonDevice;
static rage::fiDeviceRelative* updateCommonDeviceCrc;
static rage::fiDeviceRelative* updatePlatformDevice;

static rage::fiDeviceRelative* audioDevice;
static rage::fiDeviceRelative* audioSfxDevice;

static rage::fiDeviceRelative* dlcPackDevice;

static rage::fiDeviceRelative* dlcSetupDevice;

static void MountMods() 
{
	commonDevice->MountAs("common:/");
	commonDeviceCrc->MountAs("commoncrc:/");

	platformDevice->MountAs("platform:/");
	platformDeviceCrc->MountAs("platformcrc:/");

	audioDevice->MountAs("audio:/");
	audioSfxDevice->MountAs("audio:/sfx/");
}

static void UnmountMods() 
{
	rage::fiDevice::Unmount(commonDevice);
	rage::fiDevice::Unmount(commonDeviceCrc);

	rage::fiDevice::Unmount(platformDevice);
	rage::fiDevice::Unmount(platformDeviceCrc);

	rage::fiDevice::Unmount(audioDevice);
	rage::fiDevice::Unmount(audioSfxDevice);
}

static void(*InitialMountOrig)();
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

	commonDevice = new rage::fiDeviceRelative();
	commonDeviceCrc = new rage::fiDeviceRelative();

	platformDevice = new rage::fiDeviceRelative();
	platformDeviceCrc = new rage::fiDeviceRelative();

	updateDevice = new rage::fiDeviceRelative();
	updateCommonDevice = new rage::fiDeviceRelative();
	updateCommonDeviceCrc = new rage::fiDeviceRelative();
	updatePlatformDevice = new rage::fiDeviceRelative();

	audioDevice = new rage::fiDeviceRelative();
	audioSfxDevice = new rage::fiDeviceRelative();

	dlcPackDevice = new rage::fiDeviceRelative();


	commonDevice->Init("mods:/common", true, rootDevice);
	commonDeviceCrc->Init("mods:/common", true, rootDevice);

	platformDevice->Init("mods:/x64", true, rootDevice);
	platformDeviceCrc->Init("mods:/x64", true, rootDevice);

	updateDevice->Init("mods:/update", true, rootDevice);
	updateCommonDevice->Init("mods:/common", true, rootDevice);
	updateCommonDeviceCrc->Init("mods:/common", true, rootDevice);
	updatePlatformDevice->Init("mods:/x64", true, rootDevice);

	audioDevice->Init("mods:/x64/audio", true, rootDevice);
	audioSfxDevice->Init("mods:/x64/audio/sfx", true, rootDevice);

	dlcPackDevice->Init("mods:/x64/dlcpacks", true, rootDevice);

	// These devices are not affected by the remount
	updateDevice->MountAs("update:/");
	updateCommonDevice->MountAs("update_common:/");
	updateCommonDeviceCrc->MountAs("update_commoncrc:/");
	updatePlatformDevice->MountAs("update_platfrom:/");

	dlcPackDevice->MountAs("dlcpacks:/");

	MountMods();

	logger::write("device", "[%s] Mounted mod devices.", __FUNCTION__);
}

// The game remounts the update packfiles, which unintentionally disables some of our modified files from loading, 
// so we remount our mod devices at the same time to avoid this issue.
static bool(*RemountUpdateOrig)();
static bool RemountUpdateHook()
{
	UnmountMods();
	bool result = RemountUpdateOrig();
	MountMods();
	return result;
}

static uint32_t(*MountDlcContentOrig)(CMountableContent*, const char*);
static void(*UnmountDlcContentOrig)(CMountableContent*, const char*);

static const char* GetDlcName(const char* path, bool isPatchDlc = false) {
	static char buffer[128];

	const char* firstSlash = std::strchr(path, '/');

	if (!firstSlash)
		return nullptr;

	const char* nameStart = firstSlash + 1;
	const char* secondSlash = static_cast<const char*>(std::memchr(nameStart, '/', std::strlen(nameStart)));
	
	if (!secondSlash)
		return nullptr;

	if (isPatchDlc)
	{
		nameStart = secondSlash + 1;
		secondSlash = (nameStart + std::strlen(nameStart));
	}

	size_t len = static_cast<size_t>(secondSlash - nameStart);

	if (len >= sizeof(buffer))
		return nullptr; // too long, would truncate

	std::memcpy(buffer, nameStart, len);
	buffer[len] = '\0';

	return buffer;
}

static void MountModDlcSetupFile(const char* setupFilePath)
{
	if (dlcSetupDevice == nullptr)
		dlcSetupDevice = new rage::fiDeviceRelative();

	char namebuffer[64]{};

	// Handle special dlc devices
	if (std::strstr(setupFilePath, "update:/") != NULL)
		dlcSetupDevice->Init("mods:/update", true, dlcPackDevice);
	else if (std::strstr(setupFilePath, "rtp:/") != NULL)
		dlcSetupDevice->Init("dlcpacks:/rtp", true, dlcPackDevice);
	else if (std::strstr(setupFilePath, "/packs/base") != NULL)
		dlcSetupDevice->Init("dlcpacks:/base", true, dlcPackDevice);
	else if (std::strstr(setupFilePath, "ROWAssetPack") != NULL)
		dlcSetupDevice->Init("dlcpacks:/rowassetpack", true, dlcPackDevice);
	else if (std::strstr(setupFilePath, "RORAssetPack") != NULL)
		dlcSetupDevice->Init("dlcpacks:/rowassetpack", true, dlcPackDevice);
	else if (std::strstr(setupFilePath, "dlc_content_extra") != NULL)
		dlcSetupDevice->Init("dlcpacks:/dlc_content_extra", true, dlcPackDevice);
	else // Most common case
	{
		char namebuffer[64]{};
		sprintf_s(namebuffer, "dlcpacks:/%s", GetDlcName(setupFilePath, std::strstr(setupFilePath, "/pack_patch") != NULL));
		dlcSetupDevice->Init(namebuffer, true, dlcPackDevice);
	}

	dlcSetupDevice->MountAs("extra:/");
}

static uint32_t MountDlcSetupFileHook(CMountableContent* self, const char* deviceName)
{
	uint32_t result = MountDlcContentOrig(self, deviceName);

	// Mounting failed
	if (result != 0)
		return result;

	MountModDlcSetupFile(self->m_filename);

	return result;
}

static void UnmountDlcSetupFileHook(CMountableContent* self, const char* deviceName)
{
	if (dlcSetupDevice != nullptr)
		rage::fiDevice::Unmount(dlcSetupDevice);

	UnmountDlcContentOrig(self, deviceName);
}

static bool MountDlcSetupPatchFileHook(rage::fiDeviceRelative* self, const char* mountPoint)
{
	bool result = self->MountAs(mountPoint);

	// Mounting failed
	if (!result)
		return false;

	MountModDlcSetupFile(self->GetDeviceName());

	return true;
}

static bool UnmountDlcSetupPatchFileHook(const char* deviceName)
{
	if (dlcSetupDevice != nullptr)
		rage::fiDevice::Unmount(dlcSetupDevice);

	return rage::fiDevice::UnmountByName(deviceName);
}

static memory::InitFuncs CustomDevice([] {

	memory::scan("0f b7 05 ? ? ? ? 48 03 c3 44 88 34 38 66 01 1d").add(21).hook_call(InitialMountHook, &InitialMountOrig);

	memory::scan("E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 88 98 00 01 00 00").hook_call(RemountUpdateHook, &RemountUpdateOrig, true);

	memory::scan("E8 ?? ?? ?? ?? 85 C0 75 6C 33 D2").hook_call(MountDlcSetupFileHook, &MountDlcContentOrig, true);

	memory::scan("E8 ?? ?? ?? ?? 8B 8B F0 01 00 00 83 F9 01").hook_call(UnmountDlcSetupFileHook, &UnmountDlcContentOrig);

	memory::scan("E8 ?? ?? ?? ?? 84 C0 74 67 33 D2").set_call(MountDlcSetupPatchFileHook);

	memory::scan("E8 ?? ?? ?? ?? 4C 8D 9C 24 40 02 00 00 49 8B 5B 18 49 8B 73 20 49 8B 7B 28 49 8B E3").set_call(UnmountDlcSetupPatchFileHook);
});