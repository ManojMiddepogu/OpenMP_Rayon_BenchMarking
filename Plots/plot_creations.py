import matplotlib.pyplot as plt

x = [1, 2, 4, 8, 16, 32, 64]
xy1 = [0.000009, 0.000080226, 0.00015, 0.00026, 0.00023, 0.00093,0.00108]
xy2 = [0.000038, 0.000391763, 0.00044, 0.00068, 0.00197, 0.0028, 0.00469]

y1 = [1.00022, 1.000716328, 1.00056, 1.00097, 1.00118, 1.0021, 1.0036]
y2 = [1.0004, 1.000882, 1.00468, 1.0018, 1.0048, 1.009, 1.0132]

for i in range(7):
    y1[i] = y1[i]-xy1[i]-1
    y2[i] = y2[i]-xy2[i]-1

# x = x[:3]
# y1 = y1[:3]
# y2 = y2[:3]
label_x = "#Threads"
label_y = "synchronization Times"

# Plotting the first line with color 'blue'
plt.plot(x, y1, label="Rayon", color='blue')

# Plotting the second line with color 'green'
plt.plot(x, y2, label="OpenMP", color='green')

# Adding labels and title
plt.xlabel(label_x)
plt.ylabel(label_y)
plt.title(f"OpenMP vs Rayon")

# Adding a legend with location 'upper left'
plt.legend(loc='upper left')

# Save the plot with a specific name
plt.savefig('sync_times.png')

# Display the plot
plt.show()