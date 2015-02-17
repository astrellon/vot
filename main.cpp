#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <sstream>

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

    vot::GameSystem game_system(window);
    vot::GameSystem::main(&game_system);
    
    vot::FontManager font_manager;
    vot::FontManager::main(&font_manager);
    font_manager.load_default_fonts();
    
    vot::TextureManager texture_manager;
    vot::TextureManager::main(&texture_manager);
    texture_manager.load_default_textures();

    game_system.create_default_bullets();
    game_system.create_default_enemieS();

    auto player = new vot::Player(*texture_manager.texture("player"));
    player->hitbox().radius(5.0f);
    game_system.player(player);

    auto enemy = game_system.enemy_manager().spawn_enemy("enemy1");
    enemy->translate(sf::Vector2f(-40.0f, 0.0f));
    enemy = game_system.enemy_manager().spawn_enemy("enemy1");
    enemy->translate(sf::Vector2f(40.0f, 0.0f));

    player->target(enemy);

    auto window_size = window.getSize();
    player->location(sf::Vector2f(0.0f, 100.0f));

    sf::View player_camera;
    player_camera.setSize(window_size.x, window_size.y);

    vot::HudMain hud;
    vot::HudWorld worldHud(player_camera);

    auto background = new vot::Background(0.2, player_camera);
    auto background2 = new vot::Background(0.1, player_camera);
    auto background3 = new vot::Background(0.05, player_camera);

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
        
        player->update(dt);
        player_camera.setCenter(player->location());
        player_camera.setRotation(player->rotation());
        window.setView(player_camera);

        hud.update(dt);
        worldHud.update(dt);
        background->update(dt);
        background2->update(dt);
        background3->update(dt);
        // }}}
        
        // Draw game {{{
        // Clear screen
        window.clear();
       
        window.draw(*background3);
        window.draw(*background2);
        window.draw(*background);
        window.draw(game_system);
        window.draw(worldHud);
        
        window.setView(window.getDefaultView());

        window.draw(hud);
        
        // Update the window
        window.display();
        // }}}
    }

    delete background3;
    delete background2;
    delete background;

    return EXIT_SUCCESS;
}
