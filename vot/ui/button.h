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

                void label(const std::string &label);
                const std::string &label() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            private:
                std::string _label;
                sf::Text _label_graphic;
                sf::Sprite _sprite;

                void update_label_position();
        };
    }
}
