#include "generator.h"
#include <cmath>
#include <iostream>
#include <vector>

std::vector<std::vector<float>> Generator::generate(int fracIndex){
    switch (fracIndex){
        case 0: // Mandelbrot
            return generateMaJu(0);
        case 1: // Julia
            return generateMaJu(1);
        case 2: // Sierpinski Triangle
            return generateKoSi(1);
        default: // Koch Snowflake
            return generateKoSi(0);
    }
}


std::vector<float> Generator::squareComplex(std::vector<float> complex, std::vector<float> addend){
    float real = std::pow(complex[0], 2) - std::pow(complex[1], 2) + addend[0];
    float imaginary = complex[0] * complex[1] * 2 + addend[1];
    return std::vector<float>{real, imaginary};
}

std::vector<std::vector<float>> Generator::generateMandelbrot(std::vector<float> domain, std::vector<float> range, std::vector<int> size, int max_iterations){
    std::vector<std::vector<float>> points; // representing a complex number as just the actual values (just a and b, not i);
    std::vector<float> scale = {(domain[1]-domain[0])/size[0], (range[1]-range[0])/size[1]}; // scale it to domain and size

    for (int y=0; y<size[1]; y++){
        for(int x=0; x<size[0]; x++){
            std::vector<float> z = { (domain[0] + x * scale[0]), (range[0] + y * scale[1])};
            std::vector<float> c = z; // c is the starting value
            int iter = 0;
            while(iter<max_iterations){ // if C diverges to +/- infinity, it is not in set
                z = squareComplex(z, c);
                if(std::pow(z[0], 2) + std::pow(z[1], 2) > 4.0){ // if z is spiraling out of control
                    break;
                }
                iter++;
            }
            if(iter >= max_iterations){ // if it made it through the loop (if c was in the set)
                points.emplace_back(c); // add c to the points
            }
        }
    }
    return points;
}