#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "component.h"

namespace vot
{
    namespace ui
    {
        class Button : public Component
        {
            public:
                Button(const std::string &label);

                void texture(const sf::Texture &texture);

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            private:
                const std::string _label;

                sf::Sprite _sprite;
        };
    }
}
