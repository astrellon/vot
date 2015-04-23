#include "component.h"

#include <iostream>

#include "manager.h"

#include <vot/game_system.h>
#include <vot/sound.h>

namespace vot
{
    namespace ui
    {
        Component::Component() :
            _state(NONE),
            _has_focus(false),
            _enabled(true),
            _local_view(nullptr),
            _to_left(nullptr),
            _to_right(nullptr),
            _to_below(nullptr),
            _to_above(nullptr)
        {

        }
        
        Component::State Component::state() const
        {
            return _state;
        }

        void Component::has_focus(bool focus)
        {
            if (focus != _has_focus)
            {
                _has_focus = focus;

                if (focus)
                {
                    auto sound = GameSystem::sound_manager()->spawn_sound("select", Sound::UserInterface);
                    sound->play();
                }
            }
        }
        bool Component::has_focus() const
        {
            return _has_focus;
        }

        void Component::enabled(bool value)
        {
            _enabled = value;
            if (!value)
            {
                _has_focus = false;
                if (Manager::focus() == this)
                {
                    Manager::focus(nullptr);
                }
            }
        }
        bool Component::enabled() const
        {
            return _enabled;
        }

        void Component::local_view(sf::View *view)
        {
            _local_view = view;
        }
        sf::View *Component::local_view() const
        {
            return _local_view;
        }

        Component *Component::to_left() const
        {
            return _to_left;
        }
        void Component::to_left(Component *value)
        {
            _to_left = value;
        }

        Component *Component::to_right() const
        {
            return _to_right;
        }
        void Component::to_right(Component *value)
        {
            _to_right = value;
        }

        Component *Component::to_above() const
        {
            return _to_above;
        }
        void Component::to_above(Component *value)
        {
            _to_above = value;
        }

        Component *Component::to_below() const
        {
            return _to_below;
        }
        void Component::to_below(Component *value)
        {
            _to_below = value;
        }

        void Component::id(const std::string &value)
        {
            _id = value;
        }
        const std::string &Component::id() const
        {
            return _id;
        }

        bool Component::check_hover(int32_t x, int32_t y) const
        {
            auto s = size();
            auto local_pos = getInverseTransform().transformPoint(x, y);

            return local_pos.x >= 0.0f && local_pos.x <= s.x &&
                local_pos.y >= 0.0f && local_pos.y <= s.y;
        }
        bool Component::do_click(int32_t x, int32_t y, sf::Mouse::Button button)
        {
            auto handler = on_click();
            if (handler)
            {
                return handler(x, y, button);
            }
            return true;
        }

        void Component::on_click(Component::ClickHandler handler)
        {
            _on_click = handler;
        }
        Component::ClickHandler Component::on_click() const
        {
            return _on_click;
        }

        bool Component::do_keypress(sf::Keyboard::Key key)
        {
            return true;
        }
        bool Component::do_text(uint32_t text)
        {
            return true;
        }
    }
}
