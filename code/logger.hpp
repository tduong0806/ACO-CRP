#if !defined(LOGGER_H)
#define LOGGER_H

#include "template.hpp"

void printDebug(std::ostream &ostream, vector<int> fitness, string config = "") {
    ostream << "Evolution: ";
    for (auto f_i : fitness) ostream << f_i << ' ';
    ostream << "\nBest is " << fitness.back() << " with configuration: " << config << '\n';
}

void printDebug(vector<int> fitness, string config = "") {
    printDebug(cout, fitness, config);
}

#endif // LOGGER_H
