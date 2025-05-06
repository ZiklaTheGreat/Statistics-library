# Statistics Library

A C++ library for performing various statistical computations using the [Armadillo](http://arma.sourceforge.net/) linear algebra library. This project was developed as part of a bachelor's thesis.

## Features

- Core statistical functions
- Easy integration with Armadillo
- Simple demonstration app included

## Prerequisites

- C++17 compiler (e.g., g++, clang++)
- CMake 3.10+
- [BLAS and LAPACK](http://www.netlib.org/lapack/) libraries (usually preinstalled on most systems or installable via package manager)

## Installation

### 1. Clone the Repository
Start by cloning the project repository to your local machine using the following command:

```bash
git clone https://github.com/ZiklaTheGreat/Statistics-library
cd Statistics-library
git submodule update --init --recursive
```

### 2. Build the Armadillo Library
The project depends on the Armadillo library, which is included in the third_party/armadillo-code/ directory. Follow these steps to build it:

```bash
cd third_party/armadillo-code/
cmake .
make
```

### 3. Build the Main Project
Create a build directory and compile the main project using the following commands:

```bash
cd ../
mkdir build
cd build
cmake ..
make
```

## Usage
After building, you can run the demonstration application:

```bash
./main_app
```
