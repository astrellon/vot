#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    // HudMain {{{
    class HudMain : public sf::Drawable
    {
        public:
            HudMain();

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const sf::Font *_sans;
            sf::Text _health;
    };
    // }}}

    // HudWorld {{{
    class HudWorld : public sf::Drawable
    {
        public:
            HudWorld(sf::View &camera);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const sf::Font *_sans;
            sf::Sprite _target;
            sf::View &_camera;
            bool _has_target;
    };
    // }}}
}
