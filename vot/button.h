#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace vot
{
    class Button : public sf::Drawable, public sf::Transformable
    {
        public:
            Button(const std::string &label);

            enum State
            {
                NONE,
                PRESSED,
                RELEASED
            };

            State state() const;

            void has_focus(bool focus);
            bool has_focus() const;

            void texture(const sf::Texture &texture);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const std::string _label;

            State _state;
            bool _has_focus;

            sf::Sprite _sprite;
    };
}
