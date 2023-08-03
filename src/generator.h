#ifndef GENERATOR_H
#define GENERATOR_H

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "plotter.h"



// message queue taken from concurrent traffic simulation udacity project
// make a mutex so that you can calculate multiple fractal points at the same time
template <typename T>
class MessageQueue{
    public:
        T receive();
        void send(T &&msg);
        void clear();
        std::unique_lock<std::mutex> getLock();
    private:
        std::deque<T> _queue;
        std::mutex _mutex;
        std::condition_variable _cond;
};


class Generator{
    public:
        Generator();
        std::pair<std::vector<std::vector<float>>, bool> generate (int fracIndex);
    private:
        // helper functions for working with complex numbers
        std::vector<float> squareComplex (std::vector<float> complex, std::vector<float> addend={}); // return (a+bi)^2 + addend.
        float distance (std::vector<float> complex); // get absolute magnitude of complex number. for me, its the distance to zero, so i named it as such

        std::vector<std::vector<float>> generateMandelbrot(
            std::vector<float> domain, std::vector<float> range, std::vector<float> size, int max_iterations, float step=0.5
        );

        std::vector<std::vector<float>> generateJulia (
            std::vector<float> c, std::vector<float> domain, std::vector<float> range, std::vector<float> size, int max_iterations, float step=0.5
        );

        std::vector<std::vector<float>> generateSierpinski (
            std::vector<std::vector<float>>& points, int depth // recursively call the function on points array depth amount of times, then return points
        );
        std::vector<std::vector<float>> generateBarnsley(int depth);
        std::vector<std::vector<float>> generateKoch (
            std::vector<std::vector<float>>& points, int depth // recursively call the function on points array depth amount of times, then return points
        );
        std::vector<std::vector<float>> generateMaJu (int type); // generate mandelbrot/julia. very similar
        std::vector<std::vector<float>> generateKoSiBa (int type); // generate koch/sierpinski


        // thread stuff
        int _maxThreads = 64;
        std::shared_ptr<MessageQueue<std::shared_ptr<std::vector<std::vector<float>>>>> _messageQueue;
        void printLoadingBar(float percent);
        std::mutex _coutMutex;
        float _current;
        float _total;
        std::mutex _checking;

};
#endif