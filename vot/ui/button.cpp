#include "button.h"

namespace vot
{
    namespace ui
    {
        Button::Button(const std::string &label) :
            Component(),
            _label(label)
        {

        }

        void Button::texture(const sf::Texture &texture)
        {
            _sprite.setTexture(texture);
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
        }
    }
}
