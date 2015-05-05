#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace vot
{
    class Hardpoint;

    // HardpointPlacement {{{
    class HardpointPlacement
    {
        public:
            HardpointPlacement(const std::string &name);
            HardpointPlacement(const std::string &name, float x, float y, float min, float max);
            HardpointPlacement(const HardpointPlacement &clone);

            void setup(float x, float y, float min, float max);

            void hardpoint(Hardpoint *point);
            Hardpoint *hardpoint() const;

            void position(const sf::Vector2f &position);
            sf::Vector2f position() const;

            void max_angle(float value);
            float max_angle() const;
            
            void min_angle(float value);
            float min_angle() const;

            std::string name() const;

        private:
            Hardpoint *_hardpoint;
            sf::Vector2f _position;
            float _min_angle;
            float _max_angle;

            std::string _name;
    };
    // }}}
}
    
