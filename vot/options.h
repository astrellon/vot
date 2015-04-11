#pragma once

#include <string>

namespace vot
{
    class Options
    {
        public:
            static void save();
            static void load();

            static const std::string &starting_profile();

        private:
            static std::string s_starting_profile;
    };
}
