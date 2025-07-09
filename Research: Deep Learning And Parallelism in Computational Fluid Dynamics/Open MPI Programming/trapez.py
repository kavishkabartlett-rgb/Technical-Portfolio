import matplotlib.pyplot as plt
import math

# Draw the graph
def draw_graph(x,y,marker='.'):
	plt.plot(x,y)
	plt.xlabel('x')
	plt.ylabel('f(x)')
	plt.title('Numerical Integration of f(x) - Trapezoidal Rule')
	plt.show()
	plt.savefig('trapezoid_graph.png')	   


def f(x):
	return x*x

a = 0.0
b = 1.0
n = 1000
h = (b-a)/n

ax = []
Tp = []
    
def trapez(a, b, n, h):

	integral = h*(f(a) + f(b))/2.0
    
	x = a
	for i in range(1, int(n)):
		x = x + h
		integral = integral + h*f(x)
    
		ax.append(x)
		Tp.append(integral)
        
	return integral


if __name__ == '__main__':
    trapez(a,b,n,h)
    print(trapez(a,b,n,h))
draw_graph(ax,Tp)
