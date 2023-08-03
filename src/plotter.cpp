#include "plotter.h"

#include <vector>
#include <iostream>
#include <fstream>

void Plotter::plot(std::vector<std::vector<float>> data, bool scatter){
    saveData(data); // save
    plotData(scatter); // plot
}

void Plotter::saveData(std::vector<std::vector<float>> data){

    std::ofstream datafile; // output filestream
    datafile.open("data.txt"); // open file
    for (int i=0; i< data.size(); i++){ // loop through all
        datafile << data[i][0] << " " << data[i][1] << "\n"; // add to file
    }
    datafile.close(); // close file

}

void Plotter::plotData(bool scatter){
    std::ofstream cmdfile;
    cmdfile.open(_commandsfilename); // open file
    if(!scatter){ // scatter plot
        cmdfile << "plot 'data.txt' lt 7\n"; // plot data.txt
    }else{ // line plot
        cmdfile << "plot 'data.txt' lt 7 w lp\n"; // plot data.txt
    }
    cmdfile << "pause -1\n";
    cmdfile.close();
    system("gnuplot --persist gnuplot_commands.txt");
}

// https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp for the std::function part
std::vector<std::vector<float>> Plotter::generatePoints(std::function<float(float)> f, float start, float end, float step) {
    std::vector<std::vector<float>> points{};
    for (float x = start; x < end; x += step) {
        points.emplace_back(std::vector<float>{static_cast<float>(x), f(x)});
    }
    return points;
}