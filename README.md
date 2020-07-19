# Maze-Router
How to use:
To run the maze router you need a C++ compiler and an input file which contains the nets to be routed. After running the
program, you must type the file name at runtime to the compiler so that it can starts reading the input file. The input
file must be logically correct for the program to run successfully. For example, each cell must be used by only one net.
The nets must have the following format:
net_name (pin_1_layer, pin_1_x, pin_1_y) (pin_2_layer, pin_2_x, pin_2_y) ...

After running the cpp file, an output.txt file will be generated and you will need to run the visualizer.py file to 
visualize the output. To run visualizer.py, you will need python to be installed on your device. Also, you will need 
to download matplotlib and install it for the program to run.

Limitaions:
Visualizing the output have to be done for 3 layers at most. 

Assumptions:
A net must consist at least 2 pins.
