#pragma once

namespace rage
{
	class fiDeviceRelative : public fiDevice
	{
	public:
		fiDeviceRelative();
		virtual ~fiDeviceRelative() = default;

		void Init(const char* path, bool allowRoot, rage::fiDevice* parent);
		bool MountAs(const char* mountPoint);

		// Virtual function definitions is the same as rage::fiDevice.

	protected:
		const rage::fiDevice* m_parentDevice = nullptr;
		char m_parentDevicePath[256] = {};
		uint32_t m_parentDevicePathSize = 0;
		bool m_readOnly = false;
		char m_pad[0x3] = {};
	};
}