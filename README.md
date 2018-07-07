# Priority Queues
This project contains the implementation of five priority queues based in:
`binary heap`, `leftist heap`, `skew heap`, `binomial queue` and `Fibonacci queue`. Also includes the implementation
of 20 functional tests and experiments that validate their correct functioning and high performance.

## Download
Use the [git clone](https://git-scm.com/docs/git-clone) command to download this project:
```
git clone https://gitlab.com/jssprz/priority-queues-cmake.git
```

## Requirements
1. C++ standard 14, [CMAKE_CXX_STANDARD 14](https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html).
2. [CMake](https://cmake.org/download/) 3.10 or higher to be built.

## Installing and Run
Create a build directory inside the project directory and configure the build using CMake to generate
the Makefiles for [GNU Make](https://www.gnu.org/software/make/) or the corresponding generator in Windows:
```
mkdir build
cd build
cmake ..
```

Or using the cmake from CLion IDE, see [Configuring CMake](https://www.jetbrains.com/help/clion/configuring-cmake.html)
for more info.

### CMake options
With the following options we can change the Priority Queues to include in the built. Also, is possible to change
the number of trials to run in each experiment. A higher number of trials means a good estimation of standard deviation
as a greater reliability in the average time result.
```
USE_BINARY_HEAP_PQ     ON
USE_LEFTIST_HEAP_PQ    ON
USE_SKEW_HEAP_PQ       ON
USE_BINOMIAL_PQ        ON
USE_FIBONACCI_PQ       ON
USE_EXPERIMENTS        ON
TRIALS                 8
SORT_MIN_POW_2         15
SORT_MAX_POW_2         21
```
The value of the options can be changed using
```
cmake-gui
````
or
```
ccmake
```

#### Windows
The `cmake-gui` allows specify the generator for the project, which it should be possible to build the project with
`C++14` in compilers that have notion of that standard level, such as Microsoft Visual C++ after 2015 Update 3
(i.e. Visual Studio 15 2017 Win64) see
[Support For C++11/14/17 Features (Modern C++)](https://msdn.microsoft.com/en-us/library/hh567368.aspx) or
and [CXX_STANDARD](https://cmake.org/cmake/help/v3.12/prop_tgt/CXX_STANDARD.html) for more info.
Then configure the options and build the executable in Release configuration.

#### GNU Make
Build the executable named "priority_queues" using GNU Make
```
make
```

The executable can be run with:
```
./priority_queues
```

### Test Results
The program creates a `tests_results` folder in the same location of the executable. Then the program saves inside this
folder a `.report` file with the details of the execution times ([mean](https://en.wikipedia.org/wiki/Mean) and
[standard deviation](https://en.wikipedia.org/wiki/Standard_deviation)) for each test.