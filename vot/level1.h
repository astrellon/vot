#pragma once

#include "level.h"

namespace vot
{
    class Level1 : public Level
    {
        public:
            virtual void init();
            virtual void deinit();

            virtual void update(float dt);

        private:
            float _spawn_timer;
    };
}
