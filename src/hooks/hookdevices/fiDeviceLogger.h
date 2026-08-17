#pragma once

#include "rdr2/rage/fiCollection.h"

namespace rage
{
	class fiDeviceCustom : public fiCollection
	{
	public:
		fiDeviceCustom() = default;
		virtual ~fiDeviceCustom() = default;

		virtual HANDLE OpenFileWithAttribute(const char* fileName, bool readOnly, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) override;
		virtual HANDLE OpenFileHash(uint32_t* fileNameHash) override;
		virtual HANDLE OpenFileHashExt(uint32_t* fileNameHash, uint32_t fileExt) override;
		virtual HANDLE OpenFile(const char* fileName, bool readOnly) override;
		virtual HANDLE OpenFileBulk(const char* fileName, uint64_t& outbias) override;
		virtual HANDLE OpenFileBulkOverlapped(const char* fileName) override;

		virtual HANDLE MakeFileBulk(const char* fileName) override;
		virtual HANDLE MakeFileWithAttribute(const char* fileName, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) override;
		virtual HANDLE MakeFile(const char* fileName) override;

		virtual uint32_t ReadFromHandle(HANDLE handle, void* buffer, uint32_t bufferSize) override;
		virtual uint32_t ReadFromFile(const char* filename, void* buffer, uint32_t bufferSize) override;
		virtual uint32_t ReadFromHandleBulk(HANDLE handle, uint64_t offset, void* buffer, uint32_t bufferSize) override;
		virtual uint32_t ReadFromHandleBulkOverlapped(HANDLE handle, uint64_t offset, char* buffer, uint32_t bufferSize) override;

		virtual uint32_t WriteBulk(HANDLE handle, uint64_t offset, const void* inBuffer, uint32_t bufferSize) override;
		virtual uint32_t Write(HANDLE handle, const void* buffer, uint32_t bufferSize) override;

		virtual uint32_t Seek(HANDLE handle, uint32_t offset, uint32_t method) override;
		virtual uint64_t Seek64(HANDLE handle, int64_t distance, uint32_t method) override;

		virtual uint32_t CloseFileHandle(HANDLE handle) override;
		virtual uint32_t CloseFileHandleBulk(HANDLE handle) override;
		virtual uint32_t CloseFileHandleBulkOverlapped(HANDLE handle) override;

		virtual uint32_t GetSize(HANDLE handle) override;
		virtual uint64_t GetSize64(HANDLE handle) override;

		virtual uint32_t Flush(HANDLE handle) override;

		virtual bool RemoveFile(const char* fileName) override;

		virtual bool RenameFile(const char* from, const char* to) override;
		virtual bool RenameFileWithProgress(const char* from, const char* to) override;

		virtual bool MakeDirectory(const char* dir) override;
		virtual bool UnmakeDirectory(const char* dir) override;

		virtual uint64_t GetAvailableDiskSpace(const char* dir) override;

		virtual void Sanitize(HANDLE handle) override;

		virtual uint64_t GetFileSizes(void* files, void* sizes) override;
		virtual uint64_t GetFileSize(const char* fileName) override;

		virtual uint64_t GetFileTimes(void* files, void* times) override;
		virtual uint64_t GetFileTime(const char* fileName) override;
		virtual bool SetFileTime(const char* fileName, uint64_t timestamp) override;

		virtual HANDLE BeginFileSearch(const char* directoryName, const char* wildcard, rage::fiFindData& findData) override;
		virtual HANDLE BeginFileSearch(const char* directoryName, rage::fiFindData& findData) override;
		virtual bool SearchNextFile(HANDLE handle, rage::fiFindData& outData) override;
		virtual uint32_t EndFileSearch(HANDLE handle) override;

		virtual const rage::fiDevice* GetLowLevelDevice() override;

		virtual char* FixRelativeName(char* dest, uint32_t destSize, const char* src) override;

		virtual bool SetEndOfFile(HANDLE handle) override;

		virtual uint32_t GetAttributes(const char* fileName) override;
		virtual bool SetAttributes(const char* fileName, uint32_t attributes) override;

		virtual uint32_t GetRootDeviceId(const char*) override;

		virtual bool IsMemoryMappedDevice() override;

		virtual bool ReadFileSafe(HANDLE handle, void* outBuffer, uint32_t size) override;
		virtual bool WriteFileSafe(HANDLE handle, const void* buffer, uint32_t size) override;

		virtual uint32_t GetResourceInfos(void* resources, fiResourceInfo** flags) override;
		virtual uint32_t GetResourceInfo(const char* name, fiResourceInfo* outHeader) override;

		virtual uint32_t GetEncryptionKey() override;

		virtual bool IsValidFileHandle(HANDLE handle) override;

		virtual uint64_t GetBulkFileOffset(HANDLE handle) override;

		virtual uint32_t GetPhysicalSortKey(const char*) override;

		virtual bool IsRpfArchive() override;
		virtual uint64_t GetRpfArchiveVersion() override;
		virtual const fiDevice* GetRpfArchiveDevice() override;

		virtual bool IsCloudDevice() override;

		virtual uint32_t GetPackfileIndex() override;

		virtual const char* GetDeviceName() override;

		virtual bool SupportsOverlappedIO() override;

		virtual uint32_t GetClampedBufferSize(uint64_t, uint32_t) override;

		virtual uint64_t GetFinalOffset(uint64_t) override;

		virtual bool IsOverlappedRequestFinished(uint64_t, uint32_t) override;

		virtual HANDLE CreateMapping(HANDLE fileHandle, bool writeAccess, uint64_t offset, size_t viewSize, void** outMappedView) override;
		virtual uint32_t CloseMapping(HANDLE mappingHandle, void* mappedView) override;

		// Packfile functions

		virtual bool IsPackfileInitialized() override;
		virtual bool InitializePackfile(const char* filePath, bool useNametable, uint32_t cacheMode, char* headerData, const fiResourceInfo*, bool) override;

		virtual bool MountPackfileAs(const char* mountPoint) override;
		virtual void SetRelativePath(const char* relativePath) override;
		virtual void ClearRelativeOffset() override;
		virtual int GetRelativeOffset() override;

		virtual void ShutdownPackfile() override;
		virtual bool ReInitializePackFile(const char* filePath, bool useNametable, char* headerData, const fiResourceInfo*, bool) override;
		virtual void UnInitializePackFile() override;

		virtual void SetResource() override;
		virtual uint32_t GetFileExtensionPlatform() override;
		virtual HANDLE OpenBulkFromHandle(HANDLE handle, uint64_t& outbias) override;

		virtual bool HasEntries() override;
		virtual uint32_t GetEntryCount() override;
		virtual uint32_t GetEntryPhysicalSortKey(uint32_t a1, bool a2) override;

		virtual uint32_t GetEntryNameHash(HANDLE handle) override;
		virtual uint8_t GetEntryFileExtId(HANDLE handle) override;

		virtual const char* GetEntryName(HANDLE handle, char* dest, uint32_t destSize) override;
		virtual const char* GetEntryFullName(HANDLE handle, char* dest, uint32_t destSize) override;

		virtual uint32_t GetEntryIndexHash(uint32_t hash, uint32_t fileExt) override;
		virtual uint32_t GetEntryIndex(const char* filename, bool unused) override;

		virtual unsigned int GetBasePhysicalSortKey() override;

		virtual bool Prefetch(unsigned int) override;

		virtual bool IsPackfile() override;
		virtual bool IsStreaming() override;
		virtual void SetStreaming(bool) override;

		virtual uint64_t GetHeaderSize() override;

		virtual uint64_t GetPackfileSize() override;

		virtual uint64_t GetPackfileTime() override;
		virtual void SetPackfileTime(uint64_t) override;

		virtual void* GetPackfileHandle() override;
		virtual void SetPackfileHandle(void*) override;

		virtual void* GetEntryResourceInfo(void*, uint16_t) override;
		virtual void PrecacheEntryResourceInfo(void*) override;

		virtual uint64_t GetFileOffset(uint16_t) override;

		virtual uint64_t GetOnDiskSize(uint16_t) override;
		virtual uint64_t GetInMemorySize(uint16_t) override;

		virtual bool IsDirectory(uint16_t) override;

		virtual uint8_t GetCompressorId(uint16_t) override;

		virtual bool IsEncrypted(uint16_t) override;

		virtual bool IsResource(uint16_t) override;

		virtual bool IsSignatureProtected(uint16_t) override;

		virtual void* GetDatResourceInfo(void* data, uint16_t) override;

		virtual uint8_t GetEncryptionKeyId(uint16_t) override;

		virtual uint8_t GetEncryptionConfig(uint16_t) override;

		virtual void* WriteToCache(void*) override;

		virtual void Func_113(void*) override;

		virtual const char* GetPackfileName() override;
	};
}