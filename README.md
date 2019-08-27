# Buchberger Agorithm

This in an C++ implementation of [Buchberger's algorithm](https://en.wikipedia.org/wiki/Buchberger's_algorithm). The input of `buchberger` util is the initial boolean basis in [GNV format](https://github.com/mzinin/groebner_benchmarks), the output is the corresponding boolean [Gröbner basis](https://en.wikipedia.org/wiki/Gröbner_basis).


## Getting Started

### Prerequisites

To compile this project one would need:
* C++ compiler with C++14 support (tested ones are GCC and MSVC)
* CMake >= 3.5.0

### Build

On Linux:
```
cd ./build
cmake ..
make
```
On Windows do the following:
```
cd ./build
cmake ..
```
and open `buchberger.sln` with Visual Studio.


## Usage

To get the usage message just run utility without any arguments:

```
$ ./buchberger
Usage:
	./buchberger <file_name.gnv> - execute given task;
	./buchberger -v, --version - print version;
	./buchberger -h, --help - print this message.
```

### Run a benchmark

At is it said in the usage message, to run a becnhmark of computing boolean Gröbner basis one should provide path to GNV file as the first argument:

```
$ ./buchberger ./kats14.gnv
  user time: 0.70 sec
system time: 0.00 sec
  real time: 0.70 sec

The answer is CORRECT
```

`buchberger` utility will read the content on the GNV file and build a boolean Gröbner basis for the initial basis. If and only if the GNV file contains the correct answer, the output message `The answer is CORRECT` of `The answer is WRONG` has meaning.
