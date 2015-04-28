#include <SFML/Graphics.hpp>

#include <iostream>

#include "vot/game.h"
#include "vot/game_system.h"
#include "vot/font_manager.h"
#include "vot/texture_manager.h"
#include "vot/levels/level.h"
#include "vot/profile.h"
#include "vot/options.h"
#include "vot/ui/manager.h"
#include "vot/ui/ui_state.h"
#include "utils/data.h"
#include "utils/lua_serialiser.h"

int main()
{
    /*
    auto root = new utils::Data(utils::Data::MAP);
    root->at("x", 4.5);
    root->at("y", 2.0);
    root->at("flag", false);

    auto sub = new utils::Data(utils::Data::MAP);
    sub->at("name", "Melli");
    root->at("sub", sub);

    auto sub2 = new utils::Data(utils::Data::ARRAY);
    sub2->push(6.7);
    sub2->push(true);
    sub2->push("hello");
    root->at("sub2", sub2);

    std::ofstream output("testout.lua");
    utils::LuaSerialiser::serialise(root, output);
    */

    std::ifstream input("testout.lua");
    utils::LuaSerialiser::deserialise(input);

    // Init {{{
    vot::Options::load();
    
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

    if (!vot::GameSystem::init(window))
    {
        std::cout << "Failed to initialise Game system\n";
        return -1;
    }

    if (!vot::ProfileManager::init())
    {
        std::cout << "Failed to load player data\n";
        return -1;
    }

    if (!vot::LevelManager::init())
    {
        std::cout << "Failed to load levels\n";
        return -1;
    }
    
    if (!vot::ui::Manager::init())
    {
        std::cout << "Failed to initialise UI\n";
        return -1;
    }

    if (!vot::ui::State::init())
    {
        std::cout << "Failed to initialise main UI\n";
        return -1;
    }

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

    // Deinit {{{
    vot::ui::State::deinit();
    vot::LevelManager::deinit();
    vot::ProfileManager::deinit();
    vot::GameSystem::deinit();
    vot::ui::Manager::deinit();
    vot::FontManager::deinit();
    vot::TextureManager::deinit();

    vot::Options::save();
    // }}}

    return EXIT_SUCCESS;
}
