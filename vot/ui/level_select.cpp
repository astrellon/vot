#include "level_select.h"

#include <iostream>

#include "button.h"
#include "manager.h"
#include "main_menu.h"

#include <vot/texture_manager.h>
#include <vot/game_system.h>
#include <vot/levels/level.h>

namespace vot
{
    namespace ui
    {
        std::vector<Button *> LevelSelect::s_level_buttons;
        Button *LevelSelect::s_back_button = nullptr;
        bool LevelSelect::s_visible = false;

        bool LevelSelect::init()
        {
            auto idle_texture = TextureManager::texture("button_idle");

            s_back_button = new Button("Back");
            s_back_button->texture(*idle_texture);
            s_back_button->setPosition(300, 200);
            Manager::add_component(s_back_button);
            s_back_button->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                LevelSelect::visible(false);
                MainMenu::visible(true);
            });

            auto levels = LevelManager::levels();
            auto i = 0u;
            for (auto iter = levels->begin(); iter != levels->end(); ++iter)
            {
                auto btn = new Button(iter->second->name());
                btn->texture(*idle_texture);
                btn->setPosition(300, 260 + (60* i));
                Manager::add_component(btn);
                s_level_buttons.push_back(btn);

                if (i == 0u)
                {
                    btn->to_above(s_back_button);
                }
                else
                {
                    btn->to_above(s_level_buttons[i - 1]);
                }
                
                if (i == levels->size() - 1)
                {
                    btn->to_below(s_back_button);
                }
                if (i > 0)
                {
                    s_level_buttons[i - 1]->to_below(btn);
                }
                i++;

                btn->on_click([iter] (int32_t x, int32_t y, sf::Mouse::Button btn)
                {
                    GameSystem::start_game(iter->first);
                    LevelSelect::visible(false);
                });
            }

            s_back_button->to_below(s_level_buttons.front());
            s_back_button->to_above(s_level_buttons.back());

            visible(false);
            return true;
        }

        void LevelSelect::deinit()
        {

        }

        void LevelSelect::visible(bool value)
        {
            s_visible = value;
            s_back_button->enabled(value);

            for (auto i = 0u; i < s_level_buttons.size(); i++)
            {
                s_level_buttons[i]->enabled(value);
            }

            if (value)
            {
                ui::Manager::focus(s_back_button);
            }
            else
            {
                ui::Manager::focus(nullptr);
            }
        }
        bool LevelSelect::visible()
        {
            return s_visible;
        }

        void LevelSelect::on_resize( uint32_t width, uint32_t height )
        {
            auto size = s_back_button->texture()->getSize();
            auto x = (static_cast<float>(width) - size.x) * 0.5f;

            s_back_button->setPosition(x, s_back_button->getPosition().y);
            for (auto i = 0u; i < s_level_buttons.size(); i++)
            {
                auto btn = s_level_buttons[i];
                btn->setPosition(x, btn->getPosition().y);
            }
        }
    }
}
