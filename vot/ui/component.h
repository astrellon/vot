#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

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
                
                void enabled(bool value);
                bool enabled() const;

                Component *to_left() const;
                void to_left(Component *value);
                Component *to_right() const;
                void to_right(Component *value);
                Component *to_below() const;
                void to_below(Component *value);
                Component *to_above() const;
                void to_above(Component *value);
            
                virtual void update(float dt) = 0;

                virtual bool check_hover(int32_t x, int32_t y) const = 0;
                virtual void do_click(int32_t x, int32_t y, sf::Mouse::Button button) = 0;

                void id(const std::string &value);
                const std::string &id() const;
                
                typedef std::function<void (int32_t, int32_t, sf::Mouse::Button)> ClickHandler;
                void on_click(ClickHandler handler);
                ClickHandler on_click() const;

            private:
                State _state;
                bool _has_focus;
                bool _hover;
                bool _enabled;

                std::string _id;

                Component *_to_left;
                Component *_to_right;
                Component *_to_below;
                Component *_to_above;

                ClickHandler _on_click;
        };
    }
}
