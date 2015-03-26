#include "button.h"

#include <iostream>

#include <vot/font_manager.h>
#include <vot/utils.h>

namespace vot
{
    namespace ui
    {
        Button::Button(const std::string &label) :
            Component()
        {
            auto font = FontManager::font("sans");
            _label_graphic.setFont(*font);
            _label_graphic.setCharacterSize(16);
            _label_graphic.setColor(sf::Color::Black);
            this->label(label);

            //_colour.rgb(1, 1, 1);
            _colour.hsv(25.0f, 1.0f, 1.0f);

        }

        void Button::texture(const sf::Texture &texture)
        {
            _sprite.setTexture(texture);
            update_label_position();
        }

        void Button::label(const std::string &label)
        {
            _label = label;
            _label_graphic.setString(_label);
            
            update_label_position();
        }
        const std::string &Button::label() const
        {
            return _label;
        }

        void Button::update(float dt)
        {
            auto dsaturation = (hover() ? 4.0f : -3.0f) * dt;

            auto new_saturation = Utils::clamp(_colour.saturation() + dsaturation, 0.0f, 0.7f);
            _colour.saturation(new_saturation);
            _colour.calc_rgb();
            
            _sprite.setColor(_colour);
        }
        void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            states.transform *= getTransform();

            target.draw(_sprite, states);
            target.draw(_label_graphic, states);
                
            /*
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(200.0, 1.0f));
            line.setPosition(-100.0f, y * 20.0f);
            target.draw(line, states);
            */
        }

        bool Button::check_hover(int32_t x, int32_t y) const
        {
            auto size = _sprite.getTexture()->getSize();
            auto local_pos = getInverseTransform().transformPoint(x, y);

            return local_pos.x >= 0.0f && local_pos.x <= size.x &&
                local_pos.y >= 0.0f && local_pos.y <= size.y;
        }

        void Button::do_click(int32_t x, int32_t y, sf::Mouse::Button button)
        {
            auto handler = on_click();
            if (handler)
            {
                handler(x, y, button);
            }
        }

        void Button::update_label_position()
        {
            sf::Vector2u texture_size;
            if (_sprite.getTexture())
            {
                texture_size = _sprite.getTexture()->getSize();
            }
            auto text_size = _label_graphic.getLocalBounds();

            auto left = (static_cast<float>(texture_size.x) - text_size.width) * 0.5f;
            auto top = (static_cast<float>(texture_size.y) - text_size.height) * 0.5f - text_size.top;

            _label_graphic.setPosition(Utils::round(left), Utils::round(top));
        }
    }
}
