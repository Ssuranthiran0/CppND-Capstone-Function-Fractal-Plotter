#include <any>
#include <functional>
#include <iostream>
#include <cmath>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "choose.h"
#include "plotter.h"

Choose::Choose(){
    _plot = std::make_unique<Plotter>();
    _gen = std::make_unique<Generator>();
}
Plotter Choose::getPlotter(){
    return *_plot;
}
Generator& Choose::getGen(){
    return *_gen;
}
bool Choose::getScatter(){
    return _scatter;
}
void Choose::setScanner(bool s){
    _scatter = s;
}

std::function<float(float)> Choose::getFunction(std::string text){ // cant use switch case with strings
    if(text == "sin"){
        return sin;
    }
    if(text == "cos"){
        return cos;
    }
    if (text == "tan") {
        return tan;
    }
    if (text == "log" || text == "ln") {
        return log;
    }
    if (text == "sqrt") {
        return sqrt;
    }
    if (text == "exp") {
        return exp;
    }
    if (text == "abs") {
        return fabs;
    }
    if (text == "floor") {
        return floor;
    }
    if (text == "ceil") {
        return ceil;
    }
    return nullptr;
}

std::vector<std::vector<float>> Choose::generateFunctionDataWithInput(){
    std::string f;
    std::function<float(float)> funct;
    while(true){
        std::cout << "Enter a function to be plotted/trained. Note, plotting just connect points, so some 'step' functions, like ceil and floor, will show as being a line. Accepted functions (sin, cos, tan, log (ln), sqrt, exp, abs, floor, ceil): ";
        std::cin >> f;
        funct = getFunction(f);
        if(funct == nullptr){
            std::cout << "Invalid function. Try again.\n";
        }else{
            break;
        }
    }
    float start{0};
    float end{0};
    float step{0};
    std::cout << "Enter the start value (e.g., 0.0): ";
    std::cin >> start;
    std::cout << "Enter the end value (e.g., 5.0): ";
    std::cin >> end;
    std::cout << "Enter the step (e.g., 0.1). Note, extremely low values for this, in relation to start and end values, increase load times exponentially: ";
    std::cin >> step;


    // Adding inputs to the network
    return Plotter::generatePoints(funct, start, end, step);
}
std::vector<std::vector<float>> Choose::generateFractalDataWithInput(){
    int fract;
    std::cout << "Enter a number that corresponds to a fractal. Mandelbrot Set(0), Julia Set(1), Sierpinski Triangle(2), Koch Snowflake(3), ";
    std::cin >> fract;
    return generateFractalDataWithInput(fract);
}
std::vector<std::vector<float>> Choose::setValues(std::pair<std::vector<std::vector<float>>, bool> out){
    _scatter = out.second;
    return out.first;
}
std::vector<std::vector<float>> Choose::generateFractalDataWithInput(int index){
    Generator gen;
    return setValues(gen.generate(index));
}
std::vector<std::vector<float>> Choose::generateDataWithInput(){
    int answer;
    std::cout << "Enter 0 to generate a fractal, enter 1 to generate a regular function: ";
    std::cin >> answer;
    if(answer == 1){
        return generateFunctionDataWithInput();
    }else{
        return generateFractalDataWithInput();
    }
}

int main(){
    std::cout << "Please note that entering invalid input (entering text instead of numbers, etc) for MOST questions will result in either a crash, or failure to render the function. \n";

    // Testing function, generate any fractal directly with input for values (skip the function step)
    Choose chooser = Choose();
    std::thread t(&Plotter::plot, chooser.getPlotter(), chooser.generateDataWithInput(), true);
    
    t.join(); // wait for thread to terminate (wait for user to close the plotting window)
    return 0;
}   
