#include "manager.h"

#include <iostream>

#include <vot/game_system.h>

namespace vot
{
    namespace ui
    {
        Manager::ComponentList Manager::s_components;
        Component *Manager::s_has_focus = nullptr;
        Component *Manager::s_has_hover = nullptr;

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

        Component *Manager::has_hover()
        {
            return s_has_hover;
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
            target.setView(GameSystem::hud_camera());
            for (auto i = 0u; i < s_components.size(); i++)
            {
                auto comp = s_components[i].get();
                if (comp->enabled())
                {
                    target.draw(*comp, states);
                }
            }
        }

        void Manager::process_event(const sf::Event &event)
        {
            if (event.type == sf::Event::MouseMoved)
            {
                check_hover(event.mouseMove.x, event.mouseMove.y);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                check_pressed(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            }
        }

        void Manager::check_hover(int32_t x, int32_t y)
        {
            auto hovered = false;
            for (auto i = 0u; i < s_components.size(); i++)
            {
                auto comp = s_components[i].get();
                if (comp->enabled() && comp->check_hover(x, y))
                {
                    if (comp != s_has_hover && s_has_hover != nullptr)
                    {
                        s_has_hover->hover(false);
                    }
                    comp->hover(true);
                    s_has_hover = comp;
                    hovered = true;
                    break;
                }
            }

            if (!hovered && s_has_hover != nullptr)
            {
                s_has_hover->hover(false);
                s_has_hover = nullptr;
            }

            if (s_has_hover != nullptr)
            {
                //std::cout << "Hovering over: " << s_has_hover->id() << "\n";
            }
            else
            {
                //std::cout << "No hovering\n";
            }
        }

        void Manager::check_pressed(int32_t x, int32_t y, sf::Mouse::Button button)
        {
            if (s_has_hover != nullptr)
            {
                s_has_hover->do_click(x, y, button);
            }
        }
    }
}
