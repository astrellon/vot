#pragma once

#include <vector>
#include <memory>

#include "component.h"

namespace vot
{
    namespace ui
    {
        class Manager 
        {
        public:
            static bool init();
            static void deinit();

            static void add_component(Component *component);
            static void remove_component(Component *component);

            static void focus(Component *component);
            static Component *focus();

            typedef std::vector<std::unique_ptr<Component> > ComponentList;
            static const ComponentList *components();

            static void update(float dt);
            static void draw(sf::RenderTarget &target, sf::RenderStates states);

        private:
            static ComponentList s_components;
            static Component *s_has_focus;

        };
    }
}