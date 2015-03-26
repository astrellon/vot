#include "colour.h"

#include <iostream>

namespace vot
{
    Colour::Colour() :
        _red(0.0f),
        _green(0.0f),
        _blue(0.0f),
        _hue(0.0f),
        _saturation(0.0f),
        _value(0.0f)
    {

    }
    Colour::Colour(const sf::Color &colour)
    {
        _red = static_cast<float>(colour.r) / 255.0f;
        _green = static_cast<float>(colour.g) / 255.0f;
        _blue = static_cast<float>(colour.b) / 255.0f;

        calc_hsv();
    }

    Colour::operator sf::Color() const
    {
        auto red = static_cast<uint8_t>(_red * 255.0f);        
        auto green = static_cast<uint8_t>(_green * 255.0f);        
        auto blue = static_cast<uint8_t>(_blue * 255.0f);        

        return sf::Color(red, green, blue, 255u);
    }

    void Colour::red(float value)
    {
        _red = value;
    }
    float Colour::red() const
    {
        return _red;
    }

    void Colour::green(float value)
    {
        _green = value;
    }
    float Colour::green() const
    {
        return _green;
    }

    void Colour::blue(float value)
    {
        _blue = value;
    }
    float Colour::blue() const
    {
        return _blue;
    }

    void Colour::hue(float value)
    {
        _hue = value;
    }
    float Colour::hue() const
    {
        return _hue;
    }

    void Colour::saturation(float value)
    {
        _saturation = value;
    }
    float Colour::saturation() const
    {
        return _saturation;
    }

    void Colour::value(float value)
    {
        _value = value;
    }
    float Colour::value() const
    {
        return _value;
    }

    void Colour::rgb(float r, float g, float b)
    {
        _red = r;
        _green = g;
        _blue = b;
    }
    void Colour::hsv(float h, float s, float v)
    {
        _hue = h;
        _saturation = s;
        _value = v;
    }

    void Colour::calc_hsv()
    {
        auto min = _red < _green ? _red : _green;
        min = min < _blue ? min : _blue;

        auto max = _red > _green ? _red : _green;
        max = max > _blue ? max : _blue;

        _value = max;
        auto delta = max - min;
        if (max > 0.0) 
        { 
            _saturation = delta / max;
        } 
        else 
        {
            // if max is 0, then r = g = b = 0              
            // s = 0, v is undefined
            _saturation = 0.0;
            _hue = NAN;
            return;
        }

        if (_red >= max)                           // > is bogus, just keeps compilor happy
        {
            _hue = (_green - _blue) / delta;
        }
        else
        {
            if (_green >= max)
            {
                _hue = 2.0f + (_blue - _red) / delta;  // between cyan & yellow
            }
            else
            {
                _hue = 4.0f + (_red - _green) / delta;  // between magenta & cyan
            }
        }

        // To degree
        _hue *= 60.0;

        if(_hue < 0.0 )
        {
            _hue += 360.0;
        }
    }

    void Colour::calc_rgb()
    {
        if (_saturation <= 0.0) 
        {       // < is bogus, just shuts up warnings
            _red = _value;
            _green = _value;
            _blue = _value;
            return;
        }

        auto hue = _hue;
        if (hue >= 360.0) 
        {
            hue = 0.0;
        }
        hue /= 60.0;
        auto i = static_cast<long>(hue);
        auto ff = hue - i;
        auto p = _value * (1.0f - _saturation);
        auto q = _value * (1.0f - (_saturation * ff));
        auto t = _value * (1.0f - (_saturation * (1.0f - ff)));

        switch (i) 
        {
            case 0:
                _red = _value;
                _green = t;
                _blue = p;
                break;
            case 1:
                _red = q;
                _green = _value;
                _blue = p;
                break;
            case 2:
                _red = p;
                _green = _value;
                _blue = t;
                break;

            case 3:
                _red = p;
                _green = q;
                _blue = _value;
                break;
            case 4:
                _red = t;
                _green = p;
                _blue = _value;
                break;
            case 5:
            default:
                _red = _value;
                _green = p;
                _blue = q;
                break;
        }
    }

    void Colour::display()
    {
        std::cout << "[" << _red << ", " << _green << ", " << _blue << "] | [" << _hue << ", " << _saturation << ", " << _value << "]\n";
    }
}
