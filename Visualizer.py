import matplotlib.pyplot as plt
import numpy as np

f = open("out.txt", "r")
layers = [] 
x_coordinates = [] # x coordinates array
y_coordinates = [] # y coordinates array
counter = 0
flag = False
temp = ""
for x in f: # search each line in the file
    for letter in x: # search each letter in the line
        if letter == '(': 
            flag = True
            counter = 0
            continue
        if letter == ')':
            flag = False
            y_coordinates.append(temp) # y coordinate before the ')'
            temp = ""
        if(flag):
            if letter.isdigit():
                temp += letter # add digit to a temp string
                continue
            elif letter == ',':
                counter +=1
                if(counter == 1):
                    layers.append(temp) # cell layer before the first ','
                if(counter == 2):
                    x_coordinates.append(temp) # x coordinate before the second ','
                temp = ""

# create two graphs one for layers 1-2 and 2-3
fig, axes12 = plt.subplots(1,1, figsize = (15,10))
fig2, axes34 = plt.subplots(1,1, figsize = (15,10))

# draw the plot in the middle of the cell
x_plot = [int(i)+0.5 for i in x_coordinates] 
y_plot = [int(i)+0.5 for i in y_coordinates]

for i in range(len(x_plot)):
    if layers[i] == '1':
        axes12.scatter(x_plot[i], y_plot[i], s=110, c='blue', marker='s') # layer 1 is drawn in blue
    elif layers[i] == '2':
        axes12.scatter(x_plot[i], y_plot[i], s=110, c='purple', marker='s') # layer 1 is drawn in purple

# axes range
axes12.set_xticks((range(1,51))) 
axes12.set_yticks((range(1,51)))

axes12.grid(True)
axes12.set_title('layer 1 & 2')
fig.tight_layout()
plt.show()

axes34.grid(True)
axes34.set_title('layer 3 & 4')

fig2.tight_layout()
plt.show()
