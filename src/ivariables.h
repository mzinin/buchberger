#pragma once

#include "iutil.h"

#include <cstring>
#include <iostream>
#include <vector>


class IVariables
{
public:
    using List = std::vector<const char*>;
    using ConstIterator = List::const_iterator;

public:
    void add(const char *var);
    int dim() const
    {
        return list_.size();
    }

    ConstIterator begin() const
    {
        return list_.begin();

    }

    ConstIterator end() const
    {
        return list_.end();
    }

    const char* variable(int var) const
    {
        ConstIterator it(list_.begin());
        it += var;
        return *it;
    }

    int find(const char *var) const;
    int read(std::istream& in) const;

private:
    List list_;
};
