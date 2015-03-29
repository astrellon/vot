#pragma once

#include <SFML/Audio.hpp>

#include <string>
#include <map>
#include <memory>

#include "object_manager.h"

namespace vot
{
    class Sound : public sf::Sound
    {
        public:
            Sound();

            void index(uint32_t value);
            uint32_t index() const;

            void time_played_at(float time);
            float time_played_at() const;

        private:
            uint32_t _index;
            float _time_played_at;
    };

    class SoundManager : public ObjectManager<Sound, 32>
    {
        public:

            void clear();
             
            bool load_default_sounds();

            typedef std::map<std::string, std::unique_ptr<sf::SoundBuffer> > SoundBufferMap;
            const SoundBufferMap &sound_buffers();

            bool load_sound_buffer(const std::string &name, const std::string &filename);
            const sf::SoundBuffer *sound_buffer(const std::string &name);

            Sound *spawn_sound(const std::string &name);

            void update(float dt);

        private:

            SoundBufferMap _sound_buffers;

            bool load_sound_log(const std::string &name, const std::string &filename);
            void try_load(const std::string &name, const std::string &from_sound);
    };
}
