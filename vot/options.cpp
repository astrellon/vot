#include "options.h"

#include <boost/property_tree/json_parser.hpp>

#include "profile.h"

namespace vot
{
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

    }
}
