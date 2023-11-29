import matplotlib.pyplot as plt

# Threads = 64 2-level
# x = [16, 32, 64, 128, 256, 512, 1024, 2048]
# y1 = [0.006, 0.01, 0.019, 0.029, 0.039, 0.096, 0.838, 6.5]
# y2 = [0.009, 0.008, 0.011, 0.009, 0.018, 0.079, 0.96, 7.09]

# Threads = 8 2-level
# x = [16, 32, 64, 128, 256, 512, 1024, 2048]
# y1 = [0.0009, 0.0001, 0.0010, 0.0039, 0.014, 0.153, 2.4, 26.9]
# y2 = [0.0007, 0.008, 0.011, 0.009, 0.018, 0.079, 0.96, 26.1]

# Threads = 64 block
# x = [16, 32, 64, 128, 256, 512, 1024, 2048]
# y1 = [0.003, 0.004, 0.0053, 0.0076, 0.016, 0.12, 1.17, 8.97]
# y2 = [0.009, 0.008, 0.010, 0.011, 0.012, 0.041, 0.188, 1.02]

# Threads = 64 strassen
# x = [16, 32, 64, 128, 256, 512, 1024, 2048]
# y1 = [0.00004, 0.0001, 0.0017, 0.008, 0.058, 0.28, 1.5, 6.96]
# y2 = [0.009, 0.012, 0.009, 0.029, 0.05, 0.2, 1.0, 7.63]

# Threads = 64 linear
x = [16, 32, 64, 128, 256, 512, 1024, 1500, 1800, 2047, 2048]
y1 = [0.029, 0.03, 0.044, 0.1, 0.29, 1.04, 2.18, 12, 18, 26, 54]
y2 = [0.010, 0.0108, 0.011, 0.029, 0.014, 0.04, 1.09, 1.22,  2.91, 3.64, 43]

# x = x[:3]
# y1 = y1[:3]
# y2 = y2[:3]
label_x = "size of the matrix"
label_y = "Wall-time(ms)"

# Plotting the first line with color 'blue'
plt.plot(x, y1, label="Rayon", color='blue')

# Plotting the second line with color 'green'
plt.plot(x, y2, label="OpenMP", color='green')

# Adding labels and title
plt.xlabel(label_x)
plt.ylabel(label_y)
plt.title(f"Threads = 64, OpenMP vs Rayon")

# Adding a legend with location 'upper left'
plt.legend(loc='upper left')

# Save the plot with a specific name
plt.savefig('t_64_size_linear.png')

# Display the plot
plt.show()