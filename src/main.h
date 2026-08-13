#pragma once

#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <filesystem>
#include <vector>

#include "utils/log.h"
#include "utils/memory.h"
#include "utils/config.h"

#define PURECALL { throw std::logic_error(std::string("Pure virtual call reached in ") + __FUNCTION__); }

#include "hooks/DeviceLoggingHooks.h"

#include "rdr2/rage/string.h"
#include "rdr2/rage/fiDevice.h"
#include "rdr2/rage/fiCollection.h"
#include "rdr2/rage/fiDeviceRelative.h"

#include "rdr2/game/MountableContent.h"

#include "hooks/hookdevices/fiDeviceLogger.h"