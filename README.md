This is a programme that finds the area of a figure limited by three curves if it is known that each two of them have a single intersection in a given interval.

The programme is built using make. To build it in default mode, use "make" while in the main dir.
1.1 The default root calculation method is binary search. There is also chord method of approximation available. To build the project with the chord mode use "METHOD=chord make".

1.2 Functions, used by the programme by default are e^x + 2, -1/x and -2(x+1)/3. To use custom functions, make a .txt file, write two floating-point values into its first line and your functions in polish reverse notation into three next lines. Available binary operands are +, -, *, /, available singular functions are sin, cos, tan and ctg. The variable is always x. Then build the program using "SPEC_FILE=myfile.txt make hard". There is a sample file called open.txt in the main directory.

1.3 These two optionals can be combined, i.e. "SPEC_FILE=myfile.txt METHOD=chord make hard".

1.4 The programme can be launched in test mode, calculating three other functions, x^sinx, cosx and x-6 in a given interval with given prescision, and display the roots, the integrals, and the result. The test variant is both built and ran with "make test"

After the programme is built, you can launch it using ./integral (--options). List of available options: (in long and short form) --help -h: Displays all available command line options. --root -r: Displays intersection points of functions. --iterations -i: Display the number of iterations required to find intersection points and integrals. --test-root -R: Tests the root function. Format: -R F1:F2:A:B:E:R, F1, F2 - function indexes, A, B - limits, E - prescision, R - expected result. --test-integral -I: Tests the root function. Format: -I F:A:B:E:R, F, - function index, A, B - limits, E - prescision, R - expected result. If launched without any options, the programme will calculate the area limited by the three functions, answering the problem.
