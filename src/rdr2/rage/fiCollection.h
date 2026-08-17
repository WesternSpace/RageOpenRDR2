#pragma once

#include "fiDevice.h"

namespace rage
{
	class fiCollection : public fiDevice
	{
	public:
		virtual ~fiCollection() = default;
		virtual bool IsPackfileInitialized() PURECALL
		virtual bool InitializePackfile(const char* filePath, bool useNametable, uint32_t cacheMode, char* headerData, const fiResourceInfo*, bool) PURECALL

		virtual bool MountPackfileAs(const char* mountPoint) PURECALL
		virtual void SetRelativePath(const char* relativePath) PURECALL
		virtual void ClearRelativeOffset() PURECALL
		virtual int GetRelativeOffset() PURECALL

		virtual void ShutdownPackfile() PURECALL
		virtual bool ReInitializePackFile(const char* filePath, bool useNametable, char* headerData, const fiResourceInfo*, bool) PURECALL
		virtual void UnInitializePackFile() PURECALL

		virtual void SetResource() PURECALL
		virtual uint32_t GetFileExtensionPlatform() PURECALL
		virtual HANDLE OpenBulkFromHandle(HANDLE handle, uint64_t& outbias) PURECALL

		virtual bool HasEntries() PURECALL
		virtual uint32_t GetEntryCount() PURECALL
		virtual uint32_t GetEntryPhysicalSortKey(uint32_t a1, bool a2) PURECALL

		virtual uint32_t GetEntryNameHash(HANDLE handle) PURECALL
		virtual uint8_t GetEntryFileExtId(HANDLE handle) PURECALL

		virtual const char* GetEntryName(HANDLE handle, char* dest, uint32_t destSize) PURECALL
		virtual const char* GetEntryFullName(HANDLE handle, char* dest, uint32_t destSize) PURECALL

		virtual uint32_t GetEntryIndexHash(uint32_t hash, uint32_t fileExt) PURECALL
		virtual uint32_t GetEntryIndex(const char* filename, bool unused) PURECALL

		virtual unsigned int GetBasePhysicalSortKey() PURECALL

		virtual bool Prefetch(unsigned int) PURECALL

		virtual bool IsPackfile() PURECALL
		virtual bool IsStreaming() PURECALL
		virtual void SetStreaming(bool) PURECALL

		virtual uint64_t GetHeaderSize() PURECALL

		virtual uint64_t GetPackfileSize() PURECALL

		virtual uint64_t GetPackfileTime() PURECALL
		virtual void SetPackfileTime(uint64_t) PURECALL

		virtual void* GetPackfileHandle() PURECALL
		virtual void SetPackfileHandle(void*) PURECALL

		virtual void* GetEntryResourceInfo(void*, uint16_t) PURECALL
		virtual void PrecacheEntryResourceInfo(void*) PURECALL

		virtual uint64_t GetFileOffset(uint16_t) PURECALL

		virtual uint64_t GetOnDiskSize(uint16_t) PURECALL
		virtual uint64_t GetInMemorySize(uint16_t) PURECALL

		virtual bool IsDirectory(uint16_t) PURECALL

		virtual uint8_t GetCompressorId(uint16_t) PURECALL

		virtual bool IsEncrypted(uint16_t) PURECALL

		virtual bool IsResource(uint16_t) PURECALL

		virtual bool IsSignatureProtected(uint16_t) PURECALL

		virtual void* GetDatResourceInfo(void* data, uint16_t) PURECALL

		virtual uint8_t GetEncryptionKeyId(uint16_t) PURECALL
		virtual uint8_t GetEncryptionConfig(uint16_t) PURECALL

		virtual void* WriteToCache(void*) PURECALL

		virtual void Func_113(void*) PURECALL

		virtual const char* GetPackfileName() PURECALL
	};
}