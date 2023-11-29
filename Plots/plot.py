import matplotlib.pyplot as plt

# 2-level parallel
# x = [1, 2, 4, 8, 16, 24, 32, 40, 48, 64, 80]
# y1 = [17.6, 9.32, 4.78, 2.5, 1.53, 1.18, 1.01, 0.987, 0.885, 0.82, 0.812]
# y2 = [15.8, 8.7, 4.3, 2.6, 1.51, 1.3, 1.24, 1.09, 1.07, 0.86, 0.865]

# block parallel
# x = [1, 2, 4, 8, 16, 24, 32, 40, 48, 64, 80]
# y1 = [22, 11.5, 7.9, 3.7, 2.2, 1.4, 1.45, 1.2, 1.1, 1.03, 0.92]
# y2 = [2.89, 1.56, 0.78, 0.4, 0.22, 0.2, 0.19, 0.194, 0.176, 0.169, 0.161]


# strassen
# x = [1, 2, 4, 8, 16, 24, 32, 40, 48, 64, 80]
# y1 = [4.1, 2.34, 1.1, 1.2, 1.37, 1.3, 1.32, 1.33, 1.35, 1.4, 1.4]
# y2 = [4.82, 2.93, 1.86, 1.78, 1.22, 1.22, 1.20, 1.12, 1.1, 1.16, 1.2]

# linear
x = [1, 2, 4, 8, 16, 24, 32, 40, 48, 64, 80]
y1 = [4.1, 2.34, 1.1, 1.2, 1.37, 1.3, 1.32, 1.33, 1.35, 1.4, 1.4]
y2 = [4.82, 2.93, 1.86, 1.78, 1.22, 1.22, 1.20, 1.12, 1.1, 1.16, 1.2]


for i in range(1,len(x)):
    y1[i] = y1[0]/(x[i]*y1[i])
    y2[i] = y2[0]/(x[i]*y2[i]) 

x = x[1:]
y1 = y1[1:]
y2 = y2[1:]
label_x = "#Threads"
label_y = "Efficiency"

# Plotting the first line with color 'blue'
plt.plot(x, y1, label="Rayon", color='blue')

# Plotting the second line with color 'green'
plt.plot(x, y2, label="OpenMP", color='green')

# Adding labels and title
plt.xlabel(label_x)
plt.ylabel(label_y)
plt.title(f"size=1024*1024, OpenMP vs Rayon")

# Adding a legend with location 'upper left'
plt.legend(loc='upper left')

# Save the plot with a specific name
plt.savefig('efficiency_linear.png')

# Display the plot
plt.show()