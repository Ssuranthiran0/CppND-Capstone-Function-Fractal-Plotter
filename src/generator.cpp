#include "generator.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
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
            pnts = generateKoSiBa(1);
            scatter = true;
            break;
        case 3:
            pnts = generateKoSiBa(0);
            break;
        default: // Koch Snowflake
            pnts = generateKoSiBa(2);
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
    
    std::cout << "Enter width of rendered fractal, will be taken as absolute value. Note: this will scale up/down the fractal to this size. (-10 -> 10) eg. 100; ";
    std::cin >> s[0];
    std::cout << "Enter height of rendered fractal, will be taken as absolute value (-10 -> 10), eg. 5.3: ";
    std::cin >> s[1];
    std::cout << "Enter the amount of iterations, more iterations = more accuracy, but multiplicatively longer to calculate: ";
    std::cin >> m_iter;
    std::cout << "Enter the step size (distance between x of points), for some fractals, a low value is required. A recommended value is around 0.1 or 0.01. Lower step size = higher accuracy, but multiplicately longer to calculate: ";
    std::cin >> stp;
    // debugging
    d = {-2.00, 0.47}; // domain of mandelbrot set. no need to calculate values outside of this
    r = {-1.12, 1.12}; // range of mandelbrot set.
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
std::vector<std::vector<float>> Generator::generateKoSiBa(int type){
    int pointNum = type + 2; // 2 starting for Koch snowflake, 3 for sierpinski triangle.
    int d;
    std::vector<std::vector<float>> pnts(pointNum);
    std::vector<std::vector<float>> testingPnts { {-10,0}, {10,0}, {0,10} };
    if(type == 2){
        pointNum = 6;
        testingPnts = {{10,10}, {10,4}, {4,5}, {15, 3}, {17,7}, {18,0}};
    }
    //testingPnts = {{10,10}, {10,4}, {4,5}, {15, 3}, {17,7}, {18,0}};
    for(int i=0; i<pointNum; i++){/* this code doesnt work
        float x, y;
        std::cout << "Enter x value for point " << i+1 << " : ";
        std::cin >> x;
        std::cout << "Enter y value for point " << i+1 << " : ";
        std::cin >> y;
        pnts.push_back({x, y});*/
    }
    pnts = testingPnts;
    std::cout << "Enter the 'depth' of the function (how many times to do it recursively) NOTE: Sierpinski Triangle will require a lot more depth: ";
    std::cin >> d;
    switch(type){
        case 0:
            return generateKoch(pnts, d);
        case 1:
            return generateSierpinski(pnts, d);
        default:
            return generateBarnsley(d);
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

std::vector<float> multiply(std::vector<float> vec, float p){
    for(float &t : vec){
        t *= p;
    }
    return vec;
}

std::vector<std::vector<float>> Generator::generateMandelbrot(std::vector<float> domain, std::vector<float> range, std::vector<float> size, int max_iterations, float step){
    std::vector<std::vector<float>> points; // representing a complex number as just the actual values (just a and b, not i);
    _current = 0;
    _total = (domain[1]-domain[0]) * (range[1]-range[0]);

    std::vector<float> scale = {(domain[1]-domain[0])/std::abs(size[0]), (range[1]-range[0])/std::abs(size[1])};
    
    std::vector<float> c = {0, 0}; // c is the starting value
    std::vector<float> z = {0,0}; // share the memory for the two vectors

    for (float y=range[0]; y<=range[1]; y+=step){ // test for all b(y) for c
        for(float x=domain[0]; x<=domain[1]; x+=step){ // and all a(x) for c
            _current += step;
            printLoadingBar(_current/_total);
            c = {x, y};
            z = {0, 0};
            int iter = 0;
            while(iter<=max_iterations && distance(z) < 4.0 && distance(c) < 4.0){ // if C diverges to +/- infinity, it is not in set
                z = squareComplex(z, c);
                iter++;
            }
            if(iter >= max_iterations){ 
                c[0] *= scale[0];
                c[1] *= scale[1];

                points.emplace_back(c); // modify
            }
        }
    }
    return points;
}

std::vector<std::vector<float>> Generator::generateJulia(std::vector<float> c, std::vector<float> domain, std::vector<float> range, std::vector<float> size, int max_iterations, float step){
    // kinda the same as mandlebrot but not rlly
    std::vector<std::vector<float>> points; // representing a complex number as just the actual values (just a and b, not i);
    _current = 0;
    _total = (domain[1]-domain[0]) * (range[1]-range[0]) / step;

    std::vector<float> scale = {(domain[1]-domain[0])/std::abs(size[0]), (range[1]-range[0])/std::abs(size[1])};
    
    std::vector<float> z = {0,0}; // share the memory for the two vectors

    for (float y=range[0]; y<=range[1]; y+=step){ // test for all b(y) for c
        for(float x=domain[0]; x<=domain[1]; x+=step){ // and all a(x) for c
            _current += step;
            std::vector<std::vector<float>> tempPoints;
            printLoadingBar(_current / _total * 100);
            z = {x, y};
            int iter = 0;
            while(iter<=max_iterations && distance(z)){ // if C diverges to +/- infinity, it is not in set
                z = squareComplex(z, c);
                tempPoints.push_back( {c[0] * scale[0], c[1] * scale[1]} );
                iter++;
            }
            if(iter >= max_iterations){ 
                std::vector<float> pnt;
                for(auto i : tempPoints){
                    points.emplace_back(i);
                }
                //points.emplace_back(pnt); // modify
            }
        }
    }
    return points;
}


std::vector<std::vector<float>> Generator::generateSierpinski(std::vector<std::vector<float>> &points, int depth) {
    std::vector<float> currentPoint = points[rand() % 3];
    int startDepth = depth;
    Plotter().saveData(points);
    while (depth > 0) {
        printLoadingBar(float(startDepth-depth) * 100 / float(startDepth));
        std::vector<float> toPoint = points[rand() % 3];
        currentPoint = { (currentPoint[0] + toPoint[0]) / 2, (currentPoint[1] + toPoint[1]) / 2};
        points.emplace_back(currentPoint);
        depth -= 1;
    }
    return points;
}

std::vector<float> add(std::vector<float> a, std::vector<float> b, float factor){
    return { factor * (a[0] + b[0]), factor * (a[1]+b[1])};
}
std::vector<float> subtract(std::vector<float> a, std::vector<float> b, float factor){
    return { factor * (-a[0] + b[0]), factor * (-a[1]+b[1])};
}
std::vector<std::vector<float>> Generator::generateBarnsley(int depth){
    // https://en.wikipedia.org/wiki/Barnsley_fern
    float x = 0.0; // u can play around with these values
    float y = 0.0;
    std::vector<std::vector<float>> points;
    for(float i=0; i<depth; i++){
        printLoadingBar(i/float(depth));
        float r = float(rand()) / float(RAND_MAX);
        if (r < 0.1){
        x = 0.0;
        y = 0.16 * y;
        }else if (r < 0.86){
            x = 0.85 * x + 0.04 * y;
            y = -0.04 * x + 0.85 * y + 1.6;
        }else if (r < 0.93){
            x = 0.2 * x - 0.26 * y;
            y = 0.23 * x + 0.22 * y + 1.6;
        }else{
            x = -0.15 * x + 0.22 * y;
            y = 0.26 * x + 0.24 * y + 0.44;
        }
        points.push_back({x, y});
    }
    return points;

}
std::vector<std::vector<float>> Generator::generateKoch(std::vector<std::vector<float>>& points, int depth) { // BROKEN
    if(depth <= 0){
        return points;
    }

    std::vector<std::vector<float>> newPoints;
        
    for (int i = 0; i < points.size(); ++i) {
        std::vector<float>& p1 = points[i];
        std::vector<float>& p2 = points[(i + 1) % points.size()];
        
        
        // Calculate the distance between the two points
        float dist = std::sqrt(distance({(p1[0] - p2[0]), (p1[1] - p2[1])}));
        
        // Calculate the coordinates of the three new points. Got these values from somewhere, I do not remember that website.
        std::vector<float> new1 = { (2 * p1[0] + p2[0])/3, (2 * p1[1] + p2[1])/3};
        std::vector<float> new2 = { static_cast<float>((p1[0] + p2[0]) / 2 + (p2[1] - p1[1]) * std::sqrt(3) / 6), static_cast<float>((p1[1] + p2[1]) / 2 + (p1[0] - p2[0]) * std::sqrt(3) / 6) };
        std::vector<float> new3 = { (p1[0] + 2 * p2[0]) / 3, (p1[1] + 2 * p2[1]) / 3 };

        // Add the new points to the newPoints vector
        newPoints.push_back(new1);
        newPoints.push_back(new2);
        newPoints.push_back(new3);
    }
    
    // Recursively generate the fractal with reduced depth
    return generateKoch(newPoints, depth - 1);
}