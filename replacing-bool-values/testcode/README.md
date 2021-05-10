# Introduction

This directory contains test code for the 'Replacing `bool` Values' article.

## The \*.cpp Files

All the _\*.cpp_ files except _functions.cpp_ create three functions, taking 
one, two, and three flags respectively and returning an int value generated 
using the flags to control calls to the `rand()` function. The _functions.cpp_ 
file uses templates to create separate functions for each combination of `true` 
and `false` values for up to three flags. We use the `rand` function so the 
compiler cannot optimize the function calls away.

The `main` function in each program uses `srand` to initialize the random number 
generator to the same point in each program, so their outputs can be compared to 
ensure they are identical. It then runs a loop 1000 times that calls each 
function with all possible combinations of `true` and `false` and sums up the 
return values. The final sum is printed at the end. This prevents the compiler 
optimizing away the variable and all the calls used to set it up. It also allows 
us to check that each program has done the same set of calls. The run time for 
the loop is also printed so we can compare them later.

Note that the _\*.cpp_ files use the `{fmt}` library by Victor Zverovich for
output. This is the library that the C++20 `std::format` library is based on,
and is worth getting if you want to try out the new features before they hit
your standard library implementation. If you do not have it you can get it
from https://github.com/fmtlib/fmt.

If you'd rather not install a new library, you can just modify the files to 
output using iostreams. The only output lines are the last two in the `main` 
function in each program. You also have to remove the `-lfmt` options from the 
compile lines in the makefile recipe, and the `#include <fmt/format.h>` line 
from the top of each source file.

## Running make

Running `make` will use `g++` to build the following files for each _\*.cpp_ 
file:

* _.noopt.asm_ - Assembler output for build with no optimization
* _.opt.asm_ - Assembler output for build with -O3 optimization
* _.noopt_ - Executable build with no optimization
* _.opt_ - Executable build with -O3 optimization
* _.out_ - Output of running the \*.opt file

# The _find-medians.sh_ Shell Script

The time it takes to run a program can be affected by other activity on the 
machine. To mitigate against this it makes sense to run a program multiple times 
and take an average of the runtimes.

Normally when saying we will take an average people are refering to the mean, 
i.e. the sum of all numbers divided by the count of them. However, as most 
program runtimes tend to be around the lowest runtime, with just a few high 
values, taking the mean will end up with a higher number because those few long 
runtimes will skew the value upwards.

To get around this we are better off using one or both of two other averages, 
the median and the mode. The median value is what you get when you sort the list 
of values and take the one in the middle position. The mode is the most common 
value in the list.

The script file _find-medians.sh_ can be used to find the median and mode of the 
runtimes for each _\*.opt_ program. It runs each program the number of times set 
by the `numruns` variable, currently set to 101. It then works out the median 
and mode and outputs them to the file _medians.txt_. The mode value is followed 
by the the number of times the mode value appears in the runtimes list.
