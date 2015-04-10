#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include <vot/utils/colour.h>
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
                const sf::Texture *texture() const;

                void label(const std::string &label);
                const std::string &label() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

                virtual bool check_hover(int32_t x, int32_t y) const;
                virtual bool do_click(int32_t x, int32_t y, sf::Mouse::Button button); 

            private:
                std::string _label;
                sf::Text _label_graphic;
                sf::Sprite _sprite;
                utils::Colour _colour;

                void update_label_position();
        };
    }
}
