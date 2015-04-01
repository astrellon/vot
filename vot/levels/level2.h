#pragma once

#include "level.h"

namespace vot
{
    class Level2 : public Level
    {
        public:
            virtual void init();
            virtual void deinit();

            virtual void update(float dt);

            virtual std::string name() const;

        private:
            float _spawn_timer;
    };
}
