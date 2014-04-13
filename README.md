TSP
===

Implementation of Traveling Salesman Problem
Running The Program:
The input to the program needs to be in the form of a valid input.txt file and needs to be stored in the same directory as the rest of the code files. The program then saves the 2 output files(output_path and output_log) in the same directory as the input file.

Program Structure:

main.cpp: 
A vector is used to store all the input points.
The spanning tree for task 3 is constructed using Prim's algorithm.
The dist() calculates the Euclidean  distance of 2 points. 
The getHeuristic() returns the heuristic cost of the current state. For Task 2, it is simply the distance 
between the current point and the starting point. For task 3 it is the size of spanning tree which is constructed using all unvisited points and starting point.
Implements methods to check the number of arguments, parsing them, prints error messages for bad arguments and invokes the dist() and getHeuristic().

Execution of the program:

For code compilation:
Run the following command for code compilation:

g++ main.cpp -o tsp

For running the code:
Execute the command in the following format as given in the specification:-
tsp -t <task> -s <start_node> -i <input_file> -op <output_path> -ol <output_log>

