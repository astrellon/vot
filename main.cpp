#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <sstream>

#include "vot/game_system.h"
#include "vot/font_manager.h"
#include "vot/texture_manager.h"
#include "vot/circle.h"
#include "vot/player.h"
#include "vot/background.h"

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
    vot::Player *player = new vot::Player(*texture_manager.texture("player"));
    game_system.player(player);

    /*
    auto angle = 0.0f;
    for (auto i = 0u; i < 1000u; i++)
    {
        auto bullet = game_system.bullet_manager().clone_pattern_bullet("bullet_blue_circle");
        bullet->pattern_type(1u);
        sf::Transform trans;
        trans.rotate(angle);
        bullet->init_transform(trans);
        angle += 45.5f;
        bullet->update(-0.01f * i);
    }
    */

    auto window_size = window.getSize();
    player->location(sf::Vector2f(0.0f, 100.0f));

    // Create a graphical text to display
    sf::Text text("ABC", *font_manager.font("sans"), 13);

    sf::RenderTexture render_target;
    render_target.create(32, 32);
    render_target.clear(sf::Color::Transparent);
    render_target.draw(text);
    render_target.setRepeated(true);

    auto repeated_texture = render_target.getTexture();
    sf::Sprite testSprite(repeated_texture);
    testSprite.setTextureRect(sf::IntRect(0, 0, 1024, 1024));

    sf::View player_camera;
    player_camera.setSize(window_size.x, window_size.y);

    vot::Background background(0.2, player_camera);
    vot::Background background2(0.1, player_camera);
    vot::Background background3(0.05, player_camera);

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
                window.close();
        }
        // }}}
        
        // Game loop {{{
        sf::Time elapsed = clock.restart();
        auto dt = elapsed.asSeconds();
        
        player->update(dt);
        player_camera.setCenter(player->location());
        player_camera.setRotation(player->rotation());
        window.setView(player_camera);

        game_system.update(dt);
        background.update(dt);
        background2.update(dt);
        background3.update(dt);
        // }}}
        
        // Draw game {{{
        // Clear screen
        window.clear();
       
        window.draw(background3);
        window.draw(background2);
        window.draw(background);
        game_system.draw(window);
        
        window.setView(window.getDefaultView());
        
        std::stringstream ss;
        ss << "Health: " << player->life();
        text.setString(ss.str());
        
        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
        // }}}
    }
    return EXIT_SUCCESS;
}
