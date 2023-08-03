#ifndef GENERATOR_H
#define GENERATOR_H

#include <functional>
#include <thread>
#include <vector>

#include "plotter.h"

class Generator{
    public:
        Generator();
        /// Ro5
        ~Generator();
        Generator(const Generator &source);
        Generator& operator+(const Generator &source);
        Generator(Generator &&source);
        Generator& operator+(Generator &&source);
        //// EOF Ro5
    private:
        std::vector<std::thread> _threads;
}
#endif