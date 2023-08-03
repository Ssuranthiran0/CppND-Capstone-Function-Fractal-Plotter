#ifndef CHOOSE_H
#define CHOOSE_H

#include <functional>
#include <memory>
#include <string>

#include "plotter.h"
#include "generator.h"

class Choose{
    public:
        Choose();
        std::vector<std::vector<float>> generateDataWithInput();
        std::vector<std::vector<float>> generateFractalDataWithInput(int index);
        Generator& getGen();
        Plotter getPlotter();
        std::vector<std::vector<float>> setValues(std::pair<std::vector<std::vector<float>>, bool> out);
        bool getScatter();
        void setScanner(bool s);
    private:
        std::function<float(float)> getFunction(std::string text);
        std::vector<std::vector<float>> generateFunctionDataWithInput();
        std::vector<std::vector<float>> generateFractalDataWithInput();
        std::unique_ptr<Generator> _gen;
        std::unique_ptr<Plotter> _plot;
        bool _scatter{false};
};

#endif