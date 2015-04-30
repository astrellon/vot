#pragma once

#include <string>

namespace vot
{
    class Group
    {
        public:
            enum Type
            {
                PLAYER,
                ENEMY,
                NATURE
            };

            static std::string type_name(Type type);
            static Type type_name(const std::string &name);
    };
}
