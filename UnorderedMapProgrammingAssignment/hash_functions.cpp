#include "hash_functions.h"

size_t polynomial_rolling_hash::operator()(std::string const &str) const
{
    size_t hash = 0;
    size_t p = 1;
    for (int i = 0; i < str.length(); ++i) {
        hash += str.at(i) * p;
        p = (p * 19) % 3298534883309ul;
    }

    return hash;
}

size_t fnv1a_hash::operator()(std::string const &str) const
{
    size_t hash = 0xCBF29CE484222325;
    for (int i = 0; i < str.length(); ++i) {
        hash = hash ^ str.at(i);
        hash = hash * 0x00000100000001B3 ;
    }

    return hash;
}
