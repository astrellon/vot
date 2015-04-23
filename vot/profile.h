#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdint.h>

namespace vot
{
    class Hardpoint;

    class Profile
    {
        public:
            Profile(const std::string &name);

            void credits(uint32_t value);
            uint32_t credits() const;

            void points(int32_t value);
            int32_t points() const;
            void add_points(int32_t value);

            const std::string &name() const;

            typedef std::map<std::string, Hardpoint *> HardpointMap;
            const HardpointMap *hardpoints() const;

            void hardpoint(const std::string &placement_name, Hardpoint *hardpoint);
            Hardpoint *hardpoint(const std::string &placement_name) const;

            bool save();
            bool load();

        private:
            uint32_t _credits;
            int32_t _points;
            std::string _name;

            HardpointMap _hardpoints;

    };

    class ProfileManager
    {
        public:
            static bool init();
            static void deinit();

            typedef std::vector<std::unique_ptr<Profile> > Profiles;
            static const Profiles *profiles();

            static Profile *spawn_profile(const std::string &name);
            static Profile *profile(const std::string &name);

            static void current_profile(Profile *profile);
            static Profile *current_profile();

        private:
            static Profiles s_profiles;
            static Profile *s_current_profile;
    };
}
