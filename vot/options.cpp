#include "options.h"

#include <boost/filesystem.hpp>

#include "profile.h"

#include <utils/data.h>
#include <utils/lua_serialiser.h>

namespace vot
{
    std::string Options::s_starting_profile;

    void Options::save()
    {
        ::utils::Data data(::utils::Data::MAP);
        auto profile = ProfileManager::current_profile();
        if (profile != nullptr)
        {
            data.at("profile", profile->name());
        }

        ::utils::LuaSerialiser::serialise(&data, "config.lua");
    }
    void Options::load()
    {
        if (!boost::filesystem::is_regular_file("config.lua"))
        {
            return;
        }

        std::unique_ptr<::utils::Data> data(::utils::LuaSerialiser::deserialise("config.lua"));
        if (data->has("profile"))
        {
            s_starting_profile = data->at("profile")->string();
        }
    }

    const std::string &Options::starting_profile()
    {
        return s_starting_profile;
    }
}
