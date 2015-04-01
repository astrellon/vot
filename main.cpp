#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <sstream>
#include <iostream>

#include "vot/game.h"
#include "vot/game_system.h"
#include "vot/font_manager.h"
#include "vot/texture_manager.h"
#include "vot/ui/manager.h"
#include "vot/ui/button.h"
#include "vot/ui/main_menu.h"
#include "vot/ui/level_select.h"
#include "vot/levels/level.h"

int main()
{
    // Init {{{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "VOT", sf::Style::Default);

    if (!vot::FontManager::init())
    {
        std::cout << "Failed to load fonts on startup\n";
        return -1;
    }
    
    if (!vot::TextureManager::init())
    {
        std::cout << "Failed to load textures on startup\n";
        return -1;
    }

    if (!vot::ui::Manager::init())
    {
        std::cout << "Failed to initialise UI\n";
        return -1;
    }

    if (!vot::GameSystem::init(window))
    {
        std::cout << "Failed to initialise Game system\n";
        return -1;
    }

    if (!vot::ui::MainMenu::init())
    {
        std::cout << "Failed to initialise Main Menu\n";
        return -1;
    }

    if (!vot::LevelManager::init())
    {
        std::cout << "Failed to load levels\n";
        return -1;
    }

    if (!vot::ui::LevelSelect::init())
    {
        std::cout << "Failed to initialise Level Select\n";
        return -1;
    }

    /*
    button->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
    {
        std::cout << "Button clicked: " << x << ", " << y << ", " << btn << "\n";    
    });
    */
    /*
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("music.flac"))
        return EXIT_FAILURE;
    // Play the music
    music.play();
    */
    // }}}
    
    // Game loop {{{
    sf::Clock clock;
    while (window.isOpen())
    {
        // Process events {{{
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            vot::GameSystem::process_event(event);
            vot::ui::Manager::process_event(event);
        }
        // }}}
        
        // Game loop {{{
        sf::Time elapsed = clock.restart();
        auto dt = elapsed.asSeconds();

        vot::GameSystem::update(dt);
        vot::ui::Manager::update(dt);
        // }}}
        
        // Draw game {{{
        // Clear screen
        window.clear();
       
        sf::RenderStates states;
        vot::GameSystem::draw(window, states);
        vot::ui::Manager::draw(window, states);

        // Update the window
        window.display();
        // }}}
    }
    // }}}

    vot::LevelManager::deinit();
    vot::ui::LevelSelect::deinit();
    vot::ui::MainMenu::deinit();
    vot::GameSystem::deinit();
    vot::ui::Manager::deinit();
    vot::FontManager::deinit();
    vot::TextureManager::deinit();

    return EXIT_SUCCESS;
}
