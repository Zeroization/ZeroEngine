#include "FileSystem.h"
#include "nfd.hpp"
#include <fstream>

#ifdef ZERO_OS_WINDOWS
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>
#include <ShlObj.h>
# elif defined(ZERO_OS_LINUX)
#include <errno.h>
#endif

namespace ZeroEngine
{
    std::filesystem::path FileSystem::GetCurExeFilePath()
    {
#ifdef ZERO_PLATFORM_DESKTOP
#   ifdef ZERO_OS_WINDOWS
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
#   elif defined(ZERO_OS_LINUX)
        // TODO
#   endif
        std::filesystem::path path(buffer);
        if (IsDirectory(path))
        {
            return path;
        }
        return path.parent_path();
#else
        return "TODO"
#endif
    }

    std::filesystem::path FileSystem::GetWorkingDir()
    {
        return std::filesystem::current_path();
    }

    void FileSystem::SetWorkingDir(const std::filesystem::path& path)
    {
        std::filesystem::current_path(path);
    }

    void FileSystem::SetWorkingDir(std::string_view path)
    {
        SetWorkingDir(std::filesystem::path(path));
    }

    bool FileSystem::CreateDir(const std::filesystem::path& dir)
    {
        return std::filesystem::create_directories(dir);
    }

    bool FileSystem::CreateDir(std::string_view dir)
    {
        return CreateDir(std::filesystem::path(dir));
    }

    bool FileSystem::Exists(const std::filesystem::path& filepath)
    {
        return std::filesystem::exists(filepath);
    }

    bool FileSystem::Exists(std::string_view filepath)
    {
        return Exists(std::filesystem::path(filepath));
    }

    bool FileSystem::IsDirectory(const std::filesystem::path& filepath)
    {
        return std::filesystem::is_directory(filepath);
    }

    bool FileSystem::IsDirectory(std::string_view filepath)
    {
        return IsDirectory(std::filesystem::path(filepath));
    }

    bool FileSystem::IsNewer(const std::filesystem::path& filepathA, const std::filesystem::path& filepathB)
    {
        return std::filesystem::last_write_time(filepathA) > std::filesystem::last_write_time(filepathB);
    }

    bool FileSystem::FileDelete(const std::filesystem::path& filepath)
    {
        if (!Exists(filepath))
        {
            return false;
        }

        if (IsDirectory(filepath))
        {
            return std::filesystem::remove_all(filepath) > 0;
        }
        return std::filesystem::remove(filepath);
    }

    bool FileSystem::FileMove(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
    {
        if (Exists(newFilepath))
        {
            return false;
        }

        std::filesystem::rename(oldFilepath, newFilepath);
        return true;
    }

    bool FileSystem::FileMoveToDir(const std::filesystem::path& filepath, const std::filesystem::path& destDirPath)
    {
        return FileMove(filepath, destDirPath / filepath.filename());
    }

    bool FileSystem::FileCopy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
    {
        if (Exists(newFilepath))
        {
            return false;
        }

        std::filesystem::copy(oldFilepath, newFilepath);
        return true;
    }

    bool FileSystem::FileCopyToDir(const std::filesystem::path& filepath, const std::filesystem::path& destDirPath)
    {
        return FileCopy(filepath, destDirPath / filepath.filename());
    }

    bool FileSystem::FileRename(const std::filesystem::path& oldFilepath, std::string_view newName)
    {
        std::filesystem::path newPath = oldFilepath.parent_path() / std::filesystem::path(
                                            std::string(newName) + oldFilepath.extension().string());
        return FileMove(oldFilepath, newPath);
    }

    FileSysResult FileSystem::FileTryOpen(const std::filesystem::path& filepath)
    {
#ifdef ZERO_OS_WINDOWS
        HANDLE fileHandle = CreateFile(reinterpret_cast<LPCSTR>(filepath.c_str()), GENERIC_READ, 0, nullptr,
                                       OPEN_EXISTING, 0, nullptr);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            DWORD errorCode = GetLastError();
            if (errorCode == ERROR_FILE_NOT_FOUND || errorCode == ERROR_PATH_NOT_FOUND)
            {
                return FileSysResult::ErrInvalid;
            }
            if (errorCode == ERROR_SHARING_VIOLATION)
            {
                return FileSysResult::ErrLocked;
            }
            return FileSysResult::ErrOthers;
        }
        CloseHandle(fileHandle);
# elif defined(ZERO_OS_LINUX)

        // TODO: 实现Linux平台的FileTryOpen()
        LOG_CRITICAL(std::format("[{}] Linux version not implemented!", __FUNCTION__));
        return FileSysResult::ErrOthers;
#endif

        return FileSysResult::Success;
    }

    FileSysResult FileSystem::FileTryOpenAndWait(const std::filesystem::path& filepath, uint64_t ms)
    {
        FileSysResult fileRes = FileTryOpen(filepath);
        if (fileRes == FileSysResult::ErrLocked)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(operator ""ms(ms));
            return FileTryOpen(filepath);
        }
        return fileRes;
    }

    bool FileSystem::ShowFileInExplorer(const std::filesystem::path& path)
    {
        std::filesystem::path absolutePath = std::filesystem::canonical(path);
        if (!Exists(absolutePath))
        {
            return false;
        }

#ifdef ZERO_OS_WINDOWS
        std::string cmd = std::format("explorer.exe /select,\"{}\"", absolutePath.string());
#elif defined(ZERO_OS_LINUX)

        std::string cmd = std::format("xdg-open \"{}\"", dirname(absolutePath.string().data()));
#endif

        system(cmd.c_str());
        return true;
    }

    bool FileSystem::ShowDirInExplorer(const std::filesystem::path& path)
    {
#ifdef ZERO_OS_WINDOWS
        auto absolutePath = std::filesystem::canonical(path);
        if (!Exists(absolutePath))
        {
            return false;
        }

        ShellExecute(nullptr, reinterpret_cast<LPCSTR>(L"explore"),
                     reinterpret_cast<LPCSTR>(absolutePath.c_str()), nullptr,
                     nullptr, SW_SHOWNORMAL);
        return true;
# elif defined(ZERO_OS_LINUX)
        return ShowFileInExplorer(path);
#endif
    }

    bool FileSystem::OpenFileExternally(const std::filesystem::path& path)
    {
        auto absolutePath = std::filesystem::canonical(path);
        if (!Exists(absolutePath))
        {
            return false;
        }
#ifdef ZERO_OS_WINDOWS
        ShellExecute(nullptr, reinterpret_cast<LPCSTR>(L"open"),
                     reinterpret_cast<LPCSTR>(absolutePath.c_str()), nullptr,
                     nullptr, SW_SHOWNORMAL);
# elif defined(ZERO_OS_LINUX)
        std::string cmd = std::format("xdg-open \"{}\"", absolutePath.string().data());
        system(cmd.c_str());
#endif

        return true;
    }

    bool FileSystem::WriteBytes(const std::filesystem::path& filepath, const std::vector<unsigned char>& data,
                                bool appendWrite)
    {
        const auto mode = appendWrite ? (std::ios::binary | std::ios::app) : (std::ios::binary | std::ios::trunc);
        std::ofstream stream(filepath, mode);
        if (!stream || !stream.is_open())
        {
            LOG_ERROR(std::format("[{}] Can't open file: {}", __FUNCTION__, filepath.string()));
            stream.close();
            return false;
        }

        stream.write(reinterpret_cast<const char *>(data.data()),
                     static_cast<long long>(data.size()));
        stream.close();
        return true;
    }

    bool FileSystem::WriteText(const std::filesystem::path& filepath, const std::string& content,
                               bool appendWrite)
    {
        const auto mode = appendWrite ? std::ios::app : std::ios::trunc;
        std::ofstream stream(filepath, mode);
        if (!stream.is_open())
        {
            LOG_ERROR(std::format("[{}] Can't open file: {}", __FUNCTION__, filepath.string()));
            stream.close();
            return false;
        }

        stream << content;
        stream.close();
        return true;
    }

    bool FileSystem::ReadBytes(const std::filesystem::path& filepath, std::vector<unsigned char>& data)
    {
        // ate:在文件末尾开始读取，从文件末尾开始读取的优点是我们可以使用读取位置来确定文件的大小并分配缓冲区
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
        if (!stream || !stream.is_open())
        {
            LOG_ERROR(std::format("[{}] Can't read file: {}", __FUNCTION__, filepath.string()));
            stream.close();
            return false;
        }

        const auto end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        const auto size = end - stream.tellg();
        if (size == 0)
        {
            LOG_ERROR(std::format("[{}] Read nothing from file: {}", __FUNCTION__, filepath.string()));
            stream.close();
            return false;
        }

        data.resize(static_cast<size_t>(size));
        stream.read(reinterpret_cast<char *>(data.data()), size);
        stream.close();

        return true;
    }

    bool FileSystem::ReadText(const std::filesystem::path& filepath, std::string& content)
    {
        std::ifstream stream(filepath);
        if (!stream.is_open())
        {
            LOG_ERROR(std::format("[{}] Can't read file: {}", __FUNCTION__, filepath.string()));
            stream.close();
            return false;
        }

        // 一次性读取全部内容（自动处理换行符）
        content.assign(
            (std::istreambuf_iterator<char>(stream)),
            (std::istreambuf_iterator<char>())
        );
        stream.close();
        return true;
    }

    std::filesystem::path FileSystem::GetUniqueFileName(const std::filesystem::path& filepath)
    {
        // 没有重名文件, 可以直接创建
        if (!Exists(filepath))
        {
            return filepath;
        }

        // 递归生成文件名, 直到没有重复为止
        int counter = 0;
        auto checkID = [&counter, filepath](auto&& checkID) -> std::filesystem::path
        {
            ++counter;
            const std::string counterStr = std::to_string(counter);

            std::string newFileName = std::format("{} ({})", filepath.stem().string(), counterStr);
            if (filepath.has_extension())
            {
                newFileName += filepath.extension().string();
            }

            // 看看是否依旧重名
            if (Exists(filepath.parent_path() / newFileName))
            {
                return checkID(checkID);
            }
            else
            {
                return filepath.parent_path() / newFileName;
            }
        };

        return checkID(checkID);
    }

    uint64_t FileSystem::GetLastWriteTime(const std::filesystem::path& filepath)
    {
        ZERO_CORE_ASSERT(Exists(filepath), std::format("File ({}) should exist.", filepath.string()));

        if (FileTryOpenAndWait(filepath) == FileSysResult::Success)
        {
            const auto lastWriteTime = std::filesystem::last_write_time(filepath);
            return std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();
        }

        LOG_ERROR(std::format("[{}] Can't open file: {}", __FUNCTION__, filepath.string()));
        return 0;
    }

    std::filesystem::path FileSystem::OpenFileDialog(std::initializer_list<FileDialogFilterItem> filters)
    {
        NFD::UniquePath filepath;
        nfdresult_t result = NFD::OpenDialog(filepath,
                                             reinterpret_cast<const nfdfilteritem_t *>(filters.begin()),
                                             filters.size());

        switch (result)
        {
            case NFD_OKAY:
            {
                return filepath.get();
            }
            case NFD_CANCEL:
            {
                return "";
            }
            case NFD_ERROR:
            {
                LOG_ERROR(std::format("[{}] NFD internal error: {}", __FUNCTION__, NFD::GetError()));
                return "";
            }
        }

        return "";
    }

    std::filesystem::path FileSystem::OpenDirDialog(const std::filesystem::path& initialDirPath)
    {
        if (!std::filesystem::is_directory(initialDirPath))
        {
            LOG_ERROR(std::format("[{}] \"{}\" is Not a DIR!", __FUNCTION__, initialDirPath.string()));
            return "";
        }

        NFD::UniquePath filepath;
        nfdresult_t result = NFD::PickFolder(filepath, initialDirPath.string().data());

        switch (result)
        {
            case NFD_OKAY:
            {
                return filepath.get();
            }
            case NFD_CANCEL:
            {
                return "";
            }
            case NFD_ERROR:
            {
                LOG_ERROR(std::format("[{}] NFD internal error: {}", __FUNCTION__, NFD::GetError()));
                return "";
            }
        }

        return "";
    }

    std::filesystem::path FileSystem::OpenSaveFileDialog(std::initializer_list<FileDialogFilterItem> filters)
    {
        NFD::UniquePath filepath;
        nfdresult_t result = NFD::SaveDialog(filepath,
                                             reinterpret_cast<const nfdfilteritem_t *>(filters.begin()),
                                             filters.size());

        switch (result)
        {
            case NFD_OKAY:
            {
                return filepath.get();
            }
            case NFD_CANCEL:
            {
                return "";
            }
            case NFD_ERROR:
            {
                LOG_ERROR(std::format("[{}] NFD internal error: {}", __FUNCTION__, NFD::GetError()));
                return "";
            }
        }

        return "";
    }

    std::filesystem::path FileSystem::PersistentStoragePath;

    std::filesystem::path FileSystem::GetPersistentStoragePath()
    {
#ifdef ZERO_OS_WINDOWS
        if (!PersistentStoragePath.empty())
        {
            return PersistentStoragePath;
        }

        PWSTR roamingFilepath;
        HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &roamingFilepath);
        ZERO_CORE_ASSERT(result == S_OK, "AppData/Roaming should exist.");

        PersistentStoragePath = roamingFilepath;
        PersistentStoragePath /= "ZeroEngine";

        if (!Exists(PersistentStoragePath))
        {
            std::filesystem::create_directory(PersistentStoragePath);
        }
# elif defined(ZERO_OS_LINUX)
        // TODO
#endif

        return PersistentStoragePath;
    }
} // ZeroEngine
