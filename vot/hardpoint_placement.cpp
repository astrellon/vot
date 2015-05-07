#include "hardpoint_placement.h"

#include "character.h"
#include "hardpoint.h"

namespace vot
{
    // HardpointPlacement {{{
    HardpointPlacement::HardpointPlacement(const std::string &name) :
        _parent(nullptr),
        _hardpoint(nullptr),
        _min_angle(0.0f),
        _max_angle(360.0f),
        _name(name)
    {

    }
    HardpointPlacement::HardpointPlacement(const std::string &name, float x, float y, float min, float max) :
        _parent(nullptr),
        _hardpoint(nullptr),
        _position(x, y),
        _min_angle(min),
        _max_angle(max),
        _name(name)
    {

    }
    HardpointPlacement::HardpointPlacement(const HardpointPlacement &clone) :
        _parent(nullptr),
        _hardpoint(nullptr),
        _position(clone._position),
        _min_angle(clone._min_angle),
        _max_angle(clone._max_angle),
        _name(clone._name)
    {
    
    }

    void HardpointPlacement::setup(float x, float y, float min, float max)
    {
        position(sf::Vector2f(x, y));
        min_angle(min);
        max_angle(max);
    }

    void HardpointPlacement::parent(Character *parent)
    {
        _parent = parent;
    }
    Character *HardpointPlacement::parent() const
    {
        return _parent;
    }

    void HardpointPlacement::hardpoint(Hardpoint *hardpoint)
    {
        if (_hardpoint != nullptr)
        {
            _hardpoint->parent(nullptr);
        }
        _hardpoint = hardpoint;
        if (hardpoint != nullptr)
        {
            hardpoint->parent(this);
            auto average = (_min_angle + _max_angle) * 0.5f;
            hardpoint->setup(_position.x, _position.y, average, _min_angle, _max_angle);
        }
    }
    Hardpoint *HardpointPlacement::hardpoint() const
    {
        return _hardpoint;
    }

    void HardpointPlacement::position(const sf::Vector2f &position)
    {
        _position = position;
    }
    sf::Vector2f HardpointPlacement::position() const
    {
        return _position;
    }
    
    void HardpointPlacement::max_angle(float value)
    {
        _max_angle = value;
    }
    float HardpointPlacement::max_angle() const
    {
        return _max_angle;
    }
    
    void HardpointPlacement::min_angle(float value)
    {
        _min_angle = value;
    }
    float HardpointPlacement::min_angle() const
    {
        return _min_angle;
    }

    std::string HardpointPlacement::name() const
    {
        return _name;
    }
    // }}}
}
