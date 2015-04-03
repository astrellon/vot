#include "level.h"

#include "level1.h"
#include "level2.h"

namespace vot
{
    // LevelManager {{{
    LevelManager::LevelMap LevelManager::s_levels;

    bool LevelManager::init()
    {
        return create_default_levels();
    }
    void LevelManager::deinit()
    {
        s_levels.clear();
    }

    const LevelManager::LevelMap *LevelManager::levels()
    {
        return &s_levels;
    }

    void LevelManager::level(const std::string &name, Level *level)
    {
        s_levels[name] = std::unique_ptr<Level>(level);
    }
    Level *LevelManager::level(const std::string &name)
    {
        auto find = s_levels.find(name);
        if (find == s_levels.end())
        {
            return nullptr;
        }

        return find->second.get();
    }

    bool LevelManager::create_default_levels()
    {
        auto level1 = new Level1();
        level("level1", level1);
        
        auto level2 = new Level2();
        level("level2", level2);

        return true;
    }
    // }}}
}
