#include "../network/id_generator.cpp"

unsigned int id_generator::get_unique_id() {
    unsigned int tmp = rand();
    while (std::find(ids.begin(), ids.end(), tmp) != ids.end())
        tmp = rand();
    return tmp;
}