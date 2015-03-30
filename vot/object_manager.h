#pragma once

#include <stdint.h>
#include <memory>
#include <array>

#include "utils/utils.h"

namespace vot
{
    template <class T, int size>
    class ObjectManager
    {
        public:
            ObjectManager() :
                _object_index(0u)
            {

            }

            typedef std::array<std::unique_ptr<T>, size> ObjectList;
            ObjectList *objects()
            {
                return &_objects;
            }
            const ObjectList *objects() const
            {
                return &_objects;
            }

        protected:
            uint32_t _object_index;
            ObjectList _objects;

            uint32_t find_empty_object() const
            {
                auto start = _object_index;
                auto index = _object_index + 1;
                while (start != index)
                {
                    if (_objects[index].get() == nullptr)
                    {
                        return index;
                    }

                    index++;
                    if (index >= size)
                    {
                        index = 0u;
                    }
                }

                return utils::Utils::max_uint;
            }

            void insert_object(T *object, uint32_t index)
            {
                object->index(index);
                _objects[index] = std::unique_ptr<T>(object);
            }

    };
}
