#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

namespace vot
{
    class Profile
    {
        public:
            Profile(const std::string &name);

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

    class ProfileManager
    {
        public:
            static bool init();
            static void deinit();

            typedef std::vector<std::unique_ptr<Profile> > Profiles;
            static const Profiles *profiles();

            static Profile *spawn_info(const std::string &name);

            static void current_info(Profile *info);
            static Profile *current_info();

        private:
            static Profiles s_infos;
            static Profile *s_current_info;
    };
}
