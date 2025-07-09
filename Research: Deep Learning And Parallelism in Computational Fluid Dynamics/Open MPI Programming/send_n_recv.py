from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

print(f'Total {size} of processes')
d_tag=11
data = None

if rank == 0:
	data='Hello MPI World'
	print(f'Rank {rank}: Sending data to rank 1 with tag {d_tag}')
	comm.send(data,dest=1,tag=d_tag)

elif rank == 1:
	msg=comm.recv(source=0, tag=d_tag)
	print(f'Rank {rank}: Got data: {msg} from rank 0 with tag {d_tag}')

print(f'Rank {rank} shutting down with data={msg}.')

MPI.Finalize
