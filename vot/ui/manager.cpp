#include "manager.h"

#include <iostream>

#include <vot/game_system.h>

namespace vot
{
    namespace ui
    {
        Manager::ComponentList Manager::s_components;
        Component *Manager::s_has_focus = nullptr;
        Component *Manager::s_last_had_focus = nullptr;

        bool Manager::init()
        {
            return true;
        }
        void Manager::deinit()
        {
            s_has_focus = nullptr;
            s_last_had_focus = nullptr;
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
            else if (event.type == sf::Event::KeyPressed)
            {
                if (s_has_focus != nullptr)
                {
                    if (event.key.code == sf::Keyboard::Up && s_has_focus->to_above() != nullptr)
                    {
                        change_focus(s_has_focus->to_above());
                    }
                    else if (event.key.code == sf::Keyboard::Down && s_has_focus->to_below() != nullptr)
                    {
                        change_focus(s_has_focus->to_below());
                    }
                    else if (event.key.code == sf::Keyboard::Left && s_has_focus->to_left())
                    {
                        change_focus(s_has_focus->to_left());
                    }
                    else if (event.key.code == sf::Keyboard::Right && s_has_focus->to_right())
                    {
                        change_focus(s_has_focus->to_right());
                    }
                    else if (event.key.code == sf::Keyboard::Return)
                    {
                        check_pressed(0, 0, sf::Mouse::Left);
                    }
                }
                else
                {
                    if (event.key.code == sf::Keyboard::Up || 
                        event.key.code == sf::Keyboard::Down ||
                        event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Right)
                    {
                        if (s_last_had_focus == nullptr)
                        {
                            for (auto i = 0u; i < s_components.size(); i++)
                            {
                                if (s_components[i]->enabled())
                                {
                                    change_focus(s_components[i].get());
                                    break;
                                }
                            }
                        }
                        else
                        {
                            change_focus(s_last_had_focus);
                        }
                    }
                }
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
                    change_focus(comp);
                    hovered = true;
                    break;
                }
            }

            if (!hovered && s_has_focus != nullptr)
            {
                change_focus(nullptr);
            }
        }

        void Manager::check_pressed(int32_t x, int32_t y, sf::Mouse::Button button)
        {
            if (s_has_focus != nullptr)
            {
                s_has_focus->do_click(x, y, button);
            }
        }

        void Manager::change_focus(Component *comp)
        {
            if (s_has_focus != nullptr && s_has_focus != comp)
            {
                s_has_focus->has_focus(false);
            }
            if (comp != nullptr)
            {
                s_last_had_focus = s_has_focus;
            }

            s_has_focus = comp;
            if (comp != nullptr)
            {
                s_has_focus->has_focus(true);
            }
        }
    }
}
