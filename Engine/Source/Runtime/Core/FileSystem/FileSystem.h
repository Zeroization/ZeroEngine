#pragma once

#include "pch.h"
#include <filesystem>
#include <initializer_list>

namespace ZeroEngine
{
    enum class FileSysResult
    {
        Success = 0,
        ErrInvalid,
        ErrLocked,
        ErrOthers
    };

    /// 文件对话框用到的过滤器
    struct FileDialogFilterItem
    {
        const char* Name;
        const char* Spec;
    };

    class FileSystem
    {
    public:
        /// 获取当前程序的运行目录
        static std::filesystem::path GetCurExeFilePath();
        /// 获取引擎的工作目录
        static std::filesystem::path GetWorkingDir();
        /// 设置引擎的工作目录
        static void SetWorkingDir(const std::filesystem::path& path);
        static void SetWorkingDir(std::string_view path);
        /// 创建新目录
        static bool CreateDir(const std::filesystem::path& dir);
        static bool CreateDir(std::string_view dir);
        /// 检查文件是否存在
        static bool Exists(const std::filesystem::path& filepath);
        static bool Exists(std::string_view filepath);
        /// 检查路径是否为文件夹
        static bool IsDirectory(const std::filesystem::path& filepath);
        static bool IsDirectory(std::string_view filepath);
        /// 如果文件A的最后编辑时间比文件B的更新, 就返回true
        static bool IsNewer(const std::filesystem::path& filepathA, const std::filesystem::path& filepathB);

        /// 删除指定路径的文件, 如果是文件夹就全部删除
        static bool FileDelete(const std::filesystem::path& filepath);
        /// 移动给定路径的文件到目标路径
        static bool FileMove(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
        /// 移动给定路径的文件到目标文件夹中
        static bool FileMoveToDir(const std::filesystem::path& filepath, const std::filesystem::path& destDirPath);
        /// 复制给定路径的文件到目标路径
        static bool FileCopy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
        /// 复制给定路径的文件到目标文件夹中
        static bool FileCopyToDir(const std::filesystem::path& filepath, const std::filesystem::path& destDirPath);
        /// 文件重命名操作
        static bool FileRename(const std::filesystem::path& oldFilepath, std::string_view newName);
        /// 尝试打开文件
        static FileSysResult FileTryOpen(const std::filesystem::path& filepath);
        /// 尝试打开文件, 失败后会在给定时间后再尝试打开一次
        static FileSysResult FileTryOpenAndWait(const std::filesystem::path& filepath, uint64_t ms = 100);
        /// 写文件(二进制, 默认为覆盖模式)
        static bool WriteBytes(const std::filesystem::path& filepath, const std::vector<unsigned char>& data,
                               bool appendWrite = false);
        /// 写文件(文本, 默认为覆盖模式)
        static bool WriteText(const std::filesystem::path& filepath, const std::string& content,
                              bool appendWrite = false);
        /// 读文件(二进制), 返回结果在传参中
        static bool ReadBytes(const std::filesystem::path& filepath, std::vector<unsigned char>& data);
        /// 读文件(文本)
        static bool ReadText(const std::filesystem::path& filepath, std::string& content);
        /// 获取文件的唯一名字, 例如出现重命名情况, 返回A(1).txt
        static std::filesystem::path GetUniqueFileName(const std::filesystem::path& filepath);
        /// 获取文件的上次编辑时间
        static uint64_t GetLastWriteTime(const std::filesystem::path& filepath);

        /// 在资源管理器处打开
        static bool ShowFileInExplorer(const std::filesystem::path& path);
        static bool ShowDirInExplorer(const std::filesystem::path& path);
        static bool OpenFileExternally(const std::filesystem::path& path);

        /// 打开文件对话框原生窗口(nfde库)
        static std::filesystem::path OpenFileDialog(std::initializer_list<FileDialogFilterItem> filters = {});
        static std::filesystem::path OpenDirDialog(const std::filesystem::path& initialDirPath);
        static std::filesystem::path OpenSaveFileDialog(std::initializer_list<FileDialogFilterItem> filters = {});

        /// 获取持久性存储路径(Win: AppData/Roaming)
        static std::filesystem::path GetPersistentStoragePath();

        /// Windows专用, std::wstring <-> std::string (UTF-8)
        static std::string WStringToUTF8(const std::wstring& wstr);
        static std::wstring UTF8ToWString(const std::string& str);

    private:
        static std::filesystem::path PersistentStoragePath;
    };
} // ZeroEngine
