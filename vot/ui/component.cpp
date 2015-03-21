#include "component.h"

namespace vot
{
    namespace ui
    {
        Component::Component() :
            _has_focus(false),
            _state(NONE)
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

    }
}