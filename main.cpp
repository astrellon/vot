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

    if (!vot::ui::Manager::init())
    {
        std::cout << "Failed to initialise UI\n";
        std::cin.get();
        return -1;
    }

    if (!vot::GameSystem::init(window))
    {
        std::cout << "Failed to initialise Game system\n";
        std::cin.get();
        return -1;
    }

    vot::TextureManager::display("Main3");

    auto game = new vot::Game();
    vot::GameSystem::game(game);

    auto button = new vot::ui::Button("Hello");
    button->texture(*vot::TextureManager::texture("button_idle"));
    button->id("btn1");
    vot::ui::Manager::add_component(button);
    button->setPosition(300, 200);

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
       
        //window.draw(game_system);
        //vot::GameSystem::draw(window, )
        sf::RenderStates states;
        vot::GameSystem::draw(window, states);
        vot::ui::Manager::draw(window, states);

        //window.setView(vot::GameSystem::camera());
        // Update the window
        window.display();
        // }}}
    }
    // }}}

    vot::GameSystem::deinit();
    vot::ui::Manager::deinit();
    vot::FontManager::deinit();
    vot::TextureManager::deinit();

    return EXIT_SUCCESS;
}
