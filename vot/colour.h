#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class Colour
    {
        public:
            Colour();
            Colour(const sf::Color &colour);

            operator sf::Color() const;

            void red(float value);
            float red() const;

            void green(float value);
            float green() const;

            void blue(float value);
            float blue() const;

            void rgb(float r, float g, float b);

            void hue(float value);
            float hue() const;

            void saturation(float value);
            float saturation() const;

            void value(float value);
            float value() const;

            void hsv(float h, float s, float v);

            void calc_rgb();
            void calc_hsv();

            void display();

        private:
            float _red;
            float _green;
            float _blue;
            float _hue;
            float _saturation;
            float _value;

            bool _dirty;
    };

}
