from mpi4py import MPI
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

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
nproc = comm.Get_size()

a = 0.0
b = 1.0
n = 1000
h=(b-a)/n  # fixed value

def trapez(a, b, n, h):
        integral = h*(f(a) + f(b))/2.0
        x = a
        for i in range(1, int(n)):
                x = x + h
                integral = integral + h*f(x)
        return integral


dest = 0
total = -1.0

local_n = n/nproc  # fixed value
local_a = a + rank*local_n*h
local_b = local_a + local_n*h
integral = trapez(local_a, local_b, local_n, h)

if rank == 0:
	total = integral
	for source in range(1,nproc):
		integral = comm.recv(source=source)
		print("Proc ", rank, " <- ", source, ", ",integral, "\n")
		total = total + integral

else:
	print("Proc ", rank, " -> ", dest, ", ", integral, "\n")
	comm.send(integral, dest=0)

if (rank ==0):
	print("With n = ", n, " trapezoids, ")
	print("integral from ", a, "to", b, " = ", total, "\n") 


MPI.Finalize

#print(trapez(a,b,n,h))
#draw_graph(ax,Tp)
