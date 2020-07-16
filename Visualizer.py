import matplotlib.pyplot as plt
import numpy as np
fig, axes = plt.subplots(1,2, figsize = (15,6))

x_coordinates = [1, 1, 3, 3, 5] 
y_coordinates = [2, 5, 5, 10, 10]
axes[0].scatter(x_coordinates,y_coordinates)
axes[0].plot(x_coordinates,y_coordinates)

x_coordinates = [10, 10, 30] 
y_coordinates = [20, 5, 5]
axes[0].scatter(x_coordinates,y_coordinates)
axes[0].plot(x_coordinates,y_coordinates)

axes[0].set_xticks((range(1,31)))
axes[0].set_yticks((range(1,31)))

axes[0].grid(True)
axes[0].set_title('layer 1 & 2')

axes[1].grid(True)
axes[1].set_title('layer 3 & 4')

fig.tight_layout()
plt.show()
