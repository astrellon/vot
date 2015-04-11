#include "main_menu.h"

#include "button.h"
#include "text_input.h"
#include "manager.h"
#include "level_select.h"
#include "profile_select.h"

#include <vot/texture_manager.h>
#include <vot/game_system.h>
#include <vot/levels/level.h>
#include <vot/font_manager.h>

namespace vot
{
    namespace ui
    {
        Button *MainMenu::s_start_game = nullptr;
        Button *MainMenu::s_options = nullptr;
        Button *MainMenu::s_profiles = nullptr;
        Button *MainMenu::s_quit = nullptr;
        MenuHelper MainMenu::s_helper;
        sf::Text MainMenu::s_current_profile;
        sf::Text MainMenu::s_current_profile_label;

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

            s_helper.calc_nearby_components();

            auto sans = FontManager::font("sans");
            s_current_profile.setFont(*sans);
            s_current_profile.setString("asd");
            s_current_profile.setCharacterSize(16);
            s_current_profile.setColor(sf::Color::White);
            s_current_profile.setPosition(300, 230);

            s_current_profile_label.setFont(*sans);
            s_current_profile_label.setString("Current Profile");
            s_current_profile_label.setCharacterSize(16);
            s_current_profile_label.setColor(sf::Color::White);
            s_current_profile_label.setPosition(300, 200);

            auto size = GameSystem::window_size();
            on_resize(size.x, size.y);
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
            return s_helper.visible();
        }

        void MainMenu::on_resize( uint32_t width, uint32_t height )
        {
            auto size = s_start_game->texture()->getSize();
            auto x = utils::Utils::round((static_cast<float>(width) - size.x) * 0.5f);

            set_pos(s_start_game, x);
            set_pos(s_options, x);
            set_pos(s_profiles, x);
            set_pos(s_quit, x);

            auto offset = utils::Utils::round(1.1f * size.x);
            set_pos(&s_current_profile_label, x + offset);
            set_pos(&s_current_profile, x + offset);
        }
        void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            auto profile = ProfileManager::current_profile();
            if (profile == nullptr)
            {
                s_current_profile.setString("- No profile -");
            }
            else
            {
                s_current_profile.setString(profile->name());
            }

            target.draw(s_current_profile_label, states);
            target.draw(s_current_profile, states);
        }

        void MainMenu::set_pos(sf::Transformable *obj, float x)
        {
            obj->setPosition(x, obj->getPosition().y);
        }
    }
}
