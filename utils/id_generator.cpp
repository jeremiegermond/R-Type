#pragma once
#include <vector>
#include <functional>

class id_generator  {
    private:
        std::vector<unsigned int> ids;
    public:
        id_generator(){}
        unsigned int get_unique_id() {
            unsigned int tmp = rand();
            while (std::find(ids.begin(), ids.end(), tmp) != ids.end())
                tmp = rand();
            return tmp;
        };
};