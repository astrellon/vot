#include "common.h"

namespace vot
{
    std::string Group::type_name(Group::Type type)
    {
        if (type == PLAYER)
        {
            return std::string("player");
        }
        if (type == ENEMY)
        {
            return std::string("enemy");
        }
        return std::string("nature");
    }

    Group::Type Group::type_name(const std::string &name)
    {
        if (name == "player")
        {
            return PLAYER;
        }
        if (name == "enemy")
        {
            return ENEMY;
        }
        return NATURE;
    }
}
