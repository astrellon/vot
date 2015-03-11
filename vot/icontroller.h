#pragma once

namespace vot
{
    class IController
    {
        public:
            virtual void update(float dt) = 0;
    };
}
