#pragma once

#include <string>
#include <map>
#include <memory>

namespace vot
{
    class Level
    {
        public:
            virtual void init() = 0;
            virtual void deinit() = 0;

            virtual void update(float dt) = 0;
    };

    class LevelManager
    {
        public:
            static bool init();
            static void deinit();

            typedef std::map<std::string, std::unique_ptr<Level> > LevelMap;
            static const LevelMap &levels();

        private:
            static LevelMap s_levels;

            static bool create_default_levels();
    };
}
