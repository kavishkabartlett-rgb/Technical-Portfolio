# USC-FOCEX 2025 Summer Program
# -----------------------------

# Purge all loaded environment modules
module purge
# Load required environment modules
#ml load gcc/12.3.0 openmpi/4.1.5 python/3.12.2 cmake git vim htop
ml load gcc openmpi python cmake git vim htop

# Select backend
backend=CUDA # default backend set to CUDA
usetools=0 # default kokkostools set to 0 (no use of kokkos-tools)
if [[ $# -eq 1 ]]; then
  backend=$1 # Set to passed argument
elif [[ $# -eq 2 ]]; then
  backend=$1
  usetools=$2
else
  echo "Usage: source /path/to/sourcem.sh [backend] [usetools]"
  echo "where: backend is CUDA or OpenMP"
  echo "       usetools is 0 (no kokkos-tools) or 1 (use kokkos-tools)"
  return 1
fi

#export d67=$HOME/focex2025/days67/
export d67=/project2/irahbari_1339/$USER/days67/
echo "Setting environment, Kokkos backend $backend and kokkos-tools=$usetools"

if [[ $backend == "CUDA" ]]; then
  # Load cuda environment module
  module load cuda 
  # Setup several OpenMP variables
  export OMP_PROC_BIND=spread # recommended by Kokkos
  export OMP_PLACES=threads # recommended by Kokkos
  unset OMP_DISPLAY_AFFINITY
  unset OMP_DISPLAY_ENV
elif [[ $backend == "OpenMP" ]]; then
  # Setup several OpenMP variables
  export OMP_PROC_BIND=spread # recommended by Kokkos
  export OMP_PLACES=threads # recommended by Kokkos
  export OMP_DISPLAY_AFFINITY=true # displays runtime thread affinity info 
  export OMP_DISPLAY_ENV=true # displays runtime OpenMP environment info
fi

# Export Kokkos variables
export KOKKOS_SRC=$d67/kokkos/
export Kokkos_DIR=$d67/kokkos_installationDir_$backend/lib64/cmake/Kokkos/
export kokkos_tools_DIR=$d67/kokkos-tools_installationDir_$backend/
if [[ $usetools == 1 ]]; then
  # Define KOKKOS_TOOLS_LIBS
  export KOKKOS_TOOLS_LIBS=${kokkos_tools_DIR}/lib64/libkp_space_time_stack.so
else
  unset KOKKOS_TOOLS_LIBS
fi
