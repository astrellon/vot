#include "texture_manager.h"

#include <sstream>
#include <iostream>

namespace vot
{
    TextureManager::TextureMap TextureManager::_textures;
    uint32_t TextureManager::_num_textures = 0u;
    uint32_t TextureManager::_texture_load_attempt = 0u;

    bool TextureManager::init()
    {
        return load_default_textures();
    }
    void TextureManager::deinit()
    {
        _textures.clear();
        _num_textures = 0u;
        _texture_load_attempt = 0u;
    }

    const TextureManager::TextureMap &TextureManager::textures()
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

    const sf::Texture *TextureManager::texture(const std::string &name)
    {
        auto find = _textures.find(name);
        if (find == _textures.end())
        {
            return nullptr;
        }

        return &find->second;
    }

    uint32_t TextureManager::num_textures()
    {
        return _num_textures;
    }
    uint32_t TextureManager::count_textures()
    {
        return _textures.size();
    }
    uint32_t TextureManager::texture_load_attempt()
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
            try_load("powerup_beam", "powerupBlue_shield.png");
            try_load("beam", "beam.png");
            try_load("beam_top", "beam_top.png");
            try_load("beam_glow", "beam_glow.png");
            try_load("beam_turret", "beam_turret.png");
            try_load("bullet_turret", "bullet_turret.png");
            try_load("thrust", "fire15.png");
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

    void TextureManager::display(const std::string &message)
    {
        std::cout << message << " texture counts: " << num_textures() << ", " << count_textures() << ", " << texture_load_attempt() << "\n";
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
