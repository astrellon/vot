#include "component.h"

namespace vot
{
    namespace ui
    {
        Component::Component() :
            _state(NONE),
            _has_focus(false),
            _hover(false),
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
            _has_focus = focus;
        }
        bool Component::has_focus() const
        {
            return _has_focus;
        }

        void Component::hover(bool value)
        {
            _hover = value;
        }
        bool Component::hover() const
        {
            return _hover;
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
    }
}
