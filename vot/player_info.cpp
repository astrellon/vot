#include "player_info.h"

namespace vot
{
    PlayerInfo::PlayerInfo() :
        _credits(0.0f),
        _name("Unnamed")
    {

    }

    void PlayerInfo::credits(float value)
    {
        _credits = value < 0.0f ? 0.0f : value;
    }
    float PlayerInfo::credits() const
    {
        return _credits;
    }

    void PlayerInfo::name(const std::string &name)
    {
        _name = name;
    }
    const std::string &PlayerInfo::name() const
    {
        return _name;
    }
}
