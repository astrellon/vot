#pragma once

#include <SFML/Graphics.hpp>

#include "ray.h"

namespace vot
{
    class Beam : public sf::Drawable
    {
    public:
        Beam();

        Ray &hitbox();
        const Ray &hitbox() const;

        void active(bool value);
        bool active() const;

        void max_length(float value);
        float max_length() const;

        void hitting_target_length(float value);
        float hitting_target_length() const;

        void update(float dt);
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        Ray _hitbox;
        bool _active;
        float _max_length;
        float _hitting_target_length;

        sf::RectangleShape _shape;
    };
}