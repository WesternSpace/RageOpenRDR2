#pragma once

#include "rdr2/rage/fiDevice.h"
#include "rdr2/rage/fiDeviceRelative.h"
#include "rdr2/rage/string.h"

class CMountableContent 
{
public:
	char gap0[112];
	rage::fiDevice* m_pDevice;
	rage::fiDevice* m_pCrcDevice;
	rage::fiDevice* m_pCrcDeviceTU;
	rage::fiDeviceRelative* m_pDeviceTU;
	char gap90[176];
	char m_subPacks[16];
	char m_setupData[128];
	char gap1D0[8];
	rage::atString m_filename;
};