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
#include "vot/particles.h"
#include "vot/utils.h"

int main()
{
    vot::Ray ray(sf::Vector2f(-10.0f, 1.0f), 0.0f);
    vot::Ray ray2(sf::Vector2f(10.0f, 1.0f), 180.0f);
    vot::Ray ray3(sf::Vector2f(0.0f, 10.0f), 270.0f);
    vot::Ray ray4(sf::Vector2f(0.0f, -10.0f), 90.0f);
    vot::Circle circle(sf::Vector2f(0.0f, 0.0f), 6.0f);
    sf::Vector2f positions[2];
    sf::Vector2f normals[2];

    bool hit = vot::Utils::ray_circle_intersect(ray, circle, positions, normals);
    hit = vot::Utils::ray_circle_intersect(ray2, circle, positions, normals);
    hit = vot::Utils::ray_circle_intersect(ray3, circle, positions, normals);
    hit = vot::Utils::ray_circle_intersect(ray4, circle, positions, normals);
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
    texture_manager.display("Main1");
    vot::TextureManager::main()->display("Main2");

    vot::GameSystem game_system(window);
    vot::TextureManager::main()->display("Main3");
    vot::GameSystem::main(&game_system);

    vot::TextureManager::main()->display("Main4");
    game_system.init();

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

    return EXIT_SUCCESS;
}
