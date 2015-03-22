#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    namespace ui
    {
        class Component : public sf::Drawable, public sf::Transformable
        {
            public:
                Component();

                enum State
                {
                    NONE,
                    PRESSED,
                    RELEASED
                };

                State state() const;

                void has_focus(bool value);
                bool has_focus() const;
            
                void hover(bool value);
                bool hover() const;

                Component *to_left() const;
                void to_left(Component *value);
                Component *to_right() const;
                void to_right(Component *value);
                Component *to_below() const;
                void to_below(Component *value);
                Component *to_above() const;
                void to_above(Component *value);
            
                virtual void update(float dt) = 0;

            private:
                State _state;
                bool _has_focus;
                bool _hover;

                Component *_to_left;
                Component *_to_right;
                Component *_to_below;
                Component *_to_above;

        };
    }
}