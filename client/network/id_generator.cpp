#pragma once
#include "include.hpp"
#include <vector>
#include <functional>

class id_generator  {
    private:
        std::vector<unsigned int> ids;
    public:
        id_generator(){}
        unsigned int get_unique_id();
};