#include "manager.h"

#include <iostream>

#include <vot/game_system.h>

#include "main_menu.h"
#include "level_select.h"
#include "profile_select.h"
#include "profile_widget.h"
#include "ship_hanger.h"

namespace vot
{
    namespace ui
    {
        Manager::ComponentList Manager::s_components;
        Component *Manager::s_has_focus = nullptr;
        Component *Manager::s_last_had_focus = nullptr;

        bool Manager::init()
        {
            if (!MainMenu::init())
            {
                return false;
            }
            if (!ProfileSelect::init())
            {
                return false;
            }
            if (!LevelSelect::init())
            {
                return false;
            }
            if (!ProfileWidget::init())
            {
                return false;
            }
            if (!ShipHanger::init())
            {
                return false;
            }

            auto size = GameSystem::window_size();
            MainMenu::on_resize(size.x, size.y);
            ProfileSelect::on_resize(size.x, size.y);
            LevelSelect::on_resize(size.x, size.y);
            ProfileWidget::on_resize(size.x, size.y);
            ShipHanger::on_resize(size.x, size.y);

            MainMenu::visible(false);
            ProfileSelect::visible(false);
            LevelSelect::visible(false);
            ProfileWidget::visible(false);
            ShipHanger::visible(false);

            return true;
        }
        void Manager::deinit()
        {
            LevelSelect::deinit();
            ProfileSelect::deinit();
            MainMenu::deinit();
            ProfileWidget::deinit();
            ShipHanger::deinit();
            
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
            if (s_has_focus != nullptr)
            {
                s_has_focus->has_focus(false);
            }
            s_has_focus = component;
            if (s_has_focus != nullptr)
            {
                s_has_focus->has_focus(true);
            }
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
            ProfileWidget::update(dt);
        }
        void Manager::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            target.setView(GameSystem::hud_camera());

            ProfileWidget::draw(target, states);
            ShipHanger::draw(target, states);
            
            target.setView(GameSystem::hud_camera());
            for (auto i = 0u; i < s_components.size(); i++)
            {
                auto comp = s_components[i].get();
                if (comp->enabled())
                {
                    if (comp->local_view() != nullptr)
                    {
                        target.setView(*comp->local_view());
                    }
                    target.draw(*comp, states);
                    if (comp->local_view() != nullptr)
                    {
                        target.setView(GameSystem::hud_camera());
                    }
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
            else if (event.type == sf::Event::TextEntered)
            {
                if (s_has_focus != nullptr && s_has_focus->enabled())
                {
                    s_has_focus->do_text(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                auto back = event.key.code == sf::Keyboard::W ||
                    event.key.code == sf::Keyboard::Up;
                auto forward = event.key.code == sf::Keyboard::S ||
                    event.key.code == sf::Keyboard::Down ||
                    event.key.code == sf::Keyboard::Tab;
                auto left = event.key.code == sf::Keyboard::A ||
                    event.key.code == sf::Keyboard::Left;
                auto right = event.key.code == sf::Keyboard::D ||
                    event.key.code == sf::Keyboard::Right;

                if (s_has_focus != nullptr && s_has_focus->enabled())
                {
                    if (s_has_focus->do_keypress(event.key.code))
                    {
                        if (back && s_has_focus->to_above() != nullptr)
                        {
                            change_focus(s_has_focus->to_above());
                        }
                        else if (forward && s_has_focus->to_below() != nullptr)
                        {
                            change_focus(s_has_focus->to_below());
                        }
                        else if (left && s_has_focus->to_left())
                        {
                            change_focus(s_has_focus->to_left());
                        }
                        else if (right && s_has_focus->to_right())
                        {
                            change_focus(s_has_focus->to_right());
                        }
                        else if (event.key.code == sf::Keyboard::Space ||
                                event.key.code == sf::Keyboard::Return) 
                        {
                            check_pressed(0, 0, sf::Mouse::Left);
                        }
                    }
                }
                else
                {
                    if (back || forward || left || right)
                    {
                        if (s_last_had_focus == nullptr || !s_last_had_focus->enabled())
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
            else if (event.type == sf::Event::Resized)
            {
                MainMenu::on_resize(event.size.width, event.size.height);
                LevelSelect::on_resize(event.size.width, event.size.height);
                ProfileSelect::on_resize(event.size.width, event.size.height);
                ProfileWidget::on_resize(event.size.width, event.size.height);
                ShipHanger::on_resize(event.size.width, event.size.height);
            }
        }

        void Manager::check_hover(int32_t x, int32_t y)
        {
            auto hovered = false;
            const auto window = GameSystem::window();
            auto global = window->mapPixelToCoords(sf::Vector2i(x, y));
            
            for (auto i = 0u; i < s_components.size(); i++)
            {
                auto comp = s_components[i].get();
                if (comp->enabled())
                {
                    if (comp->local_view() != nullptr)
                    {
                        auto local = window->mapPixelToCoords(sf::Vector2i(x, y), *comp->local_view());
                        if (!comp->check_hover(local.x, local.y))
                        {
                            continue;
                        }
                    }
                    else if (!comp->check_hover(global.x, global.y))
                    {
                        continue;
                    }

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
