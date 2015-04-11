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
                TextInput(const std::string &label_str);

                void label(const std::string &value);
                const std::string &label() const;

                void value(const std::string &value);
                const std::string &value() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

                virtual bool check_hover(int32_t x, int32_t y) const;
                virtual bool do_click(int32_t x, int32_t y, sf::Mouse::Button button);
                virtual bool do_keypress(sf::Keyboard::Key key);
                virtual bool do_text(uint32_t text);

                virtual sf::Vector2f size() const;

            private:
                std::string _label;
                std::string _value;

                sf::Text _label_graphic;
                utils::Colour _colour;
                sf::Sprite _sprite;
                sf::Text _value_graphic;

                void update_label_position();
        };
    }
}
