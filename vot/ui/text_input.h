#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include <vot/utils/colour.h>
#include "component.h"

namespace vot
{
    namespace ui
    {
        class TextInput : public Component
        {
            public:
                TextInput(const std::string &label, float width, float height);

                void label(const std::string &value);
                const std::string &label() const;

                float width() const;
                float height() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

                virtual bool check_hover(int32_t x, int32_t y) const;
                virtual void do_click(int32_t x, int32_t y, sf::Mouse::Button button);
            private:
                std::string _label;
                float _width;
                float _height;

                sf::Text _label_graphic;
                utils::Colour _colour;
                sf::RectangleShape _outline;

                void update_label_position();
        };
    }
}
