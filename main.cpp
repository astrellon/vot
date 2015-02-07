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

    for (auto i = 0u; i < 1000u; i++)
    {
        auto bullet = game_system.bullet_manager().clone_pattern_bullet("bullet_blue_circle");
        bullet->update(0.06f * i);
    }

    vot::Circle circle1(sf::Vector2f(50.0f, 50.0f), 2.0f);
    vot::Circle circle2(sf::Vector2f(100.0f, 50.0f), 26.0f);

    // Create a graphical text to display
    sf::Text text("Hello SFML", *font_manager.font("sans"), 13);

    /*
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("music.flac"))
        return EXIT_FAILURE;
    // Play the music
    music.play();
    */
    
    // Start the game loop
    auto counter = 0;
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            //player->tra.location() += sf::Vector2f(-3.0f, 0.0f);
            player->translate(sf::Vector2f(-3.0f, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            player->translate(sf::Vector2f(3.0f, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            player->translate(sf::Vector2f(0.0f, 3.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            player->translate(sf::Vector2f(0.0f, -3.0f));
        }

        game_system.update(0.05f);
        // Clear screen
        window.clear();
       
        // Draw the sprite
        //window.draw(sprite);
        
        // Draw the string
        window.draw(text);
        //bm.draw(window);
        //game_system.draw(window);
        window.draw(circle1);
        window.draw(circle2);
        //window.draw(game_system);
        game_system.draw(window);
        // Update the window
        window.display();

        counter++;

        std::stringstream ss;
        ss << "Intersecting: " << (circle1.intersects(circle2) ? "true" : "false");
        text.setString(ss.str());
    }
    return EXIT_SUCCESS;
}
