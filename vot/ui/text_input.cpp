#include "text_input.h"

#include <vot/font_manager.h>
#include <vot/utils/utils.h>

namespace vot
{
    namespace ui
    {
        TextInput::TextInput(const std::string &label, float width, float height) :
            _label(label),
            _width(width),
            _height(height),
            _outline(nullptr)
        {
            auto font = FontManager::font("sans");
            _label_graphic.setFont(*font);
            _label_graphic.setCharacterSize(16);
            _label_graphic.setColor(sf::Color::White);
            this->label(label);
            
            _colour.hsv(25.0f, 1.0f, 1.0f);
        }

        void TextInput::label(const std::string &value)
        {
            _label = value;
            _label_graphic.setString(value);

            update_label_position();
        }

        float TextInput::width() const
        {
            return _width;
        }
        float TextInput::height() const
        {
            return _height;
        }

        void TextInput::update(float dt)
        {
            auto dsaturation = (has_focus() ? 4.0f : -3.0f) * dt;

            auto new_saturation = utils::Utils::clamp(_colour.saturation() + dsaturation, 0.0f, 0.7f);
            _colour.saturation(new_saturation);
            _colour.calc_rgb();

            /*
            if (_outline == nullptr)
            {
                _outline = new sf::RectangleShape();
                _outline->setSize(sf::Vector2f(_width, _height));
                _outline->setFillColor(sf::Color::Transparent);
                _outline->setOutlineThickness(1.0f);
                _outline->setPosition(0, 0);
            }
            _outline->setOutlineColor(_colour);
            */
        }
        void TextInput::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            states.transform *= getTransform();

            target.draw(_label_graphic, states);
            //target.draw(*_outline.get(), states);
        }

        bool TextInput::check_hover( int32_t x, int32_t y ) const
        {
            auto local_pos = getInverseTransform().transformPoint(x, y);

            return local_pos.x >= 0.0f && local_pos.x <= _width &&
                local_pos.y >= 0.0f && local_pos.y <= _height;
        }
        void TextInput::do_click( int32_t x, int32_t y, sf::Mouse::Button button )
        {
            auto handler = on_click();
            if (handler)
            {
                handler(x, y, button);
            }
        }

        void TextInput::update_label_position()
        {
            auto text_size = _label_graphic.getLocalBounds();

            auto top = utils::Utils::round((_height - text_size.height) * 0.5f - text_size.top);

            _label_graphic.setPosition(top, top);
        }
    }
}
