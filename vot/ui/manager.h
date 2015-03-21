#pragma once

#include <vector>
#include <memory>

namespace vot
{
    class Component;

    namespace ui
    {
        class Manager 
        {
        public:

            void add_component(Component *component);
            void remove_component(Component *component);

            void focus(Component *component);
            Component *focus() const;

            typedef std::vector<std::unique_ptr<Component> > ComponentList;
            const ComponentList *components() const;

            void update(float dt);

            static void main(Manager *main);
            static Manager *main();

        private:
            ComponentList _components;
            Component *_has_focus;

            static Manager *s_main;
        };
    }
}