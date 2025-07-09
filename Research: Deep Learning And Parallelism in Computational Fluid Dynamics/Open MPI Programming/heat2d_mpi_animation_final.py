# Parallel Computing for 2d Heat Equation

from mpi4py import MPI
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# MPI setup
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

## Parameters ##
alpha = 1.11e-4 #Thermal Diffusivity
nx, ny_total = 50, 50 #Global Grid size
nt = 2000 #Number of time steps 
Lx, Ly = 1.0, 1.0
dx = Lx / (nx - 1) #Step size for x, y, and time 
dy = Ly / (ny_total - 1)
dt = 0.1

assert alpha * dt * (1 / dx**2 + 1 / dy**2) < 0.5, "Stability condition violated!"

#### 1D DOMAIN DECOMPOSITION ####
rows_per_rank = ny_total // size what 
remainder = ny_total % size

if rank < remainder: #Ensures that only the beginning ranks get assigned extra rows 
    start_y = rank * (rows_per_rank + 1) #Marks the beginning and end of this rank's chunk 
    end_y = start_y + rows_per_rank + 1
else:
    start_y = rank * rows_per_rank + remainder
    end_y = start_y + rows_per_rank

local_ny = end_y - start_y #Number of rows in the subdomain 

#### Creating Ghost Rows #####
T_local = np.zeros((local_ny + 2, nx, nt)) #local_ny + 2 to include ghost row on top and on bottom
T_local[:, :, 0] = 100

for k in range(nt - 1):
    if rank > 0:
        send_buf = np.ascontiguousarray(T_local[1, :, k])
        recv_buf = np.ascontiguousarray(T_local[0, :, k])
        MPI.COMM_WORLD.Sendrecv(send_buf, rank - 1, 0, recv_buf, rank - 1, 1)
        T_local[0, :, k] = recv_buf
    if rank < size - 1:
        send_buf = np.ascontiguousarray(T_local[-2, :, k])
        recv_buf = np.ascontiguousarray(T_local[-1, :, k])
        MPI.COMM_WORLD.Sendrecv(send_buf, rank + 1, 1, recv_buf, rank + 1, 0)
        T_local[-1, :, k] = recv_buf

    if start_y == 0:
        T_local[1, :, k] = 400
    if end_y == ny_total:
        T_local[-2, :, k] = 400
    T_local[:, 0, k] = 400
    T_local[:, -1, k] = 100

    for i in range(1, local_ny + 1):
        for j in range(1, nx - 1):
            T_local[i, j, k + 1] = T_local[i, j, k] + alpha * dt * (
                (T_local[i + 1, j, k] - 2 * T_local[i, j, k] + T_local[i - 1, j, k]) / dy**2 +
                (T_local[i, j + 1, k] - 2 * T_local[i, j, k] + T_local[i, j - 1, k]) / dx**2
            )

T_time_series_local = T_local[1:-1, :, :]
all_time_series = comm.gather(T_time_series_local, root=0)

if rank == 0:
    T_all = np.vstack(all_time_series)
    x = np.linspace(0, Lx, nx)
    y = np.linspace(0, Ly, ny_total)
    X, Y = np.meshgrid(x, y)

    fig, ax = plt.subplots(figsize=(6, 5))
    cf = ax.contourf(X, Y, T_all[:, :, 0], levels=50, cmap='hot', vmin=20, vmax=400)
    fig.colorbar(cf, ax=ax, label='Temperature')

    def update(frame):
        ax.clear()
        cf = ax.contourf(X, Y, T_all[:, :, frame], levels=50, cmap='hot', vmin=20, vmax=400)
        ax.set_title(f"Time = {frame * dt:.1f}s")
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        return []

    ani = animation.FuncAnimation(fig, update, frames=range(0, nt, 50), blit=False)

    try:
        ani.save("heat2d_mpi_animation_final.mp4", writer='ffmpeg', fps=10)
    except Exception:
        ani.save("heat2d_mpi_animation_final.gif", writer='pillow', fps=10)

    plt.show()