#ifndef PLOTTER_H
#define PLOTTER_H

#include <functional>
#include <vector>
#include <string>

class Plotter{
    public:
        void plot(std::vector<std::vector<float>> data); // add a layer of abstraction between the plotting and saving functiosn and the user
        static std::vector<std::vector<float>> generatePoints(std::function<float(float)> func, float start, float end, float step); // takes a function and points as an input
    private:
        void saveData(std::vector<std::vector<float>> data); // save data to _datafilename file
        void plotData(); // read data from _datafilename, write to gnuplot_commands.txt, run gnuplot with gnuplot_commands.txt
        std::string _datafilename = "data.txt";
        std::string _commandsfilename = "gnuplot_commands.txt"; // commands
        std::vector<std::vector<float>> _data;
};

#endif