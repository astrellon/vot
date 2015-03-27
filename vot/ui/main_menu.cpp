#include "main_menu.h"

#include "button.h"
#include "manager.h"

#include <vot/texture_manager.h>
#include <vot/game_system.h>

namespace vot
{
    namespace ui
    {
        Button *MainMenu::s_start_game = nullptr;
        Button *MainMenu::s_options = nullptr;
        Button *MainMenu::s_quit = nullptr;
        bool MainMenu::s_visible = false;

        bool MainMenu::init()
        {
            auto idle_texture = TextureManager::texture("button_idle");
            
            s_start_game = new vot::ui::Button("Start Game");
            s_start_game->texture(*idle_texture);
            s_start_game->setPosition(300, 200);
            s_start_game->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                GameSystem::start_game();
                MainMenu::visible(false);
            });
            Manager::add_component(s_start_game);

            s_options = new vot::ui::Button("Options");
            s_options->texture(*idle_texture);
            s_options->setPosition(300, 260);
            Manager::add_component(s_options);
            
            s_quit = new vot::ui::Button("Quit");
            s_quit->texture(*idle_texture);
            s_quit->setPosition(300, 320);
            s_quit->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                GameSystem::close_game();
            });
            Manager::add_component(s_quit);
        
            return true;
        }

        void MainMenu::deinit()
        {

        }

        void MainMenu::visible(bool value)
        {
            s_visible = value;
            s_start_game->enabled(value);
            s_options->enabled(value);
            s_quit->enabled(value);
        }
        bool MainMenu::visible()
        {
            return s_visible;
        }

        void MainMenu::on_resize( uint32_t width, uint32_t height )
        {
            auto size = s_start_game->texture()->getSize();
            auto x = (static_cast<float>(width) - size.x) * 0.5f;

            s_start_game->setPosition(x, s_start_game->getPosition().y);
            s_options->setPosition(x, s_options->getPosition().y);
            s_quit->setPosition(x, s_quit->getPosition().y);
        }
    }
}
