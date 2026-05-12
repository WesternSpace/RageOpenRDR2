#pragma once

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
		virtual HANDLE Open(const char* fileName, bool readOnly, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) = 0;
		virtual HANDLE Open(uint32_t* fileNameHash) = 0;
		virtual HANDLE Open(uint32_t* fileNameHash, uint32_t fileExt) = 0;
		virtual HANDLE Open(const char* fileName, bool readOnly) = 0; // called in VT[11]
		virtual HANDLE OpenBulk(const char* fileName, uint64_t& outbias) = 0;
		virtual HANDLE OpenBulkOverlapped(const char* fileName) = 0;

		virtual HANDLE CreateBulk(const char* fileName) = 0;
		virtual HANDLE Create(const char* fileName, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) = 0;
		virtual HANDLE Create(const char* fileName) = 0;

		virtual int Read(HANDLE handle, void* buffer, int bufferSize) = 0;
		virtual int ReadFile(const char* filename, void* buffer, int bufferSize) = 0; // opens the file (VT[4]), calls Read on it (VT[10]), calls VT[21] ((Size64) bunch of seek calls (VT[16])), then closes the file (VT[18]). It returns full file size on success.
		virtual int ReadBulk(HANDLE handle, uint64_t offset, void* buffer, int bufferSize) = 0;
		virtual int ReadBulkOverlapped(HANDLE handle, uint64_t offset, char* buffer, int bufferSize) = 0;

		virtual int WriteBulk(HANDLE handle, uint64_t offset, const void* inBuffer, int bufferSize) = 0;
		virtual int Write(HANDLE handle, const void* buffer, int bufferSize) = 0;

		virtual int Seek(HANDLE handle, int offset, uint32_t method) = 0; // Delegates to Seek64
		virtual uint64_t Seek64(HANDLE handle, int64_t distance, uint32_t method) = 0;

		virtual uint32_t Close(HANDLE handle) = 0;
		virtual uint32_t CloseBulk(HANDLE handle) = 0; // Same impl as Close..
		virtual uint32_t CloseBulkOverlapped(HANDLE handle) = 0; // Delegates to CloseBulk

		virtual int Size(HANDLE handle) = 0; // Delegates to Size64
		virtual uint64_t Size64(HANDLE handle) = 0;

		virtual int Flush(HANDLE handle) = 0; // Just returns 0

		virtual bool Delete(const char* fileName) = 0;

		virtual bool Rename(const char* from, const char* to) = 0;
		virtual bool RenameWithProgress(const char* from, const char* to) = 0;

		virtual bool MakeDirectory(const char* dir) = 0;
		virtual bool UnmakeDirectory(const char* dir) = 0;

		virtual uint64_t GetAvailableDiskSpace(const char* dir) = 0;

		virtual void Sanitize(HANDLE handle) = 0;

		virtual uint64_t GetFileSize(void* files, void* sizes) = 0; // probably not the best param types. First is an array of files which is iterated through and the size of each each calculated and put in the second array of sizes. The sum is returned.
		virtual uint64_t GetFileSize(const char* fileName) = 0;
		
		virtual uint64_t GetFileTime(void* files, void* times) = 0; // Like GetFileSizes, but for times. Except that it doesn't return the sum, it returns times[0] if times[1] != 0, otherwise 0.
		virtual uint64_t GetFileTime(const char* fileName) = 0;
		virtual bool SetFileTime(const char* fileName, uint64_t timestamp) = 0;

		virtual HANDLE FindFileBegin(const char* directoryName, const char* wildcard, rage::fiFindData& findData) = 0;
		virtual HANDLE FindFileBegin(const char* directoryName, rage::fiFindData& findData) = 0;
		virtual bool FindFileNext(HANDLE handle, rage::fiFindData& outData) = 0;
		virtual int FindFileEnd(HANDLE handle) = 0;

		virtual const rage::fiDevice* GetLowLevelDevice() = 0; // Returns this

		virtual char* FixRelativeName(char* dest, int destSize, const char* src) = 0;

		virtual bool SetEndOfFile(HANDLE handle) = 0;

		virtual uint32_t GetAttributes(const char* fileName) = 0;
		virtual bool SetAttributes(const char* fileName, uint32_t attributes) = 0;

		virtual uint32_t GetRootDeviceId(const char*) = 0; // Returns 2 (HDD)

		virtual bool IsMemoryMappedDevice() = 0; // Returns 0

		virtual bool SafeRead(HANDLE handle, void* outBuffer, int size) = 0;
		virtual bool SafeWrite(HANDLE handle, const void* buffer, int size) = 0;

		virtual int GetResourceInfo(void* resources, fiResourceInfo** flags) = 0;
		virtual int GetResourceInfo(const char* name, fiResourceInfo* outHeader) = 0;

		virtual uint32_t GetEncryptionKey() = 0; // returns 0

		virtual bool IsValidHandle(HANDLE handle) = 0; // returns 1

		virtual uint64_t GetBulkOffset(HANDLE handle) = 0; // returns 0

		virtual uint32_t GetPhysicalSortKey(const char*) = 0;

		virtual bool IsRpf() = 0; // returns 0
		virtual uint64_t GetRpfVersion() = 0; // returns -1
		virtual const fiDevice* GetRpfDevice() = 0; // returns this

		virtual bool IsCloud() = 0; // returns 0

		virtual bool IsZip() = 0; // returns 0

		virtual const char* GetDebugName() = 0;

		virtual bool SupportsOverlappedIO() = 0; // returns 0;

		virtual uint32_t GetClampedBufferSize(uint64_t, int) = 0; // returns the third param

		virtual uint64_t GetFinalOffset(uint64_t) = 0; // returns the second param

		virtual bool IsOverlappedRequestFinished(uint64_t, int) = 0;

		virtual HANDLE CreateMapping(HANDLE fileHandle, bool writeAccess, uint64_t offset, size_t viewSize, void** outMappedView) = 0;
		virtual int CloseMapping(HANDLE mappingHandle, void* mappedView) = 0;

		bool Mount(const char* mountPoint);
		void SetPath(const char* path, bool allowRoot, rage::fiDevice* parent);
	};

	class __declspec(novtable) fiDeviceRelative
	{
		void* VMT;
		char pad[0x168];
	public:
		fiDeviceRelative();

		bool Mount(const char* mountPoint);
		void SetPath(const char* path, bool allowRoot, rage::fiDevice* parent);
	};

	class fiDeviceLocal : public fiDevice
	{
		char pad[0x180];
	public:
		fiDeviceLocal();
		~fiDeviceLocal();
		virtual HANDLE Open(const char* fileName, bool readOnly, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) override;
		virtual HANDLE Open(uint32_t* fileNameHash) override;
		virtual HANDLE Open(uint32_t* fileNameHash, uint32_t fileExt) override;
		virtual HANDLE Open(const char* fileName, bool readOnly) override;
		virtual HANDLE OpenBulk(const char* fileName, uint64_t& outbias) override;
		virtual HANDLE OpenBulkOverlapped(const char* fileName) override;

		virtual HANDLE CreateBulk(const char* fileName) override;
		virtual HANDLE Create(const char* fileName, uint32_t dwShareMode, uint32_t dwFlagsAndAttributes) override;
		virtual HANDLE Create(const char* fileName) override;

		virtual int Read(HANDLE handle, void* buffer, int bufferSize) override;
		virtual int ReadFile(const char* filename, void* buffer, int bufferSize) override;
		virtual int ReadBulk(HANDLE handle, uint64_t offset, void* buffer, int bufferSize) override;
		virtual int ReadBulkOverlapped(HANDLE handle, uint64_t offset, char* buffer, int bufferSize) override;

		virtual int WriteBulk(HANDLE handle, uint64_t offset, const void* inBuffer, int bufferSize) override;
		virtual int Write(HANDLE handle, const void* buffer, int bufferSize) override;

		virtual int Seek(HANDLE handle, int offset, uint32_t method) override;
		virtual uint64_t Seek64(HANDLE handle, int64_t distance, uint32_t method) override;

		virtual uint32_t Close(HANDLE handle) override;
		virtual uint32_t CloseBulk(HANDLE handle) override;
		virtual uint32_t CloseBulkOverlapped(HANDLE handle) override;

		virtual int Size(HANDLE handle) override;
		virtual uint64_t Size64(HANDLE handle) override;

		virtual int Flush(HANDLE handle) override;

		virtual bool Delete(const char* fileName) override;

		virtual bool Rename(const char* from, const char* to) override;
		virtual bool RenameWithProgress(const char* from, const char* to) override;

		virtual bool MakeDirectory(const char* dir) override;
		virtual bool UnmakeDirectory(const char* dir) override;

		virtual uint64_t GetAvailableDiskSpace(const char* dir) override;

		virtual void Sanitize(HANDLE handle) override;

		virtual uint64_t GetFileSize(void* files, void* sizes) override;
		virtual uint64_t GetFileSize(const char* fileName) override;
		
		virtual uint64_t GetFileTime(void* files, void* times) override;
		virtual uint64_t GetFileTime(const char* fileName) override;
		virtual bool SetFileTime(const char* fileName, uint64_t timestamp) override;

		virtual HANDLE FindFileBegin(const char* directoryName, const char* wildcard, rage::fiFindData& findData) override;
		virtual HANDLE FindFileBegin(const char* directoryName, rage::fiFindData& findData) override;
		virtual bool FindFileNext(HANDLE handle, rage::fiFindData& outData) override;
		virtual int FindFileEnd(HANDLE handle) override;

		virtual const rage::fiDevice* GetLowLevelDevice() override;

		virtual char* FixRelativeName(char* dest, int destSize, const char* src) override;

		virtual bool SetEndOfFile(HANDLE handle) override;

		virtual uint32_t GetAttributes(const char* fileName) override;
		virtual bool SetAttributes(const char* fileName, uint32_t attributes) override;

		virtual uint32_t GetRootDeviceId(const char*) override;

		virtual bool IsMemoryMappedDevice() override;

		virtual bool SafeRead(HANDLE handle, void* outBuffer, int size) override;
		virtual bool SafeWrite(HANDLE handle, const void* buffer, int size) override;

		virtual int GetResourceInfo(void* resources, fiResourceInfo** flags) override;
		virtual int GetResourceInfo(const char* name, fiResourceInfo* outHeader) override;

		virtual uint32_t GetEncryptionKey() override;

		virtual bool IsValidHandle(HANDLE handle) override;

		virtual uint64_t GetBulkOffset(HANDLE handle) override;

		virtual uint32_t GetPhysicalSortKey(const char*) override;

		virtual bool IsRpf() override;
		virtual uint64_t GetRpfVersion() override;
		virtual const fiDevice* GetRpfDevice() override;

		virtual bool IsCloud() override;

		virtual bool IsZip() override;

		virtual const char* GetDebugName() override;

		virtual bool SupportsOverlappedIO() override;

		virtual uint32_t GetClampedBufferSize(uint64_t, int) override;

		virtual uint64_t GetFinalOffset(uint64_t) override;

		virtual bool IsOverlappedRequestFinished(uint64_t, int) override;

		virtual HANDLE CreateMapping(HANDLE fileHandle, bool writeAccess, uint64_t offset, size_t viewSize, void** outMappedView) override;
		virtual int CloseMapping(HANDLE mappingHandle, void* mappedView) override;

		void ConvertFoundData(LPWIN32_FIND_DATAW foundDataWin, rage::fiFindData& foundDataRage);

		static std::string ToFullPath(const char* fileName)
		{
			std::filesystem::path cwd = std::filesystem::current_path() / "newmods/";

			std::string fName(fileName);

			// remove the crc part in the device name so you can use the normal respective folder
			auto pos = fName.find("crc:/");
			if (pos != std::string::npos)
				fName.erase(pos, 4);

			pos = fName.find(":/");
			if (pos != std::string::npos)
				fName.erase(pos, 1);

			cwd /= fName;

			cwd.make_preferred();

			return cwd.string();
		}
	};
}