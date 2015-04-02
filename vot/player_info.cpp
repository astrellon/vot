#include "player_info.h"

#include <fstream>
#include <iostream>

namespace vot
{
    // PlayerInfo {{{
    PlayerInfo::PlayerInfo(const std::string &name) :
        _credits(0.0f),
        _name(name)
    {

    }

    void PlayerInfo::credits(float value)
    {
        _credits = value < 0.0f ? 0.0f : value;
    }
    float PlayerInfo::credits() const
    {
        return _credits;
    }

    const std::string &PlayerInfo::name() const
    {
        return _name;
    }

    bool PlayerInfo::save()
    {
        auto filename = _name;
        filename += ".player";

        std::ofstream output(filename);
        if (!output)
        {
            return false;
        }

        output << "name: " << _name << "\n";
        output << "credit: " << _credits << "\n";

        return true;
    }
    bool PlayerInfo::load()
    {
        auto filename = _name;
        filename += ".player";

        std::ifstream input(filename);
        if (!input)
        {
            return false;
        }

        while (input)
        {
            std::cout << "Loaded: " << input;
        }
        return true;
    }
    // }}}
    
    // PlayerInfoManager {{{
    PlayerInfoManager::PlayerInfos PlayerInfoManager::s_infos;
    PlayerInfo *PlayerInfoManager::s_current_info = nullptr;

    bool PlayerInfoManager::init()
    {
        // Load player infos from folder
        return true;
    }
    void PlayerInfoManager::deinit()
    {

    }

    const PlayerInfoManager::PlayerInfos *PlayerInfoManager::player_infos()
    {
        return &s_infos;
    }

    PlayerInfo *PlayerInfoManager::spawn_info(const std::string &name)
    {
        auto info = new PlayerInfo(name);

        s_infos.push_back(std::unique_ptr<PlayerInfo>(info));
        return info;
    }

    void PlayerInfoManager::current_info(PlayerInfo *info)
    {
        if (s_current_info != nullptr)
        {
            s_current_info->save();
        }
        s_current_info = info;
    }
    PlayerInfo *PlayerInfoManager::current_info()
    {
        return s_current_info;
    }
    // }}}
}
