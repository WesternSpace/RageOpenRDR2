#include "main.h"

bool rage::fiDevice::Mount(const char* mountPoint)
{
	logger::write("device", "[%s] %s", __FUNCTION__, mountPoint);
	static auto func = memory::scan("48 89 5c 24 ? 57 48 81 ec ? ? ? ? 44 8a 81 ? ? ? ? 48 8b da 48 8b d1 48 8b f9 48 8b cb")
		.as<bool(*)(void*, const char*)>();
	return func(this, mountPoint);
}

void rage::fiDevice::SetPath(const char* path, bool allowRoot, rage::fiDevice* parent)
{
	logger::write("device", "[%s] %s", __FUNCTION__, path);
	static auto func = memory::scan("48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 57 48 81 ec ? ? ? ? 49 8b f1 41 8a e8 48 8b da 48 8b f9 e8")
		.as<void(*)(void*, const char*, bool, rage::fiDevice*)>();
	func(this, path, allowRoot, parent);
}

rage::fiDeviceRelative::fiDeviceRelative()
{
	static auto relativeDeviceVMT = memory::scan("48 8d 05 ? ? ? ? 48 89 5e ? 48 89 06 eb ? 48 8b f3").add(3).rip().as<void*>();
	VMT = relativeDeviceVMT;
	pad[0x158] = 0;
}

bool rage::fiDeviceRelative::Mount(const char* mountPoint)
{
	return reinterpret_cast<rage::fiDevice*>(this)->Mount(mountPoint);
}

void rage::fiDeviceRelative::SetPath(const char* path, bool allowRoot, rage::fiDevice* parent)
{
	reinterpret_cast<rage::fiDevice*>(this)->SetPath(path, allowRoot, parent);
}

rage::fiDeviceLocal::fiDeviceLocal() {
	pad[0x158] = 0;
}
rage::fiDeviceLocal::~fiDeviceLocal(){}

std::unordered_map<HANDLE, std::string> handleNames;

// A lot of the functions are not implemented, but they are not needed.

HANDLE rage::fiDeviceLocal::Open(const char* fileName, bool readOnly, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes)
{
	logger::write("device", "[%s] %s", __FUNCTION__, fileName);
	HANDLE handle = new std::ifstream(ToFullPath(fileName), std::ios::in | std::ios::binary);
	handleNames[handle] = fileName;
	return handle;
}

HANDLE rage::fiDeviceLocal::Open(uint32_t* fileNameHash)
{
	logger::write("device", "[%s] %02x", __FUNCTION__, *fileNameHash);
	return INVALID_HANDLE_VALUE;
}

HANDLE rage::fiDeviceLocal::Open(uint32_t* fileNameHash, uint32_t fileExt)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return INVALID_HANDLE_VALUE;
}

HANDLE rage::fiDeviceLocal::Open(const char* fileName, bool readOnly)
{
	logger::write("device", "[%s] %s", __FUNCTION__, fileName);
	return Open(fileName, readOnly, 0, 0);
}

HANDLE rage::fiDeviceLocal::OpenBulk(const char* fileName, uint64_t& outbias)
{
	logger::write("device", "[%s] %s", __FUNCTION__, fileName);
	return Open(fileName, true, 0, 0);
}

HANDLE rage::fiDeviceLocal::OpenBulkOverlapped(const char* fileName)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return Open(fileName, true, 0, 0);
}

HANDLE rage::fiDeviceLocal::CreateBulk(const char* fileName)
{
	logger::write("device", "[%s] %s", __FUNCTION__, fileName);
	return INVALID_HANDLE_VALUE;
}

HANDLE rage::fiDeviceLocal::Create(const char* fileName, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes)
{
	logger::write("device", "[%s] %s", __FUNCTION__, fileName);
	return 0;
}

HANDLE rage::fiDeviceLocal::Create(const char* fileName)
{
	logger::write("device", "[%s] %s", __FUNCTION__, fileName);
	return 0;
}

int rage::fiDeviceLocal::Read(HANDLE handle, void* buffer, int bufferSize)
{
	logger::write("device", "[%s] %s %d", __FUNCTION__, handleNames[handle].c_str(), bufferSize);
	auto file = (std::ifstream*)handle;
	file->read((char*)buffer, bufferSize);
	return (uint32_t)file->gcount();
}

int rage::fiDeviceLocal::ReadFile(const char* fileName, void* buffer, int size)
{
	logger::write("device", "[%s] %s %d", __FUNCTION__, fileName, size);
	HANDLE handle = Open(fileName, true, 0, 0);
	if (handle == INVALID_HANDLE_VALUE)
		return -1;
	uint64_t fileSize = Size64(handle);
	int bytesRead = (int)Read(handle, buffer, size);
	Close(handle);
	return (bytesRead == (int)fileSize) ? bytesRead : -1;
}

int rage::fiDeviceLocal::ReadBulk(HANDLE handle, uint64_t offset, void* buffer, int bufferSize)
{
	logger::write("device", "[%s] %s %d", __FUNCTION__, handleNames[handle].c_str(), bufferSize);
	auto file = (std::ifstream*)handle;
	file->seekg(offset);
	return Read(handle, buffer, bufferSize);
}

int rage::fiDeviceLocal::ReadBulkOverlapped(HANDLE handle, uint64_t offset, char* buffer, int bufferSize)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return ReadBulk(handle, offset, buffer, bufferSize);
}

int rage::fiDeviceLocal::WriteBulk(HANDLE handle, uint64_t offset, const void* inBuffer, int bufferSize)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

int rage::fiDeviceLocal::Write(HANDLE handle, const void* buffer, int bufferSize)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

int rage::fiDeviceLocal::Seek(HANDLE handle, int offset, uint32_t method)
{
	return (uint32_t)Seek64(handle, (int64_t)offset, method);
}

uint64_t rage::fiDeviceLocal::Seek64(HANDLE handle, int64_t distance, uint32_t method)
{
	logger::write("device", "[%s] %s %lld", __FUNCTION__, handleNames[handle].c_str(), distance);
	auto file = (std::ifstream*)handle;
	if (method == SEEK_CUR)
		file->seekg(distance, std::ios::cur);
	else if (method == SEEK_SET)
		file->seekg(distance, std::ios::beg);
	else if (method == SEEK_END)
		file->seekg(-distance, std::ios::end);
	return (uint64_t)file->tellg();
}

uint32_t rage::fiDeviceLocal::Close(HANDLE handle)
{
	logger::write("device", "[%s] %s", __FUNCTION__, handleNames[handle].c_str());
	auto file = (std::ifstream*)handle;
	delete file;
	handleNames.erase(handle);
	return 1;
}

uint32_t rage::fiDeviceLocal::CloseBulk(HANDLE handle)
{
	return Close(handle);
}

uint32_t rage::fiDeviceLocal::CloseBulkOverlapped(HANDLE handle)
{
	return CloseBulk(handle);
}

int rage::fiDeviceLocal::Size(HANDLE handle)
{
	return (int)Size64(handle);
}

uint64_t rage::fiDeviceLocal::Size64(HANDLE handle)
{
	uint64_t cur = Seek64(handle, 0, SEEK_CUR);
	uint64_t end = Seek64(handle, 0, SEEK_END);
	Seek64(handle, (int64_t)cur, SEEK_SET);
	logger::write("device", "[%s] %s %llu", __FUNCTION__, handleNames[handle].c_str(), end);
	return end;
}

int rage::fiDeviceLocal::Flush(HANDLE handle)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

bool rage::fiDeviceLocal::Delete(const char* fileName)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

bool rage::fiDeviceLocal::Rename(const char* from, const char* to)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

bool rage::fiDeviceLocal::RenameWithProgress(const char* from, const char* to)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

bool rage::fiDeviceLocal::MakeDirectory(const char* dir)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

bool rage::fiDeviceLocal::UnmakeDirectory(const char* dir)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

uint64_t rage::fiDeviceLocal::GetAvailableDiskSpace(const char* dir)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

void rage::fiDeviceLocal::Sanitize(HANDLE handle)
{
	logger::write("device", "[%s]", __FUNCTION__);
}

uint64_t rage::fiDeviceLocal::GetFileSize(void* files, void* sizes)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

uint64_t rage::fiDeviceLocal::GetFileSize(const char* fileName)
{
	uint64_t size = 0;
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	if (GetFileAttributesExA(ToFullPath(fileName).c_str(), GetFileExInfoStandard, &fileData))
		size = fileData.nFileSizeLow | (static_cast<uint64_t>(fileData.nFileSizeHigh) << 32);
	logger::write("device", "[%s] %s %llu", __FUNCTION__, fileName, size);
	return size;
}

uint64_t rage::fiDeviceLocal::GetFileTime(void* files, void* times)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

uint64_t rage::fiDeviceLocal::GetFileTime(const char* fileName)
{
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	uint64_t filetime = 0;
	if (GetFileAttributesExA(ToFullPath(fileName).c_str(), GetFileExInfoStandard, &fileData))
		filetime = *(uint64_t*)&fileData.ftLastWriteTime;
	logger::write("device", "[%s] %s %llu", __FUNCTION__, fileName, filetime);
	return filetime;
}

bool rage::fiDeviceLocal::SetFileTime(const char* fileName, uint64_t fileTime)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

HANDLE rage::fiDeviceLocal::FindFileBegin(const char* path, const char* wildcard, rage::fiFindData& findData)
{
	logger::write("device", "[%s] %s %s", __FUNCTION__, path, wildcard);
	WIN32_FIND_DATAW foundData;

	wchar_t wcPath[256] = { 0 };
	MultiByteToWideChar(0xFDE9u, 0, ToFullPath(path).c_str(), -1, wcPath, 256);

	wchar_t wcWildcard[256] = { 0 };
	MultiByteToWideChar(0xFDE9u, 0, wildcard, -1, wcWildcard, 256);

	wchar_t fileName[MAX_PATH] = { 0 };
	auto pathLen = wcslen(wcPath);
	bool slashFound = pathLen && (wcPath[pathLen - 1] == L'/' || wcPath[pathLen - 1] == L'\\');
	swprintf_s(fileName, MAX_PATH, slashFound ? L"%s%s" : L"%s\\%s", wcPath, wcWildcard);

	HANDLE firstFileHandle = FindFirstFileW(fileName, &foundData);
	if (firstFileHandle == INVALID_HANDLE_VALUE)
		return INVALID_HANDLE_VALUE;

	ConvertFoundData(&foundData, findData);
	return firstFileHandle;
}

HANDLE rage::fiDeviceLocal::FindFileBegin(const char* path, rage::fiFindData& findData)
{
	return FindFileBegin(path, "*", findData);
}

bool rage::fiDeviceLocal::FindFileNext(HANDLE handle, rage::fiFindData& outData)
{
	logger::write("device", "[%s]", __FUNCTION__);
	WIN32_FIND_DATAW foundData;
	bool fileFound = FindNextFileW(handle, &foundData) != 0;
	if (fileFound)
		ConvertFoundData(&foundData, outData);
	return fileFound;
}

int rage::fiDeviceLocal::FindFileEnd(HANDLE handle)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return (FindClose(handle) != 0) ? 0 : -1;
}

const rage::fiDevice* rage::fiDeviceLocal::GetLowLevelDevice()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return this;
}

char* rage::fiDeviceLocal::FixRelativeName(char* dest, int length, const char* source)
{
	logger::write("device", "[%s]", __FUNCTION__);
	memcpy(dest, source, length);
	return dest;
}

bool rage::fiDeviceLocal::SetEndOfFile(HANDLE handle)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

uint32_t rage::fiDeviceLocal::GetAttributes(const char* fileName)
{
	std::string fullPath = ToFullPath(fileName);
	uint32_t attributes = ::GetFileAttributesA(fullPath.c_str());
	logger::write("device", "[%s] %s (Newmods path: %s)", __FUNCTION__, fileName, fullPath.c_str());
	return attributes;
}

bool rage::fiDeviceLocal::SetAttributes(const char* fileName, uint32_t attributes)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

uint32_t rage::fiDeviceLocal::GetRootDeviceId(const char*)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 1;
}

bool rage::fiDeviceLocal::IsMemoryMappedDevice()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

bool rage::fiDeviceLocal::SafeRead(HANDLE handle, void* outBuffer, int size)
{
	logger::write("device", "[%s]", __FUNCTION__);
	uint32_t offset = 0;
	if (size == 0)
		return true;
	for (;;)
	{
		uint32_t bytesRead = Read(handle, (char*)outBuffer + offset, size - offset);
		if (bytesRead == 0)
			break;
		offset += bytesRead;
		if (offset >= size)
			return true;
	}
	return false;
}

bool rage::fiDeviceLocal::SafeWrite(HANDLE handle, const void* buffer, int size)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

int rage::fiDeviceLocal::GetResourceInfo(void* resources, rage::fiResourceInfo** flags)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

int rage::fiDeviceLocal::GetResourceInfo(const char* fileName, rage::fiResourceInfo* flags)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

uint32_t rage::fiDeviceLocal::GetEncryptionKey()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

bool rage::fiDeviceLocal::IsValidHandle(HANDLE handle)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

uint64_t rage::fiDeviceLocal::GetBulkOffset(HANDLE handle)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 1;
}

uint32_t rage::fiDeviceLocal::GetPhysicalSortKey(const char*)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0x40000000;
}

bool rage::fiDeviceLocal::IsRpf()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

uint64_t rage::fiDeviceLocal::GetRpfVersion()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return -1;
}

const rage::fiDevice* rage::fiDeviceLocal::GetRpfDevice()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return this;
}

bool rage::fiDeviceLocal::IsCloud()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

bool rage::fiDeviceLocal::IsZip()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

const char* rage::fiDeviceLocal::GetDebugName()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return "rage::fiDeviceLocal";
}

bool rage::fiDeviceLocal::SupportsOverlappedIO()
{
	logger::write("device", "[%s]", __FUNCTION__);
	return false;
}

uint32_t rage::fiDeviceLocal::GetClampedBufferSize(uint64_t, int)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

uint64_t rage::fiDeviceLocal::GetFinalOffset(uint64_t)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

bool rage::fiDeviceLocal::IsOverlappedRequestFinished(uint64_t, int)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

HANDLE rage::fiDeviceLocal::CreateMapping(HANDLE fileHandle, bool writeAccess, uint64_t offset, size_t viewSize, void** outMappedView)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return INVALID_HANDLE_VALUE;
}

int rage::fiDeviceLocal::CloseMapping(HANDLE mappingHandle, void* mappedView)
{
	logger::write("device", "[%s]", __FUNCTION__);
	return 0;
}

void rage::fiDeviceLocal::ConvertFoundData(LPWIN32_FIND_DATAW foundDataWin, rage::fiFindData& foundDataRage)
{
	logger::write("device", "[%s]", __FUNCTION__);
	WideCharToMultiByte(0xFDE9u, 0, foundDataWin->cFileName, -1, foundDataRage.fileName, 256, nullptr, nullptr);
	for (uint8_t i = 0; i < (uint8_t)strlen(foundDataRage.fileName); ++i)
		if (foundDataRage.fileName[i] == '/')
			foundDataRage.fileName[i] = '\\';
	foundDataRage.lastWriteTime = foundDataWin->ftLastWriteTime;
	foundDataRage.fileAttributes = foundDataWin->dwFileAttributes;
	foundDataRage.fileSize = *(uint64_t*)(&foundDataWin->nFileSizeHigh);
}