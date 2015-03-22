#pragma once

#include <string>
#include <stdint.h>

namespace vot
{
    class PlayerInfo
    {
        public:
            PlayerInfo();

            void credits(float value);
            float credits() const;

            void name(const std::string &name);
            const std::string &name() const;

        private:
            uint32_t _credits;
            std::string _name;

    };
}
