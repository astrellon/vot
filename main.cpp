#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <sstream>
#include <iostream>

#include "vot/game_system.h"
#include "vot/font_manager.h"
#include "vot/texture_manager.h"
#include "vot/circle.h"
#include "vot/player.h"
#include "vot/character.h"
#include "vot/background.h"
#include "vot/enemy.h"
#include "vot/game_hud.h"

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window", sf::Style::Default);

    vot::FontManager font_manager;
    vot::FontManager::main(&font_manager);
    if (!font_manager.load_default_fonts())
    {
        std::cout << "Failed to load fonts on startup\n";
        return -1;
    }
    
    vot::TextureManager texture_manager;
    vot::TextureManager::main(&texture_manager);
    if (!texture_manager.load_default_textures())
    {
        std::cout << "Failed to load textures on startup\n";
        return -1;
    }

    vot::GameSystem game_system(window);
    vot::GameSystem::main(&game_system);
    
    game_system.init();

    /*
    auto enemy = game_system.enemy_manager().spawn_enemy("enemy1");
    enemy->translate(sf::Vector2f(-40.0f, 0.0f));
    enemy = game_system.enemy_manager().spawn_enemy("enemy1");
    enemy->translate(sf::Vector2f(40.0f, 0.0f));
    */

    /*
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("music.flac"))
        return EXIT_FAILURE;
    // Play the music
    music.play();
    */
    
    // Start the game loop
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
            }
            if (event.type == sf::Event::KeyReleased)
            {
                game_system.key_released(event.key.code);
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
        
        // Update the window
        window.display();
        // }}}
    }

    return EXIT_SUCCESS;
}
