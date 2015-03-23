#include "button.h"

#include <vot/font_manager.h>

namespace vot
{
    namespace ui
    {
        Button::Button(const std::string &label) :
            Component()
        {
            auto font = FontManager::font("sans");
            _label_graphic.setFont(*font);
            this->label(label);
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
            sf::Color colour(255u, 255u, 255u, has_focus() ? 255u : 127u);
            _sprite.setColor(colour);
        }
        void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            states.transform *= getTransform();

            target.draw(_sprite, states);
            target.draw(_label_graphic, states);
        }

        void Button::update_label_position()
        {
            auto texture_size = _sprite.getTexture()->getSize();
            auto text_size = _label_graphic.getLocalBounds();

            auto left = (static_cast<float>(texture_size.x) - text_size.width) * 0.5f;
            auto top = (static_cast<float>(texture_size.y) - text_size.height) * 0.5f;

            _label_graphic.setPosition(left, top);
        }
    }
}
