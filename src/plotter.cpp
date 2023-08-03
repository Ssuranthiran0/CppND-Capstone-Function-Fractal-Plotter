#include "plotter.h"

#include <vector>
#include <iostream>
#include <fstream>

void Plotter::plot(std::vector<std::vector<float>> data){
    saveData(data); // save
    plotData(); // plot
}

void Plotter::saveData(std::vector<std::vector<float>> data){
    _data = data;

    std::ofstream datafile; // output filestream
    datafile.open(_datafilename); // open file
    for (int i=0; i< _data.size(); i++){ // loop through all
        datafile << _data[i][0] << " " << _data[i][1] << "\n"; // add to file
    }
    datafile.close(); // close file

}

void Plotter::plotData(){
    std::ofstream cmdfile;
    cmdfile.open(_commandsfilename); // open file
    //cmdfile << "plot " + _datafilename + " w lp\n"; // plot data.txt
    cmdfile << "plot 'data.txt' w lp\n"; // plot data.txt
    cmdfile << "pause -1\n";
    cmdfile.close();
    system(("gnuplot --persist " + _commandsfilename).c_str());
}

// https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp for the std::function part
std::vector<std::vector<float>> Plotter::generatePoints(std::function<float(float)> f, float start, float end, float step) {
    std::vector<std::vector<float>> points{};
    for (float x = start; x < end; x += step) {
        points.emplace_back(std::vector<float>{static_cast<float>(x), f(x)});
    }
    return points;
}