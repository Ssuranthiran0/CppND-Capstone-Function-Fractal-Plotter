#include <any>
#include <functional>
#include <iostream>
#include <cmath>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>

#include "network_creator.h"
#include "node.h"
#include "plotter.h"

std::function<float(float)> getFunction(std::string text){
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

std::vector<std::vector<float>> generateDataWithInput(){
    std::string functionString;
    std::function<float(float)> functionToBePlotted;
    while(true){
        std::cout << "Enter a function to be plotted/trained. Note, plotting just connect points, so some 'step' functions, like ceil and floor, will show as being a line. Accepted functions (sin, cos, tan, log (ln), sqrt, exp, abs, floor, ceil): ";
        std::cin >> functionString;
        functionToBePlotted = getFunction(functionString);
        if(functionToBePlotted == nullptr){
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
    return Plotter::generatePoints(functionToBePlotted, start, end, step);
}

int main(){

    std::thread t(&Plotter::plot, Plotter(), generateDataWithInput());
    /*
    Creator creator;
    std::vector<float> inputValues = {3,5,6,8,20};
    creator.createNetworks(inputValues);
    if(creator.getNetworks().size() != 0){ // only do it if creation worked

        // Getting the outputs
        std::vector<std::vector<float>> outputs = creator.getOutputs();

        // Printing the outputs
        for (int i = 0; i < outputs.size(); i++) {
            for(int j = 0; j < outputs[i].size(); j++)  {
                creator.getNetwork(i)->printStructure();
                std::cout << "Output: " << outputs[i][j] << std::endl;
            }
        }
    }*/
    
    t.join(); // wait for thread to terminate (wait for user to close the plotting window)
    return 0;
}   
