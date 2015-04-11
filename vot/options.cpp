#include "options.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "profile.h"

namespace vot
{
    std::string Options::s_starting_profile;

    void Options::save()
    {
        boost::property_tree::ptree data;
        auto profile = ProfileManager::current_profile();
        if (profile != nullptr)
        {
            data.add("profile", profile->name());
        }

        boost::property_tree::write_json("config.json", data);
    }
    void Options::load()
    {
        boost::property_tree::ptree data;

        if (!boost::filesystem::is_regular_file("config.json"))
        {
            return;
        }

        boost::property_tree::read_json("config.json", data);
        s_starting_profile = data.get<std::string>("profile", "");
    }

    const std::string &Options::starting_profile()
    {
        return s_starting_profile;
    }
}
