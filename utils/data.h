#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdint.h>

namespace utils
{
    class Data
    {
        public:
            enum Type
            {
                NUMBER,
                INT32,
                UINT32,
                STRING,
                BOOLEAN,
                ARRAY,
                MAP
            };

            Data(Type type);
            Data(double value);
            Data(uint32_t value);
            Data(int32_t value);
            Data(bool value);
            Data(const std::string &value);
            Data(const char *value);
            ~Data();

            Type type() const;

            typedef std::map<std::string, std::unique_ptr<Data> > DataMap;
            typedef std::vector<std::unique_ptr<Data> > DataArray;

            void number(double value);
            double number() const;

            void int32(int32_t value);
            int32_t int32() const;

            void uint32(uint32_t value);
            uint32_t uint32() const;

            void boolean(bool value);
            bool boolean() const;

            void string(const std::string &value);
            void string(const char *value);
            std::string string() const;

            // Map {{{
            DataMap::const_iterator begin_map() const;
            DataMap::iterator begin_map();

            DataMap::const_iterator end_map() const;
            DataMap::iterator end_map();

            void at(const std::string &key, double value);
            void at(const std::string &key, int32_t value);
            void at(const std::string &key, uint32_t value);
            void at(const std::string &key, bool value);
            void at(const std::string &key, const std::string &value);
            void at(const std::string &key, const char *value);
            void at(const std::string &key, Data *value);

            Data *at(const std::string &key) const;
            bool has(const std::string &key) const;

            std::size_t size_map() const;
            // }}}

            // Array {{{
            DataArray::const_iterator begin_array() const;
            DataArray::iterator begin_array();

            DataArray::const_iterator end_array() const;
            DataArray::iterator end_array();

            void push(double value);
            void push(int32_t value);
            void push(uint32_t value);
            void push(bool value);
            void push(const std::string &value);
            void push(const char *value);
            void push(Data *value);
            void at(std::size_t index, double value);
            void at(std::size_t index, int32_t value);
            void at(std::size_t index, uint32_t value);
            void at(std::size_t index, bool value);
            void at(std::size_t index, const std::string &value);
            void at(std::size_t index, const char *value);
            void at(std::size_t index, Data *value);

            Data *at(std::size_t index) const;

            std::size_t size_array() const;
            // }}}

        private:
            union Value
            {
                double number;
                int32_t int32;
                uint32_t uint32;
                bool boolean;
                std::string *string;
                DataMap *map;
                DataArray *array;
            };

            Type _type;
            Value _value;

            static DataMap s_empty_map;
            static DataArray s_empty_array;

    };

    typedef std::unique_ptr<Data> UData;
}
