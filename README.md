# Overview

Program will create linear interpolations and least square approximation of provided temperatures. Project description can be found in https://github.com/ShroudofDark/CPUTemperatures/blob/main/CPUTemps-SemesterProject.pdf

All software was authored by me, except for parseTemps.h which was provided by the professor Thomas Kennedy. 

Linear Interpolation: https://en.wikipedia.org/wiki/Linear_interpolation
Least Squares: https://en.wikipedia.org/wiki/Least_squares

# Requirements

	* Make
	* g++ (GCC) 11.2.0 or newer

# Compilation

The code can be compiled with the provided makefile using the `make` command.

Include these flags if compiling the code manually:

```
CFLAGS = -g -std=c++17 -Wall -w

```

# Sample Execution & Output

If run without command line arguments, using

```
./cpuTemps
```

The following usage message will be displayed.
```
Usage: ./cpuTemps input_file_name
```

If run using

```
./main testTemps.txt
```

Where testTemps.txt contains

```
+61.0°C +63.0°C +50.0°C +58.0°C
+80.0°C +81.0°C +68.0°C +77.0°C
+62.0°C +63.0°C +52.0°C +60.0°C
+83.0°C +82.0°C +70.0°C +79.0°C
+68.0°C +69.0°C +58.0°C +65.0°C
```

output would create 4 text files of the following names

```
testTemp-core-0.txt
testTemp-core-1.txt
testTemp-core-2.txt
testTemp-core-3.txt
```

For example the inside of testTemp-core-0.txt contains
```
       0 <= x <      30; y_0        =      61.0000 +       0.6333x; interpolation
      30 <= x <      60; y_1        =      98.0000 +      -0.6000x; interpolation
      60 <= x <      90; y_2        =      20.0000 +       0.7000x; interpolation
      90 <= x <     120; y_3        =     128.0000 +      -0.5000x; interpolation
       0 <= x <     120; y          =      67.4000 +       0.0567x; least-squares

```

Do note an input file it will only handle 4 columns which represent 4 cores. If multiple files are provided only the 1st file is read.
