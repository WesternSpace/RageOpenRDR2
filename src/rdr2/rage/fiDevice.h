#pragma once

#include <Windows.h>
#include <cstdint>
#include <stdexcept>
#include <string>

// We cannot define the functions as pure virtual functions
// else we can't interact with the functions as we want.
#define PURECALL { throw std::logic_error(std::string("Pure virtual call reached in ") + __FUNCTION__); }

namespace rage
{
	// Same as in V
	struct fiFindData
	{
		char fileName[256];
		uint64_t fileSize;
		FILETIME lastWriteTime;
		DWORD fileAttributes;
	};

	class fiResourceInfo;

	class fiDevice
	{
	public:
		virtual ~fiDevice() = default;

		static bool Unmount(fiDevice* device);
		static bool UnmountByName(const char* deviceName);

		virtual HANDLE OpenFileWithAttribute(const char* fileName, bool readOnly, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) PURECALL
		virtual HANDLE OpenFileHash(uint32_t* fileNameHash) PURECALL
		virtual HANDLE OpenFileHashExt(uint32_t* fileNameHash, uint32_t fileExt) PURECALL
		virtual HANDLE OpenFile(const char* fileName, bool readOnly) PURECALL
		virtual HANDLE OpenFileBulk(const char* fileName, uint64_t& outbias) PURECALL
		virtual HANDLE OpenFileBulkOverlapped(const char* fileName) PURECALL

		virtual HANDLE MakeFileBulk(const char* fileName) PURECALL
		virtual HANDLE MakeFileWithAttribute(const char* fileName, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) PURECALL
		virtual HANDLE MakeFile(const char* fileName) PURECALL

		virtual uint32_t ReadFromHandle(HANDLE handle, void* buffer, uint32_t bufferSize) PURECALL
		virtual uint32_t ReadFromFile(const char* filename, void* buffer, uint32_t bufferSize) PURECALL
		virtual uint32_t ReadFromHandleBulk(HANDLE handle, uint64_t offset, void* buffer, uint32_t bufferSize) PURECALL
		virtual uint32_t ReadFromHandleBulkOverlapped(HANDLE handle, uint64_t offset, char* buffer, uint32_t bufferSize) PURECALL

		virtual uint32_t WriteBulk(HANDLE handle, uint64_t offset, const void* inBuffer, uint32_t bufferSize) PURECALL
		virtual uint32_t Write(HANDLE handle, const void* buffer, uint32_t bufferSize) PURECALL

		virtual uint32_t Seek(HANDLE handle, uint32_t offset, uint32_t method) PURECALL
		virtual uint64_t Seek64(HANDLE handle, int64_t distance, uint32_t method) PURECALL

		virtual uint32_t CloseFileHandle(HANDLE handle) PURECALL
		virtual uint32_t CloseFileHandleBulk(HANDLE handle) PURECALL 
		virtual uint32_t CloseFileHandleBulkOverlapped(HANDLE handle) PURECALL

		virtual uint32_t GetSize(HANDLE handle) PURECALL
		virtual uint64_t GetSize64(HANDLE handle) PURECALL

		virtual uint32_t Flush(HANDLE handle) PURECALL

		virtual bool RemoveFile(const char* fileName) PURECALL

		virtual bool RenameFile(const char* from, const char* to) PURECALL
		virtual bool RenameFileWithProgress(const char* from, const char* to) PURECALL

		virtual bool MakeDirectory(const char* dir) PURECALL
		virtual bool UnmakeDirectory(const char* dir) PURECALL

		virtual uint64_t GetAvailableDiskSpace(const char* dir) PURECALL

		virtual void Sanitize(HANDLE handle) PURECALL

		virtual uint64_t GetFileSizes(void* files, void* sizes) PURECALL
		virtual uint64_t GetFileSize(const char* fileName) PURECALL
		
		virtual uint64_t GetFileTimes(void* files, void* times) PURECALL
		virtual uint64_t GetFileTime(const char* fileName) PURECALL
		virtual bool SetFileTime(const char* fileName, uint64_t timestamp) PURECALL

		virtual HANDLE BeginFileSearch(const char* directoryName, const char* wildcard, rage::fiFindData& findData) PURECALL
		virtual HANDLE BeginFileSearch(const char* directoryName, rage::fiFindData& findData) PURECALL
		virtual bool SearchNextFile(HANDLE handle, rage::fiFindData& outData) PURECALL
		virtual uint32_t EndFileSearch(HANDLE handle) PURECALL

		virtual const rage::fiDevice* GetLowLevelDevice() PURECALL

		virtual char* FixRelativeName(char* dest, uint32_t destSize, const char* src) PURECALL

		virtual bool SetEndOfFile(HANDLE handle) PURECALL

		virtual uint32_t GetAttributes(const char* fileName) PURECALL
		virtual bool SetAttributes(const char* fileName, uint32_t attributes) PURECALL

		virtual uint32_t GetRootDeviceId(const char*) PURECALL

		virtual bool IsMemoryMappedDevice() PURECALL

		virtual bool ReadFileSafe(HANDLE handle, void* outBuffer, uint32_t size) PURECALL
		virtual bool WriteFileSafe(HANDLE handle, const void* buffer, uint32_t size) PURECALL

		virtual uint32_t GetResourceInfos(void* resources, fiResourceInfo** flags) PURECALL
		virtual uint32_t GetResourceInfo(const char* name, fiResourceInfo* outHeader) PURECALL

		virtual uint32_t GetEncryptionKey() PURECALL

		virtual bool IsValidFileHandle(HANDLE handle) PURECALL

		virtual uint64_t GetBulkFileOffset(HANDLE handle) PURECALL

		virtual uint32_t GetPhysicalSortKey(const char*) PURECALL

		virtual bool IsRpfArchive() PURECALL
		virtual uint64_t GetRpfArchiveVersion() PURECALL
		virtual const fiDevice* GetRpfArchiveDevice() PURECALL

		virtual bool IsCloudDevice() PURECALL

		virtual uint32_t GetPackfileIndex() PURECALL

		virtual const char* GetDeviceName() PURECALL

		virtual bool SupportsOverlappedIO() PURECALL

		virtual uint32_t GetClampedBufferSize(uint64_t, uint32_t) PURECALL

		virtual uint64_t GetFinalOffset(uint64_t) PURECALL

		virtual bool IsOverlappedRequestFinished(uint64_t, uint32_t) PURECALL

		virtual HANDLE CreateMapping(HANDLE fileHandle, bool writeAccess, uint64_t offset, size_t viewSize, void** outMappedView) PURECALL
		virtual uint32_t CloseMapping(HANDLE mappingHandle, void* mappedView) PURECALL
	};
}