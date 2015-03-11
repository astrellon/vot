#pragma once

#include "icontroller.h"

namespace vot
{
    class Enemy;

    class EnemyFighter : public IController
    {
        public:
            EnemyFighter(Enemy *controlling);

            void controlling(Enemy *enemy);
            Enemy *controlling() const;

            virtual void update(float dt);

        private:
            Enemy *_controlling;
            
            float _cooldown;
    };
}
