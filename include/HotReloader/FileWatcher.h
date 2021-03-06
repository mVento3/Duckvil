#pragma once

#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <vector>

namespace Duckvil { namespace HotReloader {

    class FileWatcher
    {
    public:
        enum FileStatus
        {
            FileStatus_Created,
            FileStatus_Modified,
            FileStatus_Removed
        };

        typedef void (*ActionCallback)(const std::filesystem::path& _file, FileStatus _status, void* _pUserData);

    private:
        std::unordered_map<std::string, std::filesystem::file_time_type> m_aPathsToWatch;
        ActionCallback m_fnAction;
        std::vector<std::filesystem::path> m_aPaths;
        void* m_pUserData;

    public:
        FileWatcher(FileWatcher::ActionCallback _action, void* _pUserData);
        ~FileWatcher();

        void Watch(const std::filesystem::path& _path);
        void Update();
    };

}}