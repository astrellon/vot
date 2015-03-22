#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <sstream>
#include <iostream>

#include "vot/game_system.h"
#include "vot/font_manager.h"
#include "vot/texture_manager.h"

int main()
{
    // Init {{{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "VOT", sf::Style::Default);

    if (!vot::FontManager::init())
    {
        std::cout << "Failed to load fonts on startup\n";
        std::cin.get();
        return -1;
    }
    
    if (!vot::TextureManager::init())
    {
        std::cout << "Failed to load textures on startup\n";
        std::cin.get();
        return -1;
    }
    vot::TextureManager::display("Main1");

    vot::GameSystem game_system(window);
    vot::TextureManager::display("Main2");
    vot::GameSystem::main(&game_system);

    vot::TextureManager::display("Main3");
    game_system.init();

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

            if (event.type == sf::Event::KeyPressed)
            {
                game_system.key_pressed(event.key.code);

                if (event.key.code == sf::Keyboard::J)
                {
                    auto texture = vot::TextureManager::texture("bullet_blue_circle");
                    auto system = game_system.particle_manager().spawn_system(*texture, 20);
                    system->setPosition(0, 0);
                    system->system_type(1u);
                    system->init();
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                game_system.key_released(event.key.code);
            }

            if (event.type == sf::Event::Resized)
            {
                game_system.on_resize(event.size.width, event.size.height);
            }
        }
        // }}}
        
        // Game loop {{{
        sf::Time elapsed = clock.restart();
        auto dt = elapsed.asSeconds();

        game_system.update(dt);
        // }}}
        
        // Draw game {{{
        // Clear screen
        window.clear();
       
        window.draw(game_system);

        window.setView(game_system.camera());
        // Update the window
        window.display();
        // }}}
    }
    // }}}

    vot::FontManager::deinit();
    vot::TextureManager::deinit();

    return EXIT_SUCCESS;
}
