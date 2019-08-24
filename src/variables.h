#pragma once

#include "util.h"

#include <cstring>
#include <iostream>
#include <vector>


class Variables
{
public:
    using List = std::vector<const char*>;
    using ConstIterator = List::const_iterator;

public:
    void add(const char* var);
    uint16_t dim() const
    {
        return static_cast<uint16_t>(list_.size());
    }

    ConstIterator begin() const
    {
        return list_.begin();

    }

    ConstIterator end() const
    {
        return list_.end();
    }

    const char* variable(uint16_t var) const
    {
        ConstIterator it(list_.begin());
        it += var;
        return *it;
    }

    int read(std::istream& in) const;

private:
    int find(const char* var) const;

private:
    List list_;
};
