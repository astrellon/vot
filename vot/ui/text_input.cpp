#include "text_input.h"

#include <vot/font_manager.h>
#include <vot/utils/utils.h>
#include <vot/texture_manager.h>

namespace vot
{
    namespace ui
    {
        TextInput::TextInput(const std::string &label_str) :
            Component()
        {
            auto back = TextureManager::texture("text_input_back");
            _sprite.setTexture(*back);
            
            auto font = FontManager::font("sans");
            _label_graphic.setFont(*font);
            _label_graphic.setCharacterSize(16);
            _label_graphic.setColor(sf::Color::White);
            
            _value_graphic.setFont(*font);
            _value_graphic.setCharacterSize(16);
            _value_graphic.setColor(sf::Color::White);

            label(label_str);

            _colour.hsv(25.0f, 1.0f, 1.0f);
        }

        void TextInput::label(const std::string &value)
        {
            _label = value;
            _label_graphic.setString(value);

            update_label_position();
        }

        void TextInput::update(float dt)
        {
            auto dsaturation = (has_focus() ? 4.0f : -3.0f) * dt;

            auto new_saturation = utils::Utils::clamp(_colour.saturation() + dsaturation, 0.0f, 0.7f);
            _colour.saturation(new_saturation);
            _colour.calc_rgb();

            _sprite.setColor(_colour);
        }
        void TextInput::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            states.transform *= getTransform();

            target.draw(_label_graphic, states);
            target.draw(_sprite, states);
            target.draw(_value_graphic, states);
        }

        bool TextInput::check_hover( int32_t x, int32_t y ) const
        {
            auto size = _sprite.getTexture()->getSize();
            auto local_pos = getInverseTransform().transformPoint(x, y);

            return local_pos.x >= 0.0f && local_pos.x <= size.x &&
                local_pos.y >= 0.0f && local_pos.y <= size.y;
        }
        bool TextInput::do_click( int32_t x, int32_t y, sf::Mouse::Button button )
        {
            auto handler = on_click();
            if (handler)
            {
                return handler(x, y, button);
            }
            return true;
        }

        bool TextInput::do_keypress(sf::Keyboard::Key key)
        {
            if (key == sf::Keyboard::Tab || 
                key == sf::Keyboard::Up ||
                key == sf::Keyboard::Down ||
                key == sf::Keyboard::Left ||
                key == sf::Keyboard::Right)
            {
                return true;
            }
            return false;
        }
        bool TextInput::do_text(uint32_t text)
        {
            if (text == '\t')
            {
                return true;
            }

            if (text == '\b')
            {
                if (_value.size() > 0u)
                {
                    _value.erase(_value.size() - 1);
                }
            }
            else
            {
                _value += static_cast<char>(text);
            }
            _value_graphic.setString(_value);

            return true;
        }

        void TextInput::update_label_position()
        {
            auto size = _sprite.getTexture()->getSize();
            auto text_size = _label_graphic.getLocalBounds();

            auto top = utils::Utils::round((size.y - text_size.height) * 0.5f - text_size.top);

            _label_graphic.setPosition(top, top);
            _value_graphic.setPosition(80, top);
        }
    }
}
