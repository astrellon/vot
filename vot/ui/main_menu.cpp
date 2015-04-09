#include "main_menu.h"

#include "button.h"
#include "text_input.h"
#include "manager.h"
#include "level_select.h"
#include "profile_select.h"

#include <vot/texture_manager.h>
#include <vot/game_system.h>
#include <vot/levels/level.h>

namespace vot
{
    namespace ui
    {
        Button *MainMenu::s_start_game = nullptr;
        Button *MainMenu::s_options = nullptr;
        Button *MainMenu::s_profiles = nullptr;
        Button *MainMenu::s_quit = nullptr;
        MenuHelper MainMenu::s_helper;
        TextInput *MainMenu::s_input = nullptr;

        bool MainMenu::init()
        {
            auto idle_texture = TextureManager::texture("button_idle");
            
            s_start_game = new Button("Start Game");
            s_start_game->texture(*idle_texture);
            s_start_game->setPosition(300, 200);
            s_start_game->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                LevelSelect::visible(true);
                MainMenu::visible(false);
                
                return true;
            });
            s_helper.add_component(s_start_game);

            s_options = new Button("Options");
            s_options->texture(*idle_texture);
            s_options->setPosition(300, 260);
            s_helper.add_component(s_options);

            s_profiles = new Button("Profiles");
            s_profiles->texture(*idle_texture);
            s_profiles->setPosition(300, 320);
            s_profiles->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                ProfileSelect::visible(true);
                MainMenu::visible(false);

                return true;
            });
            s_helper.add_component(s_profiles);
            
            s_quit = new Button("Quit");
            s_quit->texture(*idle_texture);
            s_quit->setPosition(300, 380);
            s_quit->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                GameSystem::close_game();
                
                return true;
            });
            s_helper.add_component(s_quit);

            s_input = new TextInput("HURR");
            s_helper.add_component(s_input);

            s_helper.calc_nearby_components();

            return true;
        }

        void MainMenu::deinit()
        {

        }

        void MainMenu::visible(bool value)
        {
            s_helper.visible(value);
        }
        bool MainMenu::visible()
        {
            return s_helper.visible();;
        }

        void MainMenu::on_resize( uint32_t width, uint32_t height )
        {
            auto size = s_start_game->texture()->getSize();
            auto x = (static_cast<float>(width) - size.x) * 0.5f;

            s_start_game->setPosition(x, s_start_game->getPosition().y);
            s_options->setPosition(x, s_options->getPosition().y);
            s_profiles->setPosition(x, s_profiles->getPosition().y);
            s_quit->setPosition(x, s_quit->getPosition().y);
        }
    }
}
