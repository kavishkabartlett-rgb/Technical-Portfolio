

from mpi4py import MPI

comm = MPI.COMM_WORLD
rank=comm.Get_rank()
print(f'Rank {rank} starting up ...')

a_tag=3
b_tag=7
data_a = None
data_b = None

if rank == 0:
	data_a = 123

	print(f'Rank {rank}: Waiting for data_b from rank 1 with tag {b_tag}')
	data_r=comm.recv(data_b, source=1, tag=b_tag)
	print(f'Rank {rank}: Got data: {data_r} from rank 1 with tag {b_tag}')

	print(f'Rank {rank}: Sending data_a to rank 1 with tag {a_tag}')
	comm.send(data_a,dest=1,tag=a_tag)

#	print(f'Rank {rank}: Waiting for data_b from  rank 1 with tag {b_tag}')
#	data_r=comm.recv(data_b, source=1, tag=b_tag)
#	print(f'Rank {rank}: Got data: {data_r} from rank 1 with tag {a_tag}')

if rank == 1:
	data_b = 789

#	print(f'Rank {rank}: Sending data_b to rank 0 with tag {b_tag}')
#	comm.send(data_b,dest=0,tag=b_tag)

	print(f'Rank {rank}: Waiting for data_a from rank 0 with tag {a_tag}')
	data_r=comm.recv(data_a, source=0, tag=a_tag)
	print(f'Rank {rank}: Got data: {data_r} from rank 0 with tag {a_tag}')

	print(f'Rank {rank}: Sending data_b to rank 0 with tag {b_tag}')
	comm.send(data_b,dest=0,tag=b_tag)

print(f'Rank {rank} shutting down')

#MPI.Finalize
