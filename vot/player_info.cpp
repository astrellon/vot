#include "player_info.h"

#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

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
        boost::filesystem::path save_path("saves");
        boost::filesystem::create_directory(save_path);

        std::string filename("saves/");
        filename += _name;
        filename += ".save";

        std::ofstream output(filename);
        if (!output)
        {
            return false;
        }

        boost::property_tree::ptree tree;
        tree.add("name", _name);
        tree.add<float>("credits", _credits);

        boost::property_tree::write_json(filename, tree);

        return true;
    }
    bool PlayerInfo::load()
    {
        std::string filename("saves/");
        filename += _name;
        filename += ".save";

        boost::property_tree::ptree tree;
        boost::property_tree::read_json(filename, tree);
        auto name = tree.get<std::string>("name");
        auto credits = tree.get<float>("credits");

        _credits = credits;

        return true;
    }
    // }}}
    
    // PlayerInfoManager {{{
    PlayerInfoManager::PlayerInfos PlayerInfoManager::s_infos;
    PlayerInfo *PlayerInfoManager::s_current_info = nullptr;

    bool PlayerInfoManager::init()
    {
        // Load player infos from folder
        boost::filesystem::path save_path("saves");
        boost::filesystem::directory_iterator end;
        for (boost::filesystem::directory_iterator iter(save_path); iter != end; ++iter)
        {
            if (boost::filesystem::is_regular_file(iter->path()) && 
                iter->path().extension() == ".save")
            {
                std::cout << "Found save file: " << iter->path().filename() << "\n";
                auto info = spawn_info(iter->path().stem().string());
                info->load();
            }
        }
        return true;
    }
    void PlayerInfoManager::deinit()
    {
        if (s_current_info != nullptr)
        {
            s_current_info->save();
        }
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
