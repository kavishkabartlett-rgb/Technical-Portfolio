#This method tests collective communication operations 

from mpi4py import MPI

comm = MPI.COMM_WORLD #Sets up a global communicator with all processors  
rank = comm.Get_rank() #All instances of the process will have their own unique rank number
size = comm.Get_size()

# Broadcast
if rank == 0: #Only runs if the current process being run is of rank 0 
    data = {'key1': 1, 'key2': 2}
else:
    data = None

data = comm.bcast(data, root=0)
print(f"Process {rank} received data: {data}")


# Gather
#data = rank
#gathered_data = comm.gather(data, root=0)

#if rank == 0:
#    print(f"Process {rank} gathered data: {gathered_data}")


# Scatter

#if rank == 0:
#    data = [i for i in range(size)]
#else:
#    data = None

#data = comm.scatter(data, root=0)
#print(f"Process {rank} received data: {data}")


# All Gather

#data = rank
#allgathered_data = comm.allgather(data)
#print(f"Process {rank} allgathered data: {allgathered_data}")


# Reduce

#data = rank
#reduced_data = comm.reduce(data, op=MPI.SUM, root=0)
#if rank == 0:
#    print(f"Process {rank} reduced data: {reduced_data}")

# All reduce

#data = rank
#allreduced_data = comm.allreduce(data, op=MPI.SUM)
#print(f"Process {rank} allreduced data: {allreduced_data}")

MPI.Finalize
