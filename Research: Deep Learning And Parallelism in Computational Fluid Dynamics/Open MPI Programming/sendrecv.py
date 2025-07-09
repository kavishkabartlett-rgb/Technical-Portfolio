from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# Each process creates a message to send
send_data = f"Hello from rank {rank}"

# Prepare the buffer to receive data
recv_data = None

# Rank 0 sends to 1 and receives from 1
# Rank 1 sends to 0 and receives from 0
if rank == 0:
    dest = 1
    source = 1
elif rank == 1:
    dest = 0
    source = 0

# Perform the sendrcev operation
recv_data = comm.sendrecv(sendobj=send_data, dest=dest, sendtag=11, source=source, recvtag=11)

print(f"Process {rank} received: {recv_data}")


MPI.Finalize
