#pragma once

#include <stdint.h>
#include <vector>

namespace vot
{
    namespace ui
    {
        class Component;
        class Button;

        class MenuHelper
        {
            public:
                MenuHelper();

                void visible(bool value);
                bool visible() const;

                void back_button(Button *btn);
                Button *back_button() const;

                void add_component(Component *comp);
                void calc_nearby_components();
                
                typedef std::vector<Component *> ComponentList;
                const ComponentList &components() const;

                void on_resize(uint32_t width, uint32_t height);

            private:
                bool _visible;
                ComponentList _components;
                Button *_back_button;
        };
    }
}
