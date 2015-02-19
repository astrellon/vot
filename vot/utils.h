#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class AnglePair
    {
        public:
            AnglePair(float to_angle, float delta_angle) :
                _to_angle(to_angle),
                _delta_angle(delta_angle)
            {

            }

            inline float to_angle() const
            {
                return _to_angle;
            }
            inline float delta_angle() const
            {
                return _delta_angle;
            }

        private:
            float _to_angle;
            float _delta_angle;
    };

    class Utils
    {
        public:

            static AnglePair calculate_angles(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float orig_angle, float offset_angle); 
    };
}
