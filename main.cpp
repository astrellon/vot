#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <time.h>
#include <sstream>

#include "vot/game_system.h"
#include "vot/font_manager.h"
#include "vot/texture_manager.h"
#include "vot/circle.h"
#include "vot/player.h"

float randf()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

int main()
{
    srand(time_t(NULL));

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window", sf::Style::Default);

    vot::GameSystem game_system(window);
    
    vot::FontManager font_manager;
    vot::FontManager::main(&font_manager);
    font_manager.load_default_fonts();
    
    vot::TextureManager texture_manager;
    vot::TextureManager::main(&texture_manager);
    texture_manager.load_default_textures();

    game_system.create_default_bullets();
    vot::Player *player = new vot::Player(*texture_manager.texture("player"));
    game_system.player(player);

    auto angle = 0.0f;
    for (auto i = 0u; i < 1000u; i++)
    {
        auto bullet = game_system.bullet_manager().clone_pattern_bullet("bullet_blue_circle");
        sf::Transform trans;
        trans.rotate(angle);
        bullet->init_transform(trans);
        angle += 45.5f;
        bullet->update(-0.01f * i);
    }

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
    testSprite.setTextureRect(sf::IntRect(0, 0, window_size.x * 2, window_size.y * 2));

    sf::View player_camera;
    player_camera.setSize(window_size.x, window_size.y);

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
        
        // Keyboard input {{{
        sf::Time elapsed = clock.restart();
        float speed = 450.0f * elapsed.asSeconds();
        float rot_speed = 180.0f * elapsed.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player->translate(sf::Vector2f(-speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player->translate(sf::Vector2f(speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player->translate(sf::Vector2f(0.0f, speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player->translate(sf::Vector2f(0.0f, -speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            player->rotateBy(-rot_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            player->rotateBy(rot_speed);
        }
        // }}}
    
        // Game loop {{{
        player_camera.setCenter(player->location());
        player_camera.setRotation(player->rotation());
        window.setView(player_camera);

        game_system.update(elapsed.asSeconds());
        // }}}
        
        // Draw game {{{
        // Clear screen
        window.clear();
       
        game_system.draw(window);
        window.draw(testSprite);
        
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
