#include "sound.h"

#include <sstream>
#include <iostream>

#include "utils.h"
#include "game_system.h"

namespace vot
{
    Sound::Sound() :
        _index(Utils::max_uint),
        _time_played_at(0.0f)
    {

    }

    void Sound::index(uint32_t value)
    {
        _index = value;
    }
    uint32_t Sound::index() const
    {
        return _index;
    }

    void Sound::time_played_at(float value)
    {
        _time_played_at = value;
    }
    float Sound::time_played_at() const
    {
        return _time_played_at;
    }

    void SoundManager::clear()
    {
        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto sound = _objects[i].get();
            if (sound != nullptr)
            {
                sound->stop();
            }
        }
        _sound_buffers.clear();
    }

    Sound *SoundManager::spawn_sound(const std::string &name)
    {
        auto find = _sound_buffers.find(name);
        if (find == _sound_buffers.end())
        {
            return nullptr;
        }

        auto current_time = GameSystem::time_since_start();
        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto sound = _objects[i].get();
            if (sound != nullptr && sound->getBuffer() == find->second.get())
            {
                // Prevent the same sound from being played too close to each other.
                auto time_diff = current_time - sound->time_played_at();
                if (time_diff < 0.01f)
                {
                    return sound;
                }
            }
        }


        auto index = find_empty_object();
        if (index == Utils::max_uint)
        {
            return nullptr;
        }

        auto new_sound = new Sound();
        new_sound->setBuffer(*find->second.get());
        new_sound->time_played_at(current_time);
        insert_object(new_sound, index);
        return new_sound;
    }

    const SoundManager::SoundBufferMap &SoundManager::sound_buffers()
    {
        return _sound_buffers;
    }

    bool SoundManager::load_sound_buffer(const std::string &name, const std::string &filename)
    {
        if (name.empty())
        {
            return false;
        }

        auto buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile(filename))
        {
            return false;
        }

        _sound_buffers[name] = std::unique_ptr<sf::SoundBuffer>(buffer);
        return true;
    }
    const sf::SoundBuffer *SoundManager::sound_buffer(const std::string &name)
    {
        auto find = _sound_buffers.find(name);
        if (find == _sound_buffers.end())
        {
            return nullptr;
        }

        return find->second.get();
    }

    bool SoundManager::load_default_sounds()
    {
        try
        {
            try_load("laser1", "laser1.wav");
            try_load("laser2", "laser2.wav");
            try_load("laser3", "laser3.wav");
            try_load("laser4", "laser4.wav");
            try_load("explosion1", "explosion1.wav");
            try_load("explosion2", "explosion2.wav");
            try_load("explosion3", "explosion3.wav");
            try_load("hit1", "hit1.wav");
            try_load("chargeup", "chargeup.wav");
            try_load("select", "select.wav");
            try_load("pickup1", "pickup1.wav");
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    void SoundManager::try_load(const std::string &name, const std::string &from_sound)
    {
        std::string filename("data/sounds/");
        filename += from_sound;

        if (!load_sound_buffer(name, filename))
        {
            throw std::runtime_error("Failed to load sound");
        }
    }

    bool SoundManager::load_sound_log(const std::string &name, const std::string &filename)
    {
        auto result = load_sound_buffer(name, filename);
        if (!result)
        {
            std::cout << "Failed to load " << name << " sound\n";
        }
        return result;
    }

    void SoundManager::update(float dt)
    {
        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto sound = _objects[i].get();
            if (sound != nullptr)
            {
                if (sound->getStatus() != sf::SoundSource::Playing)
                {
                    _objects[i] = nullptr;
                }
            }
        }
    }
}