# CPPND: Capstone Project Repo

This is my capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

Cloned from https://github.com/Udacity/CppND-Capstone-Hello-World starter code provided by Udacity.

## Overview of Project
For this project, I have chosen to implement a function grapher/fractal generator. It will take input from the console, and then open up a gnuplot window to show the function/fractal.

## Project Structure


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw-w64.org/)
* gnuplot >= 5.4.8
  * All OSes: [click here for installation files](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.8/)
  * Link includes exe, zip, and 7z files for Windows and a tar.gz file for all OSes.
  * Linux
    * Arch Linux: [Arch package link](https://archlinux.org/packages/extra/x86_64/gnuplot/) Command: sudo pacman -Syu gnuplot
  * To test installation, open a terminal window and type 'gnuplot' into the window and press enter. If it opens gnuplot (in the terminal), then the program will run properly.


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Plotter`.

## Capstone Project Fulfilled Requirements

Requirements for the capstone project.

### Loops, Functions, I/O COMPLETELY DONE 3/3

* 1. The project demonstrates an understanding of C++ functions and control structures. DONE
    * A variety of control structures are used in the project.
    * The project code is clearly organized into functions.
  
*  2. The project reads data from a file and process the data, or the program writes data to a file. DONE
    * The project reads data from an external file or writes data to a file as part of the necessary operation of the program. (Plotter.cpp, Plotter::saveData() ~line11)
  
*  3. The project accepts user input and processes the input. DONE
    * The project accepts input from a user as part of the necessary operation of the program.
    * User can specify a function/fractal to be graphed, when the program is started (main.cpp)

### Object Oriented Programming MAINLY DONE 4/9

* 1. The project uses Object Oriented Programming techniques. DONE
	* The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
  * Shown in all files in src, besides main.cpp

* 2. Classes use appropriate access specifiers for class members. DONE
  * All class data members are explicitly specified as public, protected, or private.
  * Shown throughout all classes

* 3. Class constructors utilize member initialization lists. DONE
  * All class members that are set to argument values are initialized through member initialization lists.
    * Shown throughout all class constructors

* 4. Classes abstract implementation details from their interfaces. NOT DONE
  * All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.
    * Unimplemented

* 5. Classes encapsulate behavior. DONE
	* Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
    * Implemented in all classes

* 6. Classes follow an appropriate inheritance hierarchy. NOT DONE
	* Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.

* 7. Overloaded functions allow the same function to operate on different parameters. NOT DONE
	* One function is overloaded with different signatures for the same function name.

* 8. Derived class functions override virtual base class functions. NOT DONE
	* One member function in an inherited class overrides a virtual base class member function.

* 9. Templates generalize functions in the project. NOT DONE
	* One function is declared with a template that allows it to accept a generic parameter.

### Memory Management PARTLY DONE 2/6

* 1. The project makes use of references in function declarations. DONE
	* At least two variables are defined as references, or two functions use pass-by-reference in the project code.

* 2. The project uses destructors appropriately. NOT DONE (Smart ptrs)
	* At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.

* 3. The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. ? NOT DONE
	* The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.

* 4. The project follows the Rule of 5. DONE
  * For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.
  * Implemented in all classes

* 5. The project uses move semantics to move data, instead of copying it, where possible. NOT DONE
	* For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.

* 6. The project uses smart pointers instead of raw pointers. NOT DONE
	* The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.

### Concurrency PARTIALLY DONE 1/4

* 1. The project uses multithreading. DONE
	* The project uses multiple threads in the execution.
    * Uses thread in main.cpp to ensure than program doesn't terminate until user closes window.

* 2. A promise and future is used in the project. NOT DONE
	* A promise and future is used to pass data from a worker thread to a parent thread in the project code.

* 3. A mutex or lock is used in the project. NOT DONE
	* A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.

* 4. A condition variable is used in the project. NOT DONE
	* A std::condition_variable is used in the project code to synchronize thread execution.
