#include "generator.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <ostream>
#include <random>
#include <thread>
#include <utility>
#include <vector>


//// IMPLEMENTATION OF MESSAGEQUEUE
/// Taken from Concurrent Traffic Simulation Project
// My implementation of that project: https://github.com/Ssuranthiran0/CppND-Program-a-Concurrent-Traffic-Simulation/blob/master/src/TrafficLight.cpp
template <typename T>
T MessageQueue<T>::receive(){
    //std::cout << "Receiving\n";
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] {
        return !_queue.empty();
    });

    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
std::unique_lock<std::mutex> MessageQueue<T>::getLock() {
        return std::unique_lock<std::mutex>(_mutex);
    }


template <typename T>
void MessageQueue<T>::send(T &&msg){
    //std::cout << "Sending\n";
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}




//// IMPLEMENTATION OF GENERATOR CLASS
Generator::Generator(){
    _messageQueue = std::make_shared<MessageQueue<std::shared_ptr<std::vector<std::vector<float>>>>> ();
}
std::pair<std::vector<std::vector<float>>, bool> Generator::generate(int fracIndex){
    std::cout << "For most fractals, a small domain, range and a very fine step size results in more precision.\n";
    std::vector<std::vector<float>> pnts;
    bool scatter{false};
    switch (fracIndex){
        case 0: // Mandelbrot
            pnts = generateMaJu(0);
            scatter = true;
            break;
        case 1: // Julia
            pnts = generateMaJu(1);
            break;
            scatter = true;
        case 2: // Sierpinski Triangle
            pnts = generateKoSi(1);
            break;
        default: // Koch Snowflake
            pnts = generateKoSi(0);
            break;
    }
    return {pnts, scatter};
}

// helper functions for working with complex numbers
std::vector<float> Generator::squareComplex(std::vector<float> complex, std::vector<float> addend){
    float newReal = std::pow(complex[0], 2) - std::pow(complex[1], 2) + addend[0];
    float newImaginary = complex[0] * complex[1] * 2 + addend[1];
    return std::vector<float>{newReal, newImaginary};
}
float Generator::distance(std::vector<float> complex){
    return std::pow(complex[0], 2) + std::pow(complex[1], 2); // a^2 + b^2 for complex number a + bi
}


std::vector<std::vector<float>> Generator::generateMaJu(int type){ 
    std::vector<float> d(2), r(2), s(2); // for user input, i like to use short names. also initialize it with a length
    int m_iter;
    float stp;
    /*
    std::cout << "Enter width of rendered fractal, will be taken as absolute value. Note: this will scale up/down the fractal to this size. (-10 -> 10) eg. 100; ";
    std::cin >> s[0];
    std::cout << "Enter height of rendered fractal, will be taken as absolute value (-10 -> 10), eg. 5.3: ";
    std::cin >> s[1];
    std::cout << "Enter the amount of iterations, more iterations = more accuracy, but multiplicatively longer to calculate: ";
    std::cin >> m_iter;
    std::cout << "Enter the step size (distance between x of points), lower step size = higher accuracy, but multiplicately longer to calculate: ";
    std::cin >> stp;*/
    // debugging
    float size = 0.5;
    d = {-2, 0.47}; // domain of mandelbrot set. no need to calculate values outside of this
    r = {-1.12, 1.12}; // range of mandelbrot set.
    s = {-size,size};
    m_iter = 50;
    stp = 0.01;
    switch(type){
        case 0:
            return generateMandelbrot(d, r, s, m_iter, stp);
        default:
            std::vector<float> c(2);
            std::cout << "Enter x(a) value for the starting constant complex num for the Julia set(pick any value): ";
            std::cin >> c[0];
            std::cout << "Enter y(b) value for the constant: ";
            std::cin >> c[1];
            return generateJulia(c, d, r, s, m_iter, stp);
    }
}
std::vector<std::vector<float>> Generator::generateKoSi(int type){
    int pointNum = type + 2; // 2 starting for Koch snowflake, 3 for sierpinski triangle.
    int d;
    std::vector<std::vector<float>> pnts(pointNum);
    for(int i=0; i<pointNum; i++){
        std::cout << "Enter x value for point " << i+1 << " : ";
        std::cin >> pnts[i][0];
        std::cout << "Enter y value for point " << i+1 << " : ";
        std::cin >> pnts[i][1];
    }
    std::cout << "Enter the 'depth' of the function (how many times to do it recursively): ";
    std::cin >> d;
    switch(type){
        case 0:
            return generateKoch(pnts, d);
        default:
            return generateSierpinski(pnts, d);
    }

}

bool Generator::checkIfInMandlebrot(std::vector<float> c, int max_iterations, float step){
    std::vector<float> z = {0.0,0.0};
    int iter = 0;
    //std::cout << "x: " << c[0] << " y: " << c[1] << " dist: " << distance(c) << "\n";
    if(distance(c) > 4.0){ // no need to test it if it is already out
        return false;
    }
    while(iter<max_iterations && distance(z) < 4.0){ // if C diverges to +/- infinity, it is not in set
        z = squareComplex(z, c);
        iter++;
    }
    if(iter >= max_iterations){ // if it made it through the loop (if c was in the set)
        return true;
    }else{
        return false;
    }
}
void Generator::printLoadingBar(float percent) {
    //percent *= _averageMandlebrotInRange;
    int barWidth = 50; // width of percent bar
    std::cout << "Loading ["; // print opening bar
    int progress = barWidth * percent / 100; // percent
    for (int i = 0; i < barWidth; ++i) { // for each char in width of bar
        if (i < progress) // [========   ]
            std::cout << "=";
        else
            std::cout << " ";
    }
    std::cout << "] " << std::setw(3) << percent << "%\n"; // write percent
    std::cout.flush();
}
void Generator::checkIfInMandlebrot(float x, float y, int max_iterations, float step, std::vector<float> scale, std::vector<std::vector<float>> &points){
    std::vector<float> c = { x, y}; // c is the starting value
    if(checkIfInMandlebrot(c, max_iterations, step)){
        c[0] *= scale[0];
        c[1] *= scale[1];

        std::unique_lock<std::mutex> lock(_checking); // only let one thread send/receive at a time

        std::vector<std::vector<float>> tempPoints = *_messageQueue->receive(); // receive
        tempPoints.emplace_back(c); // modify
        _messageQueue->send(std::move(std::make_shared<std::vector<std::vector<float>>>(tempPoints))); // add c to the points
    }
}
void Generator::calculateMandelbrotThread(int i, std::vector<float> domain, std::vector<float> range, int max_iterations, float step, std::vector<float> scale, std::vector<std::vector<float>>& points, float total){
    std::vector<float> tempRange(2);
    tempRange[0] = range[0] * 1.0 / _maxThreads * (i+1);
    tempRange[1] = range[1] * 1.0 / _maxThreads * (i+1);
    std::shared_ptr<std::vector<std::vector<float>>> pointsPtr = std::make_shared<std::vector<std::vector<float>>>(points);
    _messageQueue->send(std::move(pointsPtr));
    for (float y=range[0]; y<=range[1]; y+=step){ // test for all b(y) for c
        for(float x=domain[0]; x<=domain[1]; x+=step){ // and all a(x) for c
            _current += step / _maxThreads;
            {
                std::unique_lock<std::mutex> lock(_coutMutex);
                printLoadingBar(_current/total);
            }
            checkIfInMandlebrot(x, y, max_iterations, step, scale, points);
        }   
    }
}

std::vector<std::vector<float>> Generator::generateMandelbrot(std::vector<float> domain, std::vector<float> range, std::vector<float> size, int max_iterations, float step){
    std::vector<std::vector<float>> points; // representing a complex number as just the actual values (just a and b, not i);
    _current = 0;
    _total = (domain[1]-domain[0]) * (range[1]-range[0]);
    std::vector<float> scale = {(domain[1]-domain[0])/std::abs(size[0]), (range[1]-range[0])/std::abs(size[1])};
    std::vector<std::thread> threads;
    for(float i=0; i<_maxThreads; i++){ // run _maxThreads amount of threads at a time
        std::thread t(&Generator::calculateMandelbrotThread, this, i, domain, range, max_iterations, step, scale, std::ref(points), _total); // make a thread
        threads.emplace_back(std::move(t));
    }
    for (std::thread& t : threads) { // for every thread
        if (t.joinable()) { // if can join
            t.join(); // join thread (wait for done)
        }
    }
    points = *_messageQueue->receive();
    return points;
}

std::vector<std::vector<float>> Generator::generateJulia(const std::vector<float>& c, std::vector<float> domain, std::vector<float> range, std::vector<float> size, int max_iterations, float step){
    return {};
}

std::vector<std::vector<float>> Generator::generateSierpinski (std::vector<std::vector<float>>& points, int depth){ // method though the chaos game
    //https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
    if(depth > 0){
        std::shuffle(std::begin(points), std::end(points), std::default_random_engine()); // shuffle vector
        std::vector<float> &start = points[0]; // start point
        std::vector<float> &end = points[1]; // end point
        std::vector<float> midpoint = {(end[0]-start[0])/2, (end[1]-start[1])/2};
        points.emplace_back(midpoint);
        return generateSierpinski(points, depth - 1); // do it recursively
    }
    return points;
}

std::vector<std::vector<float>> Generator::generateKoch (std::vector<std::vector<float>>& points, int depth){
    return {};
}