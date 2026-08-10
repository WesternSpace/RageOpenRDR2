#pragma once

void InitRelativeDeviceHook(void* self, const char* path, bool allowRoot, void* parent);
bool MountRelativeDeviceHook(void* self, const char* mountPoint);