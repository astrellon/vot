#include "manager.h"

namespace vot
{
    namespace ui
    {
        Manager::ComponentList Manager::s_components;
        Component *Manager::s_has_focus = nullptr;

        bool Manager::init()
        {
            return true;
        }
        void Manager::deinit()
        {
            s_has_focus = nullptr;
            s_components.clear();
        }

        void Manager::add_component(Component *component)
        {
            s_components.push_back(std::unique_ptr<Component>(component));
        }
        void Manager::remove_component(Component *component)
        {
            for (auto i = 0u; i < s_components.size(); i++)
            {
                if (s_components[i].get() == component)
                {
                    s_components.erase(s_components.begin() + i);
                    break;
                }
            }
        }

        void Manager::focus(Component *component)
        {
            s_has_focus = component;
        }
        Component *Manager::focus()
        {
            return s_has_focus;
        }

        const Manager::ComponentList *Manager::components()
        {
            return &s_components;
        }

        void Manager::update(float dt)
        {
            for (auto i = 0u; i < s_components.size(); i++)
            {
                auto comp = s_components[i].get();
                comp->update(dt);
            }
        }
        void Manager::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            for (auto i = 0u; i < s_components.size(); i++)
            {
                auto comp = s_components[i].get();
                target.draw(*comp, states);
            }
        }
    }
}
