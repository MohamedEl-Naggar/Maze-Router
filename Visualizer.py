import matplotlib.pyplot as plt
import numpy as np

f = open("out.txt", "r")
layers = []
x_coordinates = []
y_coordinates = []
counter = 0
flag = False
temp = ""
for x in f:
    for letter in x:
        if letter == '(':
            flag = True
            counter = 0
            continue
        if letter == ')':
            flag = False
            y_coordinates.append(temp)
            temp = ""
        if(flag):
            if letter.isdigit():
                temp += letter
                continue
            elif letter == ',':
                counter +=1
                if(counter == 1):
                    layers.append(temp)
                if(counter == 2):
                    x_coordinates.append(temp)
                temp = ""


fig, axes = plt.subplots(1,2, figsize = (15,6))

x_plot = [int(i)+0.5 for i in x_coordinates]
y_plot = [int(i)+0.5 for i in y_coordinates]
for i in range(len(x_plot)):
    if layers[i] == '1':
        axes[0].scatter(x_plot[i], y_plot[i], s=110, c='blue', marker='s')
    elif layers[i] == '2':
        axes[0].scatter(x_plot[i], y_plot[i], s=110, c='purple', marker='s')



#axes[0].scatter(x_coordinates,y_coordinates)
#axes[0].plot(x_coordinates,y_coordinates)

axes[0].set_xticks((range(1,31)))
axes[0].set_yticks((range(1,31)))

axes[0].grid(True)
axes[0].set_title('layer 1 & 2')

axes[1].grid(True)
axes[1].set_title('layer 3 & 4')

fig.tight_layout()
plt.show()
