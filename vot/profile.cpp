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
        _credits(0.0f),
        _name(name)
    {

    }

    void Profile::credits(float value)
    {
        _credits = value < 0.0f ? 0.0f : value;
    }
    float Profile::credits() const
    {
        return _credits;
    }

    const std::string &Profile::name() const
    {
        return _name;
    }

    bool Profile::save()
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
    bool Profile::load()
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
    
    // ProfileManager {{{
    ProfileManager::Profiles ProfileManager::s_profiles;
    Profile *ProfileManager::s_current_profile = nullptr;

    bool ProfileManager::init()
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
