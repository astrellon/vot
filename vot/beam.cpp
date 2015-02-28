#include "beam.h"

namespace vot
{
    Beam::Beam() :
        _active(false),
        _max_length(100.0f),
        _hitting_target_length(-1.0f)
    {

    }

    Ray &Beam::hitbox()
    {
        return _hitbox;
    }
    const Ray &Beam::hitbox() const
    {
        return _hitbox;
    }

    void Beam::active(bool value)
    {
        _active = value;
    }
    bool Beam::active() const
    {
        return _active;
    }

    void Beam::max_length(float value)
    {
        _max_length = value;
    }
    float Beam::max_length() const
    {
        return _max_length;
    }

    void Beam::hitting_target_length(float value)
    {
        _hitting_target_length = value;
    }
    float Beam::hitting_target_length() const
    {
        return _hitting_target_length;
    }

    void Beam::update(float dt)
    {
        auto length = _hitting_target_length < 0.0f ? _max_length : _hitting_target_length;
        _shape.setOrigin(0.0f, 3.0f);
        _shape.setSize(sf::Vector2f(length, 6.0f));
        _shape.setRotation(_hitbox.rotation());
    }

    void Beam::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!_active)
        {
            return;
        }

        target.draw(_shape, states);
    }


}