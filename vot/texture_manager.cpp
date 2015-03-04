#include "texture_manager.h"

#include <sstream>
#include <iostream>

namespace vot
{
    TextureManager *TextureManager::s_main = nullptr;

    TextureManager::TextureManager() :
        _num_textures(0u),
        _texture_load_attempt(0u)
    {

    }

    const TextureManager::TextureMap &TextureManager::textures() const
    {
        return _textures;
    }

    bool TextureManager::load_texture(const std::string &name, const std::string &filename)
    {
        _texture_load_attempt++;
        if (name.empty())
        {
            return false;
        }

        sf::Texture texture;
        if (!texture.loadFromFile(filename))
        {
            return false;
        }
        texture.setSmooth(true);

        _textures[name] = texture;
        _num_textures++;
        return true;
    }

    const sf::Texture *TextureManager::find_texture(const std::string &name) const
    {
        auto find = _textures.find(name);
        if (find == _textures.end())
        {
            return nullptr;
        }

        return &find->second;
    }

    uint32_t TextureManager::num_textures() const
    {
        return _num_textures;
    }
    uint32_t TextureManager::count_textures() const
    {
        return _textures.size();
    }
    uint32_t TextureManager::texture_load_attempt() const
    {
        return _texture_load_attempt;
    }
    
    bool TextureManager::load_default_textures()
    {
        try
        {
            try_load("bullet_blue", "laserBlue07.png");
            try_load("bullet_blue_circle", "bullet_blue_circle.png");
            try_load("bullet_red_circle", "bullet_red_circle.png");
            try_load("player", "playerShip2_blue.png");
            try_load("red_star", "red_star.png");
            try_load("blue_star", "blue_star.png");
            try_load("enemy", "ufoRed.png");
            try_load("target", "target.png");
            try_load("powerup_bullet", "powerupBlue_bolt.png");
            try_load("powerup_homing", "powerupBlue_star.png");
            try_load("homing_center", "homing_center.png");
            try_load("homing_outer", "homing_outer.png");
            try_load("beam", "beam.png");
            try_load("beam_top", "beam_top.png");
            try_load("turret", "turret.png");
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    void TextureManager::try_load(const std::string &name, const std::string &from_data)
    {
        std::stringstream filename;
        filename << "data/" << from_data;

        if (!load_texture_log(name, filename.str()))
        {
            throw std::runtime_error("Failed to load texture");
        }
    }

    void TextureManager::display(const std::string &message) const
    {
        std::cout << message << " texture counts: " << num_textures() << ", " << count_textures() << ", " << texture_load_attempt() << "\n";
    }

    void TextureManager::main(TextureManager *manager)
    {
        s_main = manager;
    }
    TextureManager *TextureManager::main()
    {
        return s_main;
    }

    const sf::Texture *TextureManager::texture(const std::string &name)
    {
        return s_main->find_texture(name); 
    }

    bool TextureManager::load_texture_log(const std::string &name, const std::string &filename)
    {
        auto result = load_texture(name, filename); 
        if (!result)
        {
            std::cout << "Failed to load " << name << " texture\n";
        }
        return result;
    }
}
