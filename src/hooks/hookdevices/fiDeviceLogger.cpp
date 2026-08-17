#include "fiDeviceLogger.h"
#include "utils/memory.h"
#include "utils/log.h"

#include <cstdio>
#include <string>
#include <unordered_map>


std::unordered_map<HANDLE, std::string> handleNames;

static const char* getHandleName(HANDLE handle)
{
    auto it = handleNames.find(handle);
    return it != handleNames.end() ? it->second.c_str() : "<unknown>";
}

HANDLE rage::fiDeviceCustom::OpenFileWithAttribute(const char* fileName, bool readOnly, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(this, 1, fileName, readOnly, dwShareMode, dwFlagsAndAttributes);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;
    return handle;
}

HANDLE rage::fiDeviceCustom::OpenFileHash(uint32_t* fileNameHash)
{
    logger::write("device", "[%s] 0x%X.bin (%s)", __FUNCTION__, *fileNameHash, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(this, 2, fileNameHash);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    char namebuffer[32]{};
    sprintf_s(namebuffer, "0x%X.bin", *fileNameHash);
    handleNames[handle] = namebuffer;
    return handle;
}

HANDLE rage::fiDeviceCustom::OpenFileHashExt(uint32_t* fileNameHash, uint32_t fileExt)
{
    logger::write("device", "[%s] 0x%X.bin; Ext: %d (%s)", __FUNCTION__, *fileNameHash, fileExt, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(this, 3, fileNameHash, fileExt);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    char namebuffer[32]{};
    sprintf_s(namebuffer, "0x%X.bin; Ext: %d", *fileNameHash, fileExt);
    handleNames[handle] = namebuffer;
    return handle;
}

HANDLE rage::fiDeviceCustom::OpenFile(const char* fileName, bool readOnly)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(this, 4, fileName);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;

    return handle;
}

HANDLE rage::fiDeviceCustom::OpenFileBulk(const char* fileName, uint64_t& outbias)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(this, 5, fileName, &outbias);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;

    return handle;
}

HANDLE rage::fiDeviceCustom::OpenFileBulkOverlapped(const char* fileName)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(
        this, 6, fileName);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;

    return handle;
}

HANDLE rage::fiDeviceCustom::MakeFileBulk(const char* fileName)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(
        this, 7, fileName);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;

    return handle;
}

HANDLE rage::fiDeviceCustom::MakeFileWithAttribute(const char* fileName, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(
        this, 8, fileName, dwShareMode, dwFlagsAndAttributes);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;

    return handle;
}

HANDLE rage::fiDeviceCustom::MakeFile(const char* fileName)
{
    logger::write("device", "[%s] %s %s()", __FUNCTION__, fileName, this->GetDeviceName());

    HANDLE handle = memory::call_original_vft_func<HANDLE>(this, 9, fileName);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    handleNames[handle] = fileName;

    return handle;
}

uint32_t rage::fiDeviceCustom::ReadFromHandle(HANDLE handle, void* buffer, uint32_t bufferSize)
{
    return memory::call_original_vft_func<uint32_t>(this, 10, handle, buffer, bufferSize);
}

uint32_t rage::fiDeviceCustom::ReadFromFile(const char* fileName, void* buffer, uint32_t size)
{
    return memory::call_original_vft_func<uint32_t>(this, 11, fileName, buffer, size);
}

uint32_t rage::fiDeviceCustom::ReadFromHandleBulk(HANDLE handle, uint64_t offset, void* buffer, uint32_t bufferSize)
{
    return memory::call_original_vft_func<uint32_t>(this, 12, handle, offset, buffer, bufferSize);
}

uint32_t rage::fiDeviceCustom::ReadFromHandleBulkOverlapped(HANDLE handle, uint64_t offset, char* buffer, uint32_t bufferSize)
{
    return memory::call_original_vft_func<uint32_t>(this, 13, handle, offset, buffer, bufferSize);
}

uint32_t rage::fiDeviceCustom::WriteBulk(HANDLE handle, uint64_t offset, const void* inBuffer, uint32_t bufferSize)
{
    return memory::call_original_vft_func<uint32_t>(this, 14, handle, offset, inBuffer, bufferSize);
}

uint32_t rage::fiDeviceCustom::Write(HANDLE handle, const void* buffer, uint32_t bufferSize)
{
    return memory::call_original_vft_func<uint32_t>(this, 15, handle, buffer, bufferSize);
}

uint32_t rage::fiDeviceCustom::Seek(HANDLE handle, uint32_t offset, uint32_t method)
{
    return memory::call_original_vft_func<uint32_t>(this, 16, handle, offset, method);
}

uint64_t rage::fiDeviceCustom::Seek64(HANDLE handle, int64_t distance, uint32_t method)
{
    return memory::call_original_vft_func<uint64_t>(this, 17, handle, distance, method);
}

uint32_t rage::fiDeviceCustom::CloseFileHandle(HANDLE handle)
{
    handleNames.erase(handle);
    return memory::call_original_vft_func<uint32_t>(this, 18, handle);
}

uint32_t rage::fiDeviceCustom::CloseFileHandleBulk(HANDLE handle)
{
    handleNames.erase(handle);
    return memory::call_original_vft_func<uint32_t>(this, 19, handle);
}

uint32_t rage::fiDeviceCustom::CloseFileHandleBulkOverlapped(HANDLE handle)
{
    handleNames.erase(handle);
    return memory::call_original_vft_func<uint32_t>(this, 20, handle);
}

uint32_t rage::fiDeviceCustom::GetSize(HANDLE handle)
{
    return memory::call_original_vft_func<uint32_t>(this, 21, handle);
}

uint64_t rage::fiDeviceCustom::GetSize64(HANDLE handle)
{
    return memory::call_original_vft_func<uint64_t>(this, 22, handle);
}

uint32_t rage::fiDeviceCustom::Flush(HANDLE handle)
{
    return memory::call_original_vft_func<uint32_t>(this, 23, handle);
}

bool rage::fiDeviceCustom::RemoveFile(const char* fileName)
{
    logger::write("device", "[%s] %s", __FUNCTION__, fileName);
    return memory::call_original_vft_func<bool>(this, 24, fileName);
}

bool rage::fiDeviceCustom::RenameFile(const char* from, const char* to)
{
    logger::write("device", "[%s] %s -> %s", __FUNCTION__, from, to);
    return memory::call_original_vft_func<bool>(this, 25, from, to);
}

bool rage::fiDeviceCustom::RenameFileWithProgress(const char* from, const char* to)
{
    logger::write("device", "[%s] %s -> %s", __FUNCTION__, from, to);
    return memory::call_original_vft_func<bool>(this, 26, from, to);
}

bool rage::fiDeviceCustom::MakeDirectory(const char* dir)
{
    logger::write("device", "[%s] %s", __FUNCTION__, dir);
    return memory::call_original_vft_func<bool>(this, 27, dir);
}

bool rage::fiDeviceCustom::UnmakeDirectory(const char* dir)
{
    logger::write("device", "[%s] %s", __FUNCTION__, dir);
    return memory::call_original_vft_func<bool>(this, 28, dir);
}

uint64_t rage::fiDeviceCustom::GetAvailableDiskSpace(const char* dir)
{
    return memory::call_original_vft_func<uint64_t>(this, 29, dir);
}

void rage::fiDeviceCustom::Sanitize(HANDLE handle)
{
    memory::call_original_vft_func<void>(this, 30, handle);
}

uint64_t rage::fiDeviceCustom::GetFileSizes(void* files, void* sizes)
{
    return memory::call_original_vft_func<uint64_t>(this, 31, files, sizes);
}

uint64_t rage::fiDeviceCustom::GetFileSize(const char* fileName)
{
    uint64_t size = memory::call_original_vft_func<uint64_t>(this, 32, fileName);
    return size;
}

uint64_t rage::fiDeviceCustom::GetFileTimes(void* files, void* times)
{
    return memory::call_original_vft_func<uint64_t>(this, 33, files, times);
}

uint64_t rage::fiDeviceCustom::GetFileTime(const char* fileName)
{
    uint64_t time = memory::call_original_vft_func<uint64_t>(this, 34, fileName);
    return time;
}

bool rage::fiDeviceCustom::SetFileTime(const char* fileName, uint64_t timestamp)
{
    return memory::call_original_vft_func<bool>(this, 35, fileName, timestamp);
}

HANDLE rage::fiDeviceCustom::BeginFileSearch(const char* directoryName, const char* wildcard, rage::fiFindData& findData)
{
    logger::write("device", "[%s] %s %s", __FUNCTION__, directoryName, wildcard);
    return memory::call_original_vft_func<HANDLE>(this, 36, directoryName, wildcard, &findData);
}

HANDLE rage::fiDeviceCustom::BeginFileSearch(const char* directoryName, rage::fiFindData& findData)
{
    logger::write("device", "[%s] %s", __FUNCTION__, directoryName);
    return memory::call_original_vft_func<HANDLE>(this, 37, directoryName, &findData);
}

bool rage::fiDeviceCustom::SearchNextFile(HANDLE handle, rage::fiFindData& outData)
{
    logger::write("device", "[%s]", __FUNCTION__);
    return memory::call_original_vft_func<bool>(this, 38, handle, &outData);
}

uint32_t rage::fiDeviceCustom::EndFileSearch(HANDLE handle)
{
    logger::write("device", "[%s]", __FUNCTION__);
    return memory::call_original_vft_func<uint32_t>(this, 39, handle);
}

const rage::fiDevice* rage::fiDeviceCustom::GetLowLevelDevice()
{
    return memory::call_original_vft_func<const rage::fiDevice*>(this, 40);
}

char* rage::fiDeviceCustom::FixRelativeName(char* dest, uint32_t length, const char* source)
{
    return memory::call_original_vft_func<char*>(this, 41, dest, length, source);
}

bool rage::fiDeviceCustom::SetEndOfFile(HANDLE handle)
{
    return memory::call_original_vft_func<bool>(this, 42, handle);
}

uint32_t rage::fiDeviceCustom::GetAttributes(const char* fileName)
{
    uint32_t result = memory::call_original_vft_func<uint32_t>(this, 43, fileName);

    //logger::write("device", "[%s] %s (Result: %X)", __FUNCTION__, fileName, result);
    return result;
}

bool rage::fiDeviceCustom::SetAttributes(const char* fileName, uint32_t attributes)
{
    return memory::call_original_vft_func<bool>(this, 44, fileName, attributes);
}

uint32_t rage::fiDeviceCustom::GetRootDeviceId(const char* arg)
{
    return memory::call_original_vft_func<uint32_t>(this, 45, arg);
}

bool rage::fiDeviceCustom::IsMemoryMappedDevice()
{
    return memory::call_original_vft_func<bool>(this, 46);
}

bool rage::fiDeviceCustom::ReadFileSafe(HANDLE handle, void* outBuffer, uint32_t size)
{
    return memory::call_original_vft_func<bool>(this, 47, handle, outBuffer, size);
}

bool rage::fiDeviceCustom::WriteFileSafe(HANDLE handle, const void* buffer, uint32_t size)
{
    return memory::call_original_vft_func<bool>(this, 48, handle, buffer, size);
}

uint32_t rage::fiDeviceCustom::GetResourceInfos(void* resources, rage::fiResourceInfo** flags)
{
    return memory::call_original_vft_func<uint32_t>(this, 49, resources, flags);
}

uint32_t rage::fiDeviceCustom::GetResourceInfo(const char* name, rage::fiResourceInfo* outHeader)
{
    return memory::call_original_vft_func<uint32_t>(this, 50, name, outHeader);
}

uint32_t rage::fiDeviceCustom::GetEncryptionKey()
{
    return memory::call_original_vft_func<uint32_t>(this, 51);
}

bool rage::fiDeviceCustom::IsValidFileHandle(HANDLE handle)
{
    return memory::call_original_vft_func<bool>(this, 52, handle);
}

uint64_t rage::fiDeviceCustom::GetBulkFileOffset(HANDLE handle)
{
    return memory::call_original_vft_func<uint64_t>(this, 53, handle);
}

uint32_t rage::fiDeviceCustom::GetPhysicalSortKey(const char* arg)
{
    return memory::call_original_vft_func<uint32_t>(this, 54, arg);
}

bool rage::fiDeviceCustom::IsRpfArchive()
{
    return memory::call_original_vft_func<bool>(this, 55);
}

uint64_t rage::fiDeviceCustom::GetRpfArchiveVersion()
{
    return memory::call_original_vft_func<uint64_t>(this, 56);
}

const rage::fiDevice* rage::fiDeviceCustom::GetRpfArchiveDevice()
{
    return memory::call_original_vft_func<const rage::fiDevice*>(this, 57);
}

bool rage::fiDeviceCustom::IsCloudDevice()
{;
    return memory::call_original_vft_func<bool>(this, 58);
}

uint32_t rage::fiDeviceCustom::GetPackfileIndex()
{
    return memory::call_original_vft_func<uint32_t>(this, 59);
}

const char* rage::fiDeviceCustom::GetDeviceName()
{
    return memory::call_original_vft_func<const char*>(this, 60);
}

bool rage::fiDeviceCustom::SupportsOverlappedIO()
{
    return memory::call_original_vft_func<bool>(this, 61);
}

uint32_t rage::fiDeviceCustom::GetClampedBufferSize(uint64_t arg1, uint32_t arg2)
{
    return memory::call_original_vft_func<uint32_t>(this, 62, arg1, arg2);
}

uint64_t rage::fiDeviceCustom::GetFinalOffset(uint64_t arg)
{
    return memory::call_original_vft_func<uint64_t>(this, 63, arg);
}

bool rage::fiDeviceCustom::IsOverlappedRequestFinished(uint64_t arg1, uint32_t arg2)
{
    return memory::call_original_vft_func<bool>(this, 64, arg1, arg2);
}

HANDLE rage::fiDeviceCustom::CreateMapping(HANDLE handle, bool writeAccess, uint64_t offset, size_t viewSize, void** outMappedView)
{
    logger::write("device", "[%s] %s", __FUNCTION__, getHandleName(handle));
    return memory::call_original_vft_func<HANDLE>(this, 65, handle, writeAccess, offset, viewSize, outMappedView);
}

uint32_t rage::fiDeviceCustom::CloseMapping(HANDLE mappingHandle, void* mappedView)
{
    logger::write("device", "[%s]", __FUNCTION__);
    return memory::call_original_vft_func<uint32_t>(this, 66, mappingHandle, mappedView);
}

// Packfile functions

bool rage::fiDeviceCustom::IsPackfileInitialized()
{
    return memory::call_original_vft_func<bool>(this, 67);
}

bool rage::fiDeviceCustom::InitializePackfile(const char* filePath, bool useNametable, uint32_t cacheMode, char* headerData, const fiResourceInfo* a5, bool a6)
{
    logger::write("device", "[%s] %s", __FUNCTION__, filePath);
    return memory::call_original_vft_func<bool>(this, 68, filePath, useNametable, cacheMode, headerData, a5, a6);
}

bool rage::fiDeviceCustom::MountPackfileAs(const char* mountPoint)
{
    logger::write("device", "[%s] %s (%s)", __FUNCTION__, mountPoint, this->GetDeviceName());
    return memory::call_original_vft_func<bool>(this, 69, mountPoint);
}

void rage::fiDeviceCustom::SetRelativePath(const char* relativePath)
{
    logger::write("device", "[%s] %s", __FUNCTION__, relativePath);
    memory::call_original_vft_func<void>(this, 70, relativePath);
}

void rage::fiDeviceCustom::ClearRelativeOffset()
{
    memory::call_original_vft_func<void>(this, 71);
}

int rage::fiDeviceCustom::GetRelativeOffset()
{
    return memory::call_original_vft_func<int>(this, 72);
}

void rage::fiDeviceCustom::ShutdownPackfile()
{
    logger::write("device", "[%s] (%s)", __FUNCTION__, this->GetDeviceName());
    memory::call_original_vft_func<void>(this, 73);
}

bool rage::fiDeviceCustom::ReInitializePackFile(const char* filePath, bool useNametable, char* headerData, const fiResourceInfo* info, bool flag)
{
    logger::write("device", "[%s] %s", __FUNCTION__, filePath);
    return memory::call_original_vft_func<bool>(this, 74, filePath, useNametable, headerData, info, flag);
}

void rage::fiDeviceCustom::UnInitializePackFile()
{
    logger::write("device", "[%s] (%s)", __FUNCTION__, this->GetDeviceName());
    memory::call_original_vft_func<void>(this, 75);
}

void rage::fiDeviceCustom::SetResource()
{
    memory::call_original_vft_func<void>(this, 76);
}

uint32_t rage::fiDeviceCustom::GetFileExtensionPlatform()
{
    return memory::call_original_vft_func<uint32_t>(this, 77);
}

HANDLE rage::fiDeviceCustom::OpenBulkFromHandle(HANDLE handle, uint64_t& outbias)
{
    logger::write("device", "[%s] %s", __FUNCTION__, getHandleName(handle));
    return memory::call_original_vft_func<HANDLE>(this, 78, handle, &outbias);
}

bool rage::fiDeviceCustom::HasEntries()
{
    return memory::call_original_vft_func<bool>(this, 79);
}

uint32_t rage::fiDeviceCustom::GetEntryCount()
{
    return memory::call_original_vft_func<uint32_t>(this, 80);
}

uint32_t rage::fiDeviceCustom::GetEntryPhysicalSortKey(uint32_t a1, bool a2)
{
    return memory::call_original_vft_func<uint32_t>(this, 81, a1, a2);
}

uint32_t rage::fiDeviceCustom::GetEntryNameHash(HANDLE handle)
{
    uint32_t hash = memory::call_original_vft_func<uint32_t>(this, 82, handle);

    char namebuffer[512]{};
    sprintf_s(namebuffer, "0x%X.bin (%s)", hash, this->GetDeviceName());
    handleNames[handle] = namebuffer;

    return hash;
}

uint8_t rage::fiDeviceCustom::GetEntryFileExtId(HANDLE handle)
{
    return memory::call_original_vft_func<uint8_t>(this, 83, handle);
}

const char* rage::fiDeviceCustom::GetEntryName(HANDLE handle, char* dest, uint32_t destSize)
{
    const char* name = memory::call_original_vft_func<const char*>(this, 84, handle, dest, destSize);

    if (name != nullptr)
        handleNames[handle] = name;

    return name;
}

const char* rage::fiDeviceCustom::GetEntryFullName(HANDLE handle, char* dest, uint32_t destSize)
{
    const char* name = memory::call_original_vft_func<const char*>(this, 85, handle, dest, destSize);

    if (name != nullptr)
        handleNames[handle] = name;

    return name;
}

uint32_t rage::fiDeviceCustom::GetEntryIndexHash(uint32_t hash, uint32_t fileExt)
{
    return memory::call_original_vft_func<uint32_t>(this, 86, hash, fileExt);
}

uint32_t rage::fiDeviceCustom::GetEntryIndex(const char* filename, bool unused)
{
    return memory::call_original_vft_func<uint32_t>(this, 87, filename, unused);
}

unsigned int rage::fiDeviceCustom::GetBasePhysicalSortKey()
{
    return memory::call_original_vft_func<unsigned int>(this, 88);
}

bool rage::fiDeviceCustom::Prefetch(unsigned int a1)
{
    return memory::call_original_vft_func<bool>(this, 89, a1);
}

bool rage::fiDeviceCustom::IsPackfile()
{
    return memory::call_original_vft_func<bool>(this, 90);
}

bool rage::fiDeviceCustom::IsStreaming()
{
    return memory::call_original_vft_func<bool>(this, 91);
}

void rage::fiDeviceCustom::SetStreaming(bool a1)
{
    memory::call_original_vft_func<void>(this, 92, a1);
}

uint64_t rage::fiDeviceCustom::GetHeaderSize()
{
    return memory::call_original_vft_func<uint64_t>(this, 93);
}

uint64_t rage::fiDeviceCustom::GetPackfileSize()
{
    return memory::call_original_vft_func<uint64_t>(this, 94);
}

uint64_t rage::fiDeviceCustom::GetPackfileTime()
{
    return memory::call_original_vft_func<uint64_t>(this, 95);
}

void rage::fiDeviceCustom::SetPackfileTime(uint64_t a1)
{
    memory::call_original_vft_func<void>(this, 96, a1);
}

void* rage::fiDeviceCustom::GetPackfileHandle()
{
    return memory::call_original_vft_func<void*>(this, 97);
}

void rage::fiDeviceCustom::SetPackfileHandle(void* a1)
{
    memory::call_original_vft_func<void>(this, 98, a1);
}

void* rage::fiDeviceCustom::GetEntryResourceInfo(void* a1, uint16_t a2)
{
    return memory::call_original_vft_func<void*>(this, 99, a1, a2);
}

void rage::fiDeviceCustom::PrecacheEntryResourceInfo(void* a1)
{
    memory::call_original_vft_func<void>(this, 100, a1);
}

uint64_t rage::fiDeviceCustom::GetFileOffset(uint16_t a1)
{
    return memory::call_original_vft_func<uint64_t>(this, 101, a1);
}

uint64_t rage::fiDeviceCustom::GetOnDiskSize(uint16_t a1)
{
    return memory::call_original_vft_func<uint64_t>(this, 102, a1);
}

uint64_t rage::fiDeviceCustom::GetInMemorySize(uint16_t a1)
{
    return memory::call_original_vft_func<uint64_t>(this, 103, a1);
}

bool rage::fiDeviceCustom::IsDirectory(uint16_t a1)
{
    return memory::call_original_vft_func<bool>(this, 104, a1);
}

uint8_t rage::fiDeviceCustom::GetCompressorId(uint16_t a1)
{
    return memory::call_original_vft_func<uint8_t>(this, 105, a1);
}

bool rage::fiDeviceCustom::IsEncrypted(uint16_t a1)
{
    return memory::call_original_vft_func<bool>(this, 106, a1);
}

bool rage::fiDeviceCustom::IsResource(uint16_t a1)
{
    return memory::call_original_vft_func<bool>(this, 107, a1);
}

bool rage::fiDeviceCustom::IsSignatureProtected(uint16_t a1)
{
    return memory::call_original_vft_func<bool>(this, 108, a1);
}

void* rage::fiDeviceCustom::GetDatResourceInfo(void* data, uint16_t a2)
{
    return memory::call_original_vft_func<void*>(this, 109, data, a2);
}

uint8_t rage::fiDeviceCustom::GetEncryptionKeyId(uint16_t a1)
{
    return memory::call_original_vft_func<uint8_t>(this, 110, a1);
}

uint8_t rage::fiDeviceCustom::GetEncryptionConfig(uint16_t a1)
{
    return memory::call_original_vft_func<uint8_t>(this, 111, a1);
}

void* rage::fiDeviceCustom::WriteToCache(void* a1)
{
    return memory::call_original_vft_func<void*>(this, 112, a1);
}

void rage::fiDeviceCustom::Func_113(void* a1)
{
    return memory::call_original_vft_func<void>(this, 113, a1);
}

const char* rage::fiDeviceCustom::GetPackfileName()
{
    return memory::call_original_vft_func<const char*>(this, 114);
}
