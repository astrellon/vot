#pragma once

#include <SFML/Audio.hpp>

#include <string>
#include <map>
#include <memory>

#include "object_manager.h"

namespace vot
{
    // Sound {{{
    class Sound : public sf::Sound
    {
        public:
            Sound();

            void index(uint32_t value);
            uint32_t index() const;

            void time_played_at(float time);
            float time_played_at() const;

            enum Type {
                Music,
                SoundEffects,
                UserInterface
            };
            void type(Type value);
            Type type() const;

        private:
            uint32_t _index;
            float _time_played_at;
            Type _type;
    };
    // }}}

    // SoundManager {{{
    class SoundManager : public ObjectManager<Sound, 128>
    {
        public:

            void clear();
             
            bool init();
            bool load_default_sounds();

            // Unsure if we can just use raw pointers for sound buffers or not.
            // OpenAL seems to complain when we delete them on shutdown.
            typedef std::map<std::string, sf::SoundBuffer *> SoundBufferMap;
            const SoundBufferMap &sound_buffers();

            typedef std::map<Sound::Type, float> SoundVolumeMap;
            const SoundVolumeMap &volumes() const;

            void volume(Sound::Type type, float volume, bool update_existing_sounds = true);
            float volume(Sound::Type type) const;

            float sfml_volume(Sound::Type type) const;

            bool load_sound_buffer(const std::string &name, const std::string &filename);
            const sf::SoundBuffer *sound_buffer(const std::string &name);

            Sound *spawn_sound(const std::string &name, Sound::Type type);

            void update(float dt);

        private:

            SoundBufferMap _sound_buffers;
            SoundVolumeMap _volumes;

            bool load_sound_log(const std::string &name, const std::string &filename);
            void try_load(const std::string &name, const std::string &from_sound);
    };
    // }}}
}
