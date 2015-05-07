#include "profile.h"

#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>

#include "options.h"
#include "hardpoint.h"
#include "player.h"

#include <utils/data.h>
#include <utils/lua_serialiser.h>

namespace vot
{
    // Profile {{{
    Profile::Profile(const std::string &name) :
        _credits(0u),
        _points(0u),
        _name(name)
    {

    }

    void Profile::credits(uint32_t value)
    {
        _credits = value;
    }
    uint32_t Profile::credits() const
    {
        return _credits;
    }

    void Profile::points(int32_t value)
    {
        _points = value;
    }
    int32_t Profile::points() const
    {
        return _points;
    }
    void Profile::add_points(int32_t value)
    {
        _points += value;
    }

    const std::string &Profile::name() const
    {
        return _name;
    }

    const Profile::HardpointMap *Profile::hardpoints() const
    {
        return &_hardpoints;
    }
    void Profile::hardpoint(const std::string &placement_name, Hardpoint *hardpoint)
    {
        _hardpoints[placement_name] = hardpoint;
    }
    Hardpoint *Profile::hardpoint(const std::string &placement_name) const
    {
        auto find = _hardpoints.find(placement_name);
        if (find == _hardpoints.end())
        {
            return nullptr;
        }

        return find->second;
    }
    void Profile::clear_hardpoints()
    {
        _hardpoints.clear();
    }

    void Profile::apply_to_player(Player *player) const
    {
        player->clear_placement_hardpoints();

        auto hardpoints = ProfileManager::current_profile()->hardpoints();
        for (auto iter = hardpoints->cbegin(); iter != hardpoints->cend(); ++iter)
        {
            player->add_hardpoint_to_placement(iter->first, iter->second->clone());
        }
    }
    void Profile::apply_from_player(const Player *player)
    {
        auto profile = ProfileManager::current_profile();
        profile->clear_hardpoints();

        auto placements = player->placements();
        for (auto iter = placements->cbegin(); iter != placements->cend(); ++iter)
        {
            if (iter->second->hardpoint() != nullptr)
            {
                profile->hardpoint(iter->first, iter->second->hardpoint()->clone());
            }
        }
    }

    bool Profile::save()
    {
        boost::filesystem::path save_path("saves");
        boost::filesystem::create_directory(save_path);

        std::string filename("saves/");
        filename += _name;
        filename += ".lua";

        ::utils::Data output(::utils::Data::MAP);
        output.at("name", _name);
        output.at("credits", _credits);
        output.at("points", _points);

        auto hardpoints = new ::utils::Data(::utils::Data::MAP);
        output.at("hardpoints", hardpoints);
        for (auto iter = _hardpoints.cbegin(); iter != _hardpoints.cend(); ++iter)
        {
            auto hardpoint_data = new ::utils::Data(::utils::Data::MAP);
            iter->second->serialise(hardpoint_data);
            hardpoints->at(iter->first, hardpoint_data);
        }

        ::utils::LuaSerialiser::serialise(&output, filename);

        return true;
    }
    bool Profile::load()
    {
        std::string filename("saves/");
        filename += _name;
        filename += ".lua";

        ::utils::UData input(::utils::LuaSerialiser::deserialise(filename));

        _credits = input->at("credits")->uint32();
        _points = input->at("points")->int32();

        auto hardpoints = input->at("hardpoints");
        if (hardpoints != nullptr)
        {
            for (auto iter = hardpoints->begin_map(); iter != hardpoints->end_map(); ++iter)
            {
                auto hardpoint = Hardpoint::create_from_data(iter->second.get());
                _hardpoints[iter->first] = hardpoint;
            }
        }

        return true;
    }
    // }}}
    
    // ProfileManager {{{
    ProfileManager::Profiles ProfileManager::s_profiles;
    Profile *ProfileManager::s_current_profile = nullptr;

    bool ProfileManager::init()
    {
        // Load player infos from folder
        boost::filesystem::path save_path("saves");
        if (!boost::filesystem::is_directory(save_path))
        {
            boost::filesystem::create_directory(save_path);
            return true;
        }

        boost::filesystem::directory_iterator end;
        for (boost::filesystem::directory_iterator iter(save_path); iter != end; ++iter)
        {
            if (boost::filesystem::is_regular_file(iter->path()) && 
                iter->path().extension() == ".lua")
            {
                std::cout << "Found save file: " << iter->path().filename() << "\n";
                auto info = spawn_profile(iter->path().stem().string());
                info->load();
            }
        }

        auto starting_profile = Options::starting_profile();
        if (!starting_profile.empty())
        {
            s_current_profile = profile(starting_profile);
        }
        return true;
    }
    void ProfileManager::deinit()
    {
        if (s_current_profile != nullptr)
        {
            s_current_profile->save();
        }
    }

    const ProfileManager::Profiles *ProfileManager::profiles()
    {
        return &s_profiles;
    }

    Profile *ProfileManager::spawn_profile(const std::string &name)
    {
        auto info = new Profile(name);

        s_profiles.push_back(std::unique_ptr<Profile>(info));
        return info;
    }
    Profile *ProfileManager::profile(const std::string &name)
    {
        for (auto i = 0u; i < s_profiles.size(); i++)
        {
            if (s_profiles[i]->name() == name)
            {
                return s_profiles[i].get();
            }
        }

        return nullptr;
    }

    void ProfileManager::current_profile(Profile *info)
    {
        if (s_current_profile != nullptr)
        {
            s_current_profile->save();
        }
        s_current_profile = info;
    }
    Profile *ProfileManager::current_profile()
    {
        return s_current_profile;
    }
    // }}}
}
