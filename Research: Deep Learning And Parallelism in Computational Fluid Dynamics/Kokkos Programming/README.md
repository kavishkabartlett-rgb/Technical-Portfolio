# Accessing and setting up the environment

```bash
# Login to discovery (replace username with your actual USC username)
ssh username@discovery.usc.edu
# Create a dedicated folder for this part of the summer program
export d67=/project2/irahbari_1339/$USER/days67/
mkdir -p $d67
cd $d67
# To have the d67 variable always available, you can add it to your $HOME/.bashrc via
echo export d67=/project2/irahbari_1339/\$USER/days67/ >> $HOME/.bashrc

# Copy the `sourceme.sh` file from my shared space with this command:
cp /project2/irahbari_1339/bermejom/shared/sourceme.sh $d67/
# We will follow the instructions given in this `README.md` file whose contents can be seen:
# - with an editor (vim, nano, Visual Studio Code, etc)
# - with a Markdown (pre)viewer (e.g., Opening it in Visual Studio Code and pressing Ctrl+Shift+V)
# - outputting to a terminal with
# cat /project2/irahbari_1339/bermejom/shared/README.md
```

```bash
# Request an interactive allocation on a single GPU node on the `focex-gpu-2` reservation
# use gpu:v100:1 for NVIDIA V100 GPUs
# use gpu:a40:1 for NVIDIA A40 GPUs
salloc -p gpu -N 1 -n 10 --mem=80gb --account=irahbari_1339 --gres=gpu:v100:1 --time=10:00:00 --reservation=focex-gpu-2
# Source the sourceme.sh file previously created, with the CUDA backend
export d67=/project2/irahbari_1339/$USER/days67/
source $d67/sourceme.sh CUDA
# Alternatively, to source with the options for the OpenMP backend
# source $d67/sourceme.sh CUDA
# The active backend can be found by echoing the `backend` environment variable that gets set when sourcing the sourcme.sh file
echo $backend
```

# Installing Kokkos

```bash
cd $d67

# Clone the repository
git clone https://github.com/kokkos/kokkos.git

# Run lscpu and nvidia-smi to find out what CPU and GPU this node has
# For Intel Xeon Gold 6130 -> specify Kokkos_ARCH_SKX=ON
# For NVIDIA V100 -> specify Kokkos_ARCH_VOLTA70=ON
```

Note: - Installing one version of Kokkos with all backends suitable for the targeted architecture can suffice. Here, however, we will install two versions of Kokkos in different directories, one with `Serial`, `OpenMP`, and `Cuda` backends, and another with only `Serial` and `OpenMP` but not `Cuda`. Then, we will switch between them when we create different builds of our application, by changing our environment to point to the right Kokkos build.

## CUDA-enabled version
```bash
source $d67/sourceme.sh CUDA
mkdir $d67/kokkos_build_CUDA/
cd $d67/kokkos_build_CUDA/
cmake $d67/kokkos/ \
  -DCMAKE_CXX_COMPILER=${KOKKOS_SRC}/bin/nvcc_wrapper \
  -DCMAKE_INSTALL_PREFIX=$d67/kokkos_installationDir_CUDA \
  -DKokkos_ENABLE_SERIAL=ON \
  -DKokkos_ENABLE_OPENMP=ON \
  -DKokkos_ENABLE_CUDA=ON \
  -DKokkos_ARCH_SKX=ON \
  -DKokkos_ARCH_VOLTA70=ON \
  -DKokkos_ENABLE_CUDA_LAMBDA=ON \
  -DKokkos_ENABLE_CUDA_RELOCATABLE_DEVICE_CODE=ON \
  -DKokkos_ENABLE_TESTS=ON
# One-liner given below for copy-paste convenience below
# cmake $d67/kokkos/ -DCMAKE_CXX_COMPILER=${KOKKOS_SRC}/bin/nvcc_wrapper -DCMAKE_INSTALL_PREFIX=$d67/kokkos_installationDir_CUDA  -DKokkos_ENABLE_SERIAL=ON -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_SKX=ON -DKokkos_ARCH_VOLTA70=ON -DKokkos_ENABLE_CUDA_LAMBDA=ON -DKokkos_ENABLE_CUDA_RELOCATABLE_DEVICE_CODE=ON -DKokkos_ENABLE_TESTS=ON

# Build (in parallel using 12 processes)
gmake -j12
# Install in the directory specified during the CMake configuration
gmake -j12 install
# Note: If the previous command fails with a "Disk quota exceeded", use the following steps (here commented) to solve it
## 1. Remove the failed installation directory
# rm -rf ../kokkos_installationDir_CUDA/
## 2. Create the installation directory in your $HOME directory
# mkdir $HOME/kokkos_installationDir_CUDA/
## 3. Create a symbolic link to the previously created directory so that the software can be installed there
# ln -s $HOME/kokkos_installationDir_CUDA/ ../
## 4. Install
# gmake -j12 install
```

## OpenMP-only version (only OpenMP backend enabled on host, no GPU)
```bash
source $d67/sourceme.sh OpenMP
mkdir $d67/kokkos_build_OpenMP/
cd $d67/kokkos_build_OpenMP/
cmake $d67/kokkos/ \
  -DCMAKE_CXX_COMPILER=mpic++ \
  -DCMAKE_INSTALL_PREFIX=$d67/kokkos_installationDir_OpenMP \
  -DKokkos_ENABLE_SERIAL=ON \
  -DKokkos_ENABLE_OPENMP=ON \
  -DKokkos_ENABLE_CUDA=OFF \
  -DKokkos_ARCH_SKX=ON \
  -DKokkos_ENABLE_HWLOC=OFF \
  -DKokkos_ENABLE_TESTS=ON
# One-liner given for copy-paste convenience below
# cmake $d67/kokkos/ -DCMAKE_CXX_COMPILER=mpic++ -DCMAKE_INSTALL_PREFIX=$d67/kokkos_installationDir_OpenMP  -DKokkos_ENABLE_SERIAL=ON -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_CUDA=OFF -DKokkos_ARCH_SKX=ON -DKokkos_ENABLE_HWLOC=OFF -DKokkos_ENABLE_TESTS=ON

# Build (in parallel using 12 processes)
gmake -j12
# Install in the directory specified during the CMake configuration
gmake -j12 install
# Note: If the previous command fails with a "Disk quota exceeded", use the following steps (here commented) to solve it
## 1. Remove the failed installation directory
# rm -rf ../kokkos_installationDir_OpenMP/
## 2. Create the installation directory in your $HOME directory
# mkdir $HOME/kokkos_installationDir_OpenMP/
## 3. Create a symbolic link to the previously created directory so that the software can be installed there
# ln -s $HOME/kokkos_installationDir_OpenMP/ ../
## 4. Install
# gmake -j12 install
```

# Installing kokkos-tools

Kokkos-tools will be used for profiling and debugging later in the course, so we install it now to have it available.
A single installation would suffice, but as we did with Kokkos, here we will install two versions for each of the two scenarios we will consider at runtime (`Cuda` and `OpenMP` backends).

```bash
# Clone and install kokkos-tools repository
cd $d67/
git clone https://github.com/kokkos/kokkos-tools.git
```

## CUDA-enabled version
```bash
# CUDA-enabled version
source $d67/sourceme.sh CUDA
mkdir $d67/kokkos-tools_build_CUDA/
cd $d67/kokkos-tools_build_CUDA/
cmake ../kokkos-tools \
  -DCMAKE_CXX_COMPILER=mpic++ \
  -DCMAKE_INSTALL_PREFIX=$d67/kokkos-tools_installationDir_CUDA/ \
  -DKokkos_DIR=${Kokkos_DIR_CUDA} \
  -DKokkosTools_ENABLE_MPI=ON
# One-liner given for copy-paste convenience below
# cmake ../kokkos-tools -DCMAKE_CXX_COMPILER=mpic++ -DCMAKE_INSTALL_PREFIX=$d67/kokkos-tools_installationDir_CUDA/ -DKokkos_DIR=${Kokkos_DIR_CUDA} -DKokkosTools_ENABLE_MPI=ON
gmake -j12 install
```

## OpenMP-only version (only OpenMP backend enabled on host, no GPU)
```bash
# OpenMP-only version
source $d67/sourceme.sh OpenMP
mkdir $d67/kokkos-tools_build_OpenMP/
cd $d67/kokkos-tools_build_OpenMP/
cmake ../kokkos-tools \
  -DCMAKE_CXX_COMPILER=mpic++ \
  -DCMAKE_INSTALL_PREFIX=$d67/kokkos-tools_installationDir_OpenMP/ \
  -DKokkos_DIR=${Kokkos_DIR_OpenMP} \
  -DKokkosTools_ENABLE_MPI=ON
# One-liner given for copy-paste convenience below
# cmake ../kokkos-tools -DCMAKE_CXX_COMPILER=mpic++ -DCMAKE_INSTALL_PREFIX=$d67/kokkos-tools_installationDir_OpenMP/ -DKokkos_DIR=${Kokkos_DIR_OpenMP} -DKokkosTools_ENABLE_MPI=ON
gmake -j12 install
```

# Building our software (exercises) on top of Kokkos

We copy the `exercises` folder from my shared space that contains the software we will be starting from:
```bash
# Copy and build exercises
cd $d67
cp -r /project2/irahbari_1339/bermejom/shared/exercises/ $d67/
```

The `CMakeLists.txt` file within that folder contains the directives that will be passed to the `CMake` build system. The rest of the files are source code for the starting point of each exercise.

We will generate two out-of-source builds of the software, one with CUDA-enabled Kokkos and the other with OpenMP-only Kokkos:

## To use CUDA-enabled Kokkos installation (e.g., to run on GPU):
```bash
# CUDA-enabled
source $d67/sourceme.sh CUDA
mkdir $d67/exercises_build_CUDA/
cd $d67/exercises_build_CUDA/
cmake ../exercises -DCMAKE_CXX_COMPILER=mpic++ -DKokkos_DIR=$Kokkos_DIR
gmake -j12
```

## To use the OpenMP-only Kokkos installation (e.g., to run only on the CPU):
```bash
# OpenMP-only
source $d67/sourceme.sh OpenMP
mkdir $d67/exercises_build_OpenMP/
cd $d67/exercises_build_OpenMP/
cmake ../exercises -DCMAKE_CXX_COMPILER=mpic++ -DKokkos_DIR=$Kokkos_DIR
gmake -j12
```

# Running the exercises

## exercise_1

Start by opening, reviewing, building, and running `exercise_1`.

Building with the CUDA backend active will take `Kokkos::Cuda` as the default execution space:

- Try running the `exercise_1` program first as is. You will notice that each iteration prints a hello message to the screen with its iteration index. Notice that the printed messages do not appear necessarily in sequential order. Iterations are run by different threads of the GPU, with no guarantee of the order in which they run. If we run it again, we may see that the printed messages have a completely different order.

- Next (see `exercise_1_solution_a.cpp`), try changing the policy of the `parallel_for` loop from just `N` to `Kokkos::RangePolicy<Kokkos::Serial>(0,N)`. That way, we tell Kokkos that we want to use the `Serial` execution space rather than the `Cuda` default. Build the program again with `gmake` and notice how the iterations are now printed in order.

- Next, let's change the `RangePolicy` from `Serial` to `OpenMP`. Build (with `gmake`) and run again. The behavior of the `OpenMP` backend can be controlled with environment variables. For example, we can choose to use a desired number of OpenMP threads, for example 16, by invoking:
`export OMP_NUM_THREADS=16`
Run the program again and see what happens to the order in which iteration messages are printed. Are they ordered?
Change from 16 to 1 threads and run again (no need to rebuild with `gmake` since we are just changing a runtime option):
`export OMP_NUM_THREADS=1`
Are the printed messages ordered now? Why?

- Next (see `exercise_1_solution_b.cpp`), let's define a 1D `Kokkos::View` before the parallel for loop, to be of type `int` and of size `N`. Let's try to initialize its elements in the kernel, setting them to the iteration index `i`.
```c++
    Kokkos::View<int*> my_view("my_view", N);
    Kokkos::parallel_for("for", 
      Kokkos::RangePolicy<Kokkos::OpenMP>(0,N),
                         KOKKOS_LAMBDA (std::int64_t i)
    {  
      my_view(i) = i;
      // ...
    });
```
Rebuild and run. Do you get an error? What happened? The view is defined in the default memory space (which is Cuda for our currently active Kokkos build), but we are asking Kokkos explicitly to execute the parallel for loop in the OpenMP execution space, which cannot access the allocated memory on the GPU, and that program fails at runtime. We can either:
  * Change the parallel for loop `RangePolicy` to be executed in the `Cuda` (default) space
  `Kokkos::RangePolicy<Kokkos::Cuda>(0,N), KOKKOS_LAMBDA (std::int64_t i)`
  * Or change the space in which the view is defined to match that of the `parallel_for` kernel:
  `Kokkos::View<int*,OpenMP> my_view("my_view", N);`

When the memory space of the view and execution space of the kernel are compatible, the program runs without errors. What happens if we create the view in the `OpenMP` space, and loop in the `Serial` space? Will it work? Why? 

Take aways so far:
- Parallel kernels will run with no guarantee of the order of iterations. It depends on the execution space and varies from run to run, depending on the thread assignment.
- Without changing the build option, we are able to specify the execution space where we want to run a parallel kernel.
- We can define views and also specify their memory space. Operating with those views in parallel kernels can only be done if there is compatibility between the execution space of the kernel and the memory space of the views.

## exercise_2

In this case, we want to port that code to use Kokkos Views and parallel kernels instead of the C++ arrays and standard for loops.

Start by:
1. Uncommenting the definition of Kokkos Views `x`, `y`, `A`
2. Commenting/deleting the allocation of pointers `x`, `y`, `A`, which are replaced by the views.
3. Convert the use of square brackets `[ i ]` to parentheses `( i )` (and `j`) when accessing the elements of views in the loops. Likewise, for `A[ j * M + i ]`, convert to `A( j, i )`.
4. Comment/delete the memory deallocation of pointers.

Are these changes enough to make it work? No, if we run it in Cuda, but yes if we run it with OpenMP. You could try that, for example, by switching to the OpenMP build via:
```bash
cd ../exercises_build_OpenMP/ # change directory
source $d67/sourceme.sh OpenMP # this step is needed to use the OpenMP-only build of Kokkos we did above
gmake # build the program
./exercise_2_begin # run it; does it work now?
```
Why didn't it work for Cuda, but it does for OpenMP? The reason is that the default memory space in OpenMP is the RAM on the CPU (Host), which we can access with regular for loops on the data array pointed by the views.

Therefore, what was missing for Cuda is that we also need to replace the `for` loops where we initialize the values of the views to use `parallel_for` loops, which will run on the right execution space with access to the default (Cuda) memory space where the view data resides.
Modify those initialization loops and let's go back to Cuda for testing this with:
```bash
cd ../exercises_build_CUDA/ # change directory
source $d67/sourceme.sh CUDA # reload the CUDA-enabled build of Kokkos
gmake # build the program
./exercise_2_begin # run it; does it work now?
```
Once it works, we get as output information about the bandwidth (in GB/s). Let's try and get some insight into the benefits of using the GPU in this case:
- Run the program again with a different problem size and number of iterations:
```bash
./exercise_2_begin -N 20 -M 10 -nrepeat 10
```
Did the bandwidth increase? Why do you think that is?
As we increase the problem size, we are able to fill the memory lines of the GPU and better utilize it for parallel computations, thus gaining bandwidth.
Let's conduct a parametric sweep on the number of rows `N` (between 1^2 and 2^20, in increments of 2^2) and print the bandwidth results for each run:
```bash
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 10 | grep bandwidth;done
```
On Cuda, we will see something like this:
```bash
  N( 2 ) M( 1024 ) nrepeat ( 10 ) problem( 0.024592 MB ) time( 0.00118441 s ) bandwidth( 0.20763 GB/s )
  N( 8 ) M( 1024 ) nrepeat ( 10 ) problem( 0.073792 MB ) time( 0.00120495 s ) bandwidth( 0.612408 GB/s )
  N( 32 ) M( 1024 ) nrepeat ( 10 ) problem( 0.270592 MB ) time( 0.00123247 s ) bandwidth( 2.19553 GB/s )
  N( 128 ) M( 1024 ) nrepeat ( 10 ) problem( 1.05779 MB ) time( 0.0012763 s ) bandwidth( 8.28798 GB/s )
  N( 512 ) M( 1024 ) nrepeat ( 10 ) problem( 4.20659 MB ) time( 0.00130182 s ) bandwidth( 32.3132 GB/s )
  N( 2048 ) M( 1024 ) nrepeat ( 10 ) problem( 16.8018 MB ) time( 0.00239362 s ) bandwidth( 70.1942 GB/s )
  N( 8192 ) M( 1024 ) nrepeat ( 10 ) problem( 67.1826 MB ) time( 0.0026869 s ) bandwidth( 250.038 GB/s )
  N( 32768 ) M( 1024 ) nrepeat ( 10 ) problem( 268.706 MB ) time( 0.00447623 s ) bandwidth( 600.294 GB/s )
  N( 131072 ) M( 1024 ) nrepeat ( 10 ) problem( 1074.8 MB ) time( 0.0151637 s ) bandwidth( 708.796 GB/s )
  N( 524288 ) M( 1024 ) nrepeat ( 10 ) problem( 4299.17 MB ) time( 0.0619707 s ) bandwidth( 693.742 GB/s )
```
Notice the increase of bandwidth in the last column, as we increase the problem size, getting us closer to the max bandwidth of the NVIDIA V100 (900 GB/s).

Let's try now changing the size again, but instead with a parametric sweep in the number of columns, `M`, rather than in the number of rows `N`:
```bash
for M in `seq 2 2 20`;do ./exercise_2_begin -N 10 -M $M -nrepeat 10 | grep bandwidth;done
```
The result is a bit different now, isn't it?
```
  N( 1024 ) M( 4 ) nrepeat ( 10 ) problem( 0.040992 MB ) time( 0.00023399 s ) bandwidth( 1.75187 GB/s )
  N( 1024 ) M( 16 ) nrepeat ( 10 ) problem( 0.139392 MB ) time( 0.000235522 s ) bandwidth( 5.91843 GB/s )
  N( 1024 ) M( 64 ) nrepeat ( 10 ) problem( 0.532992 MB ) time( 0.000291634 s ) bandwidth( 18.2761 GB/s )
  N( 1024 ) M( 256 ) nrepeat ( 10 ) problem( 2.10739 MB ) time( 0.000500214 s ) bandwidth( 42.1298 GB/s )
  N( 1024 ) M( 1024 ) nrepeat ( 10 ) problem( 8.40499 MB ) time( 0.00230695 s ) bandwidth( 36.4333 GB/s )
  N( 1024 ) M( 4096 ) nrepeat ( 10 ) problem( 33.5954 MB ) time( 0.00886156 s ) bandwidth( 37.9114 GB/s )
  N( 1024 ) M( 16384 ) nrepeat ( 10 ) problem( 134.357 MB ) time( 0.0349496 s ) bandwidth( 38.4431 GB/s )
  N( 1024 ) M( 65536 ) nrepeat ( 10 ) problem( 537.403 MB ) time( 0.139773 s ) bandwidth( 38.4484 GB/s )
  N( 1024 ) M( 262144 ) nrepeat ( 10 ) problem( 2149.59 MB ) time( 0.536301 s ) bandwidth( 40.0818 GB/s )
  N( 1024 ) M( 1048576 ) nrepeat ( 10 ) problem( 8598.33 MB ) time( 2.10605 s ) bandwidth( 40.8268 GB/s )
```
We are far from reaching anything close to the bandwidth of the GPU.

Let's now use `nvidia-smi` to diagnose our GPU utilization.
To do so, follow these steps:
1. On the compute node you are working with the interactive session, run
```bash
hostname
```
to get the name of the host (identifying the compute node). Let's suppose you get `d13-09` (change this in the next steps according to the actual name you get from running the previous command).
2. Open a separate terminal, `ssh` into discovery, and, from the login node, ssh now into the compute node you have. In this example:
```bash
ssh d13-09
```
3. Once you are logged in to that compute node, run:
```bash
source $d67/sourceme.sh CUDA # to source the CUDA environment
```
4. Finally, launch the `nvidia-smi` tool in a way we refresh the output every tenth of a second, with:
```bash
watch -n 0.1 nvidia-smi
```
5. You will see something like:
```
Wed Jun 19 07:02:53 2024
+---------------------------------------------------------------------------------------+
| NVIDIA-SMI 535.129.03             Driver Version: 535.129.03   CUDA Version: 12.2     |
|-----------------------------------------+----------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |         Memory-Usage | GPU-Util  Compute M. |
|                                         |                      |               MIG M. |
|=========================================+======================+======================|
|   0  Tesla V100-PCIE-32GB           Off | 00000000:3B:00.0 Off |                    0 |
| N/A   27C    P0              36W / 250W |      0MiB / 32768MiB |      1%      Default |
|                                         |                      |                  N/A |
+-----------------------------------------+----------------------+----------------------+

+---------------------------------------------------------------------------------------+
| Processes:                                                                            |
|  GPU   GI   CI        PID   Type   Process name                            GPU Memory |
|        ID   ID                                                             Usage      |
|=======================================================================================|
|  No running processes found                                                           |
+---------------------------------------------------------------------------------------+
```
5. If possible, place the two terminals next to each other (e.g., top half and bottom half of your screen; otherwise you will have to switch back and forth between terminals in step 6)
6. From the original terminal where you had the interactive session, run again the first parametric sweep, now increasing the number of iterations so that we have a longer runtime to better see the GPU utilization.
```bash
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 5000| grep bandwidth;done
```
7. As the parametric sweep runs, monitor the output of `nvidia-smi` in the other terminal for metrics such as the utilization of the GPU (`GPU-Util`), the `GPU Memory Usage`, the power usage (`Pwr:Usage/Cap`), and the temperature (`Temp`). You will also notice that the program you are executing appears as the running process at the bottom of the report. When it stops execution, everything will go back to idle.
8. One could play with the sweep parameters, number of repetitions, etc. to see the GPU behavior.

Let's switch to OpenMP. To do so, let's press `Ctrl+C` in terminal where we had `nvidia-smi` running and then run the following command in both terminals:
```bash
source $d67/sourceme.sh OpenMP
```
In the terminal where you had `nvidia-smi` running, execute:
```bash
htop
```
That will bring a continuous monitor of CPU utilization. At the top, you will see progress bars indicating the utilization of each core (1 to 32), and the memory utilization. At the bottom you will see the processes running in that node. You can type `u` followed by your username to only see processes that belong to you.
As we monitor that panel, we will see up to 16 cores firing up from our utilization of the node, and possibly the other 16 cores firing up from another classmate who may be sharing the node with us, as we have split the resources of each node to have two students accessing them (16 CPU cores out of the 32, and 1 GPU out of the 2 that we have in each node).

Then, in the terminal with the interactive allocation (original terminal), follow these steps:
```bash
# Change to the directory where the executable is located:
cd $d67/exercises_build_OpenMP/
gmake # build the updated code with the latest modifications
```
Run the program with:
```bash
./exercise_2_begin -N 14 -M 10 -nrepeat 100
```
Now wee see much more information in the output, such as the following example:
```bash
OPENMP DISPLAY ENVIRONMENT BEGIN
  _OPENMP = '201511'
  OMP_DYNAMIC = 'FALSE'
  OMP_NESTED = 'FALSE'
  OMP_NUM_THREADS = '16'
  OMP_SCHEDULE = 'DYNAMIC'
  OMP_PROC_BIND = 'SPREAD'
  OMP_PLACES = '{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15}'
  OMP_STACKSIZE = '0'
  OMP_WAIT_POLICY = 'PASSIVE'
  OMP_THREAD_LIMIT = '4294967295'
  OMP_MAX_ACTIVE_LEVELS = '1'
  OMP_NUM_TEAMS = '0'
  OMP_TEAMS_THREAD_LIMIT = '0'
  OMP_CANCELLATION = 'FALSE'
  OMP_DEFAULT_DEVICE = '0'
  OMP_MAX_TASK_PRIORITY = '0'
  OMP_DISPLAY_AFFINITY = 'TRUE'
  OMP_AFFINITY_FORMAT = 'level %L thread %i affinity %A'
  OMP_ALLOCATOR = 'omp_default_mem_alloc'
  OMP_TARGET_OFFLOAD = 'DEFAULT'
OPENMP DISPLAY ENVIRONMENT END
  User N is 16384
  User M is 1024
  Total size S = 16777216 N = 16384 M = 1024
level 1 thread 0x7f6f49d6c000 affinity 0
level 1 thread 0x7f6f450aa700 affinity 1
level 1 thread 0x7f6f448a9700 affinity 2
level 1 thread 0x7f6f440a8700 affinity 3
level 1 thread 0x7f6f438a7700 affinity 4
level 1 thread 0x7f6f430a6700 affinity 5
level 1 thread 0x7f6f428a5700 affinity 6
level 1 thread 0x7f6f420a4700 affinity 7
level 1 thread 0x7f6f418a3700 affinity 8
level 1 thread 0x7f6f410a2700 affinity 9
level 1 thread 0x7f6f408a1700 affinity 10
level 1 thread 0x7f6f400a0700 affinity 11
level 1 thread 0x7f6f3f89f700 affinity 12
level 1 thread 0x7f6f3f09e700 affinity 13
level 1 thread 0x7f6f3e89d700 affinity 14
level 1 thread 0x7f6f3e09c700 affinity 15
  Computed result for 16384 x 1024 is 16777216.000000
  N( 16384 ) M( 1024 ) nrepeat ( 100 ) problem( 134.357 MB ) time( 0.19127 s ) bandwidth( 70.2446 GB/s )
```
The important information for our purposes is annotated below:
```bash
OMP_NUM_THREADS = '16' # -> we are using 16 OpenMP threads
# they are mapped to cores 0-15; the other person sharing this node will have the remaining cores 16-31
OMP_PLACES = '{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15}'
# Each OpenMP thread, running independently, is mapped to one CPU core following the affinity described below
level 1 thread 0x7f6f49d6c000 affinity 0
```
We also see the bandwidth at the end of the report:
```bash
  N( 16384 ) M( 1024 ) nrepeat ( 100 ) problem( 134.357 MB ) time( 0.19127 s ) bandwidth( 70.2446 GB/s )
```
Let's now run the same two parametric sweep as we did for the GPU previously, with an extra to clean up the output:
- In `N`:
```bash
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 10 2> /dev/null | grep bandwidth;done
```
- In `M`:
```bash
for M in `seq 2 2 20`;do ./exercise_2_begin -N 10 -M $M -nrepeat 10 2> /dev/null | grep bandwidth;done
```
Did you notice any difference in the bandwidth results we are getting, comparing OpenMP and Cuda backends?
Is there much of a difference in performance when we sweep in `N` or `M` for OpenMP, as we found for Cuda?

Let's increase the number of repetitions, so that we can use the `htop` monitor to check for CPU core and memory utilization. Run the following parametric sweep while keeping an eye on the `htop`.
```bash
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 100 2> /dev/null | grep bandwidth;done
```

As we did before, we can control at runtime the number of OpenMP threads we would like to use.
- Try changing the number of threads to lower value and run again the parametric sweep
```bash
export OMP_NUM_THREADS=4 # for a lower count
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 100 2> /dev/null | grep bandwidth;done
# keep an eye on the `htop` display
# then change again the number of threads to oversubscribe the cores
export OMP_NUM_THREADS=32 # for a higher count
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 100 2> /dev/null | grep bandwidth;done
# then change again the number of threads to oversubscribe the cores
export OMP_NUM_THREADS=256 # for an even higher count
for N in `seq 1 2 20`;do ./exercise_2_begin -N $N -M 10 -nrepeat 100 2> /dev/null | grep bandwidth;done

# keep an eye on the `htop` display
```
What happens to the bandwidth with a lower thread count?
What happens to the bandwidth and the `htop` monitor with a higher thread count, possibly exceeding the number of cores (16) we have allocated?

## exercise_3




## Sequence of exercises from Kokkos lectures 2 to 4: exercise_[24]_begin

* Folow instructions in headers.

## exercise_mpi_kokkos_timers_profiling

### Run exercise_mpi_kokkos_timers_profiling with CUDA:
```bash
source $d67/sourceme.sh CUDA
cd $d67/exercises_build_CUDA/
mpirun -np 2 --bind-to core ./exercise_mpi_kokkos_timers_profiling -s 1400000000 -i 10
```

Suggestions:
- Test how much the size of the views can be increased before getting memory allocation errors.
- To see the usage of the GPU cards in real time:
```bash
# Get the hostname from the interactive allocation by running
hostname
# which will return something like d13-09.hpc.usc.edu but with a different
# node ID in the beginning.
# Then, from a separate terminal, ssh into that node ID, in this example with:
ssh d13-09
# source the sourceme.sh file
source $d67/sourceme.sh CUDA
# Run the command
watch -n 0.1 nvidia-smi
# which will execute and show the output of the `nvidia-smi` command every 0.1s
```

### Run exercise_mpi_kokkos_timers_profiling with OpenMP:

```bash
source $d67/sourceme.sh OpenMP
# Note that the sourced file already sets the following OpenMP variables:
export OMP_PROC_BIND=spread # recommended by Kokkos
export OMP_PLACES=threads # recommended by Kokkos
export OMP_DISPLAY_AFFINITY=true # displays runtime info about thread affinity
export OMP_DISPLAY_ENV=true # displays runtime OpenMP environment info
# Change to the directory where the executable is located:
cd $d67/exercises_build_OpenMP/
# Run via mpirun, selecting the number of MPI tasks and OpenMP threads through
# the mpirun arguments. Note the use of the "--bind-to none" option when using
# the OpenMP backend, rather than the "--bind-to core" option, so that the MPI
# task (-np 1) can access all the cores of the host, and then Kokkos will map
# the threads to the available cores
mpirun -np 1 -x OMP_NUM_THREADS=32 --bind-to none exercise_mpi_kokkos_timers_profiling -s 1400000000 -i 10
```

Suggestions:
- Test how much the size of the views can be increased before getting memory allocation errors.
- To see the usage of the CPU cores in real time:
```bash
# Get the hostname from the interactive allocation by running
hostname
# which will return something like d13-09.hpc.usc.edu but with a different
# node ID in the beginning.
# Then, from a separate terminal, ssh into that node ID, in this example with:
ssh d13-09
# source the sourceme.sh file
source $d67/sourceme.sh OpenMP
# Run the command
htop
# which will show the CPU cores and memory utilization with progress bars,
# and the processes running in the system below
```
- Try different combinations of MPI and OpenMP parallelism

- Use Kokkos-tools to profile the program execution, identifying several
  performance metrics:
```bash
export KOKKOS_TOOLS_LIBS=${kokkos_tools_DIR}/lib64/libkp_space_time_stack.so
```
- Identify which Kokkos kernels take most of the execution time
- Try to change the code to speed-up time to solution, by relocating some
  operations. Compare the output
