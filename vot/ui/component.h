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

                void has_focus(bool focus);
                bool has_focus() const;
            
                virtual void update(float dt) = 0;

            private:
                State _state;
                bool _has_focus;

        };
    }
}