#include "button.h"

namespace vot
{
    Button::Button(const std::string &label) :
        _label(label),
        _state(Button::NONE),
        _has_focus(false)
    {

    }

    Button::State Button::state() const
    {
        return _state;
    }

    void Button::has_focus(bool focus)
    {
        _has_focus = focus;
    }
    bool Button::has_focus() const
    {
        return _has_focus;
    }

    void Button::texture(const sf::Texture &texture)
    {
        _sprite.setTexture(texture);
    }

    void Button::update(float dt)
    {
        sf::Color colour(255u, 255u, 255u, _has_focus ? 255u : 127u);
        _sprite.setColor(colour);
    }
    void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(_sprite, states);
    }
}
