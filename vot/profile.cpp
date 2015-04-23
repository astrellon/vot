#include "profile.h"

#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "options.h"

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

    bool Profile::save()
    {
        boost::filesystem::path save_path("saves");
        boost::filesystem::create_directory(save_path);

        std::string filename("saves/");
        filename += _name;
        filename += ".save";

        boost::property_tree::ptree output;
        output.add("name", _name);
        output.add("credits", _credits);
        output.add("points", _points);

        boost::property_tree::ptree hardpoints;
        output.add_child("hardpoints", hardpoints);
        for (auto iter = _hardpoints.begin(); iter != _hardpoints.end(); ++iter)
        {
            boost::property_tree::ptree hardpoint;    
        }

        boost::property_tree::write_json(filename, output);

        return true;
    }
    bool Profile::load()
    {
        std::string filename("saves/");
        filename += _name;
        filename += ".save";

        boost::property_tree::ptree tree;
        boost::property_tree::read_json(filename, tree);
        auto name = tree.get<std::string>("name");
        auto credits = tree.get<uint32_t>("credits", 0u);
        auto points = tree.get<int32_t>("points", 0);

        _credits = credits;
        _points = points;

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
                iter->path().extension() == ".save")
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
