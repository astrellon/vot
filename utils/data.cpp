#include "data.h"

namespace utils
{
    Data::DataMap Data::s_empty_map;
    Data::DataArray Data::s_empty_array;

    Data::Data(Data::Type type) :
        _type(type)
    {
        if (_type == STRING)
        {
            _value.string = nullptr;
        }
        else if (type == ARRAY)
        {
            _value.array = new DataArray();
        }
        else if (type == MAP)
        {
            _value.map = new DataMap();
        }
    }
    Data::Data(double value) :
        _type(NUMBER)
    {
        _value.number = value;
    }
    Data::Data(bool value) :
        _type(BOOLEAN)
    {
        _value.boolean = value;
    }
    Data::Data(const std::string &value) :
        _type(STRING)
    {
        _value.string = new std::string(value);
    }

    Data::~Data()
    {
        if (_type == STRING && _value.string != nullptr)
        {
            delete _value.string;
        }
        else if (_type == ARRAY)
        {
            delete _value.array;
        }
        else if (_type == MAP)
        {
            delete _value.map;
        }
    }

    Data::Type Data::type() const
    {
        return _type;
    }

    void Data::number(double value)
    {
        if (_type == NUMBER)
        {
            _value.number = value;
        }
    }
    double Data::number() const
    {
        return _value.number;
    }

    void Data::boolean(bool value)
    {
        if (_type == BOOLEAN)
        {
            _value.boolean = value;
        }
    }
    bool Data::boolean() const
    {
        return _value.boolean;
    }

    void Data::string(const std::string &value)
    {
        if (_type == STRING)
        {
            _value.string = new std::string(value);
        }
    }
    std::string Data::string() const
    {
        if (_type == STRING)
        {
            return std::string(*_value.string);
        }
        return std::string();
    }

    // Map {{{
    Data::DataMap::const_iterator Data::begin_map() const
    {
        return _type == MAP ? _value.map->cbegin() : s_empty_map.cend();
    }
    Data::DataMap::iterator Data::begin_map()
    {
        return _type == MAP ? _value.map->begin() : s_empty_map.end();
    }

    Data::DataMap::const_iterator Data::end_map() const
    {
        return _type == MAP ? _value.map->cend() : s_empty_map.cend();
    }
    Data::DataMap::iterator Data::end_map()
    {
        return _type == MAP ? _value.map->end() : s_empty_map.end();
    }

    void Data::at(const std::string &key, double value)
    {
        if (_type == MAP)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, bool value)
    {
        if (_type == MAP)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, const std::string &value)
    {
        if (_type == MAP)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, Data *value)
    {
        if (_type == MAP)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(value);
        }
    }

    Data *Data::at(const std::string &key) const
    {
        if (_type == MAP)
        {
            auto find = _value.map->find(key);
            if (find != _value.map->end())
            {
                return find->second.get();
            }
        }
        return nullptr;
    }
    bool Data::has(const std::string &key) const
    {
        if (_type == MAP)
        {
            return _value.map->find(key) != _value.map->cend();
        }
        return false;
    }

    std::size_t Data::size_map() const
    {
        return _type == MAP ? _value.map->size() : 0u;
    }
    // }}}

    // Array {{{
    Data::DataArray::const_iterator Data::begin_array() const
    {
        return _type == ARRAY ? _value.array->cbegin() : s_empty_array.cend();
    }
    Data::DataArray::iterator Data::begin_array()
    {
        return _type == ARRAY ? _value.array->begin() : s_empty_array.end();
    }

    Data::DataArray::const_iterator Data::end_array() const
    {
        return _type == ARRAY ? _value.array->cend() : s_empty_array.cend();
    }
    Data::DataArray::iterator Data::end_array()
    {
        return _type == ARRAY ? _value.array->end() : s_empty_array.end();
    }

    void Data::push(double value)
    {
        if (_type == ARRAY)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(bool value)
    {
        if (_type == ARRAY)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(const std::string &value)
    {
        if (_type == ARRAY)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(Data *value)
    {
        if (_type == ARRAY)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(value));
        }
    }

    void Data::at(std::size_t index, double value)
    {
        if (_type == ARRAY)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, bool value)
    {
        if (_type == ARRAY)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, const std::string &value)
    {
        if (_type == ARRAY)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, Data *value)
    {
        if (_type == ARRAY)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(value);
        }
    }

    Data *Data::at(std::size_t index) const
    {
        if (_type == ARRAY && index < _value.array->size())
        {
            (*_value.array)[index].get();
        }
        return nullptr;
    }

    std::size_t Data::size_array() const
    {
        return _type == ARRAY ? _value.array->size() : 0u;
    }
    // }}}

}