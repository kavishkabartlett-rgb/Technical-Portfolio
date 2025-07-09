from mpi4py import MPI
import numpy as np

def f(x):
    return x**2

# Create x values for plotting
x_vals = np.linspace(0, 1, 100)
y_vals = f(x_vals)

plt.figure(figsize=(6,4))
plt.plot(x_vals, y_vals, label='f(x) = x^2', color='blue')
plt.title("Function f(x) = x^2")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.grid(True)
plt.legend()
plt.show()

# Integration bounds
a = 0
b = 1

# Number of subintervals
n = 1000

# Width of each subinterval
h = (b - a) / n

# x values
x = np.linspace(a, b, n+1)

# Apply trapezoidal rule
integral = (h/2) * (f(x[0]) + 2 * np.sum(f(x[1:n])) + f(x[n]))

print(f"Estimated integral of f(x) = x^2 from 0 to 1 using trapezoidal rule: {integral:.6f}")
