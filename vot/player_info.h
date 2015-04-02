#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

namespace vot
{
    class PlayerInfo
    {
        public:
            PlayerInfo(const std::string &name);

            void credits(float value);
            float credits() const;

            const std::string &name() const;
            const std::string &filename() const;

            bool save();
            bool load();

        private:
            uint32_t _credits;
            std::string _name;

    };

    class PlayerInfoManager
    {
        public:
            static bool init();
            static void deinit();

            typedef std::vector<std::unique_ptr<PlayerInfo> > PlayerInfos;
            static const PlayerInfos *player_infos();

            static PlayerInfo *spawn_info(const std::string &name);

            static void current_info(PlayerInfo *info);
            static PlayerInfo *current_info();

        private:
            static PlayerInfos s_infos;
            static PlayerInfo *s_current_info;
    };
}
