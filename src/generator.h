#ifndef GENERATOR_H
#define GENERATOR_H

#include <functional>
#include <thread>
#include <vector>

#include "plotter.h"

class Generator{
    public:
        std::vector<std::vector<float>> generate (int fracIndex);
    private:
        std::vector<float> squareComplex (std::vector<float> complex, std::vector<float> addend={});
        std::vector<std::vector<float>> generateMandelbrot(std::vector<float> domain, std::vector<float> range, std::vector<int> size, int max_iterations);
        std::vector<std::vector<float>> generateJulia (const std::vector<float>& constant, std::vector<float> domain, std::vector<float> range, std::vector<int> size, int max_iterations);
        std::vector<std::vector<float>> generateSierpinski (std::vector<float>& points, int depth, std::vector<std::vector<float>> startingPoints);
        std::vector<std::vector<float>> generateKoch (std::vector<float>& points, int depth, std::vector<std::vector<float>> startingPoints);
        std::vector<std::vector<float>> generateMaJu (bool type); // generate mandelbrot/julia. very similar
        std::vector<std::vector<float>> generateKoSi (bool type); // generate koch/sierpinski

};
#endif