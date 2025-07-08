# USC-FOCEX 2024 Summer Program
# -----------------------------

CMAKE_MINIMUM_REQUIRED (VERSION 3.16)
PROJECT(focex2024 CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(default_build_type "RelWithDebInfo")
  message(STATUS "No build type specified; set to '${default_build_type}'.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING
    "Choose the type of build: Debug, Release, RelWithDebInfo and MinSizeRel."
    FORCE)
endif()

# Find and setup required packages
find_package(MPI REQUIRED)
find_package(Kokkos REQUIRED)
set(CMAKE_CUDA_RUNTIME_LIBRARY SHARED) # CUDA runtime library option

# ----------------------------------------------------------------------------
# Exercise 1 - Getting started
add_executable(exercise_1
               exercise_1.cpp)
target_link_libraries(exercise_1
                      Kokkos::kokkos)

# ----------------------------------------------------------------------------
# Exercise 2 (from Kokkos-lectures)
add_executable(exercise_2_begin
               exercise_2_begin.cpp)
target_link_libraries(exercise_2_begin
                      Kokkos::kokkos)

# ----------------------------------------------------------------------------
# Exercise 3 (from Kokkos-lectures)
add_executable(exercise_3_begin
               exercise_3_begin.cpp)
target_link_libraries(exercise_3_begin
                      Kokkos::kokkos)

# ----------------------------------------------------------------------------
# Exercise 4 (from Kokkos-lectures)
add_executable(exercise_4_begin
               exercise_4_begin.cpp)
target_link_libraries(exercise_4_begin
                      Kokkos::kokkos)

# ----------------------------------------------------------------------------
# Exercise - parallel_scan
add_executable(exercise_parallel_scan
               exercise_parallel_scan.cpp)
target_link_libraries(exercise_parallel_scan
                      Kokkos::kokkos)
                      
# ----------------------------------------------------------------------------
# Exercise - parallel_scan_full
add_executable(exercise_parallel_scan_full
               exercise_parallel_scan_full.cpp)
target_link_libraries(exercise_parallel_scan_full
                      Kokkos::kokkos)

# ----------------------------------------------------------------------------
# Exercise N
add_executable(exercise_mpi_kokkos_timers_profiling
               exercise_mpi_kokkos_timers_profiling.cpp)
target_link_libraries(exercise_mpi_kokkos_timers_profiling
                      Kokkos::kokkos)

# ----------------------------------------------------------------------------
CUDA aware MPI+Kokkos demo
add_executable(cuda_aware_mpi_kokkos_demo cuda_aware_mpi_kokkos_demo.cpp)
target_link_libraries(cuda_aware_mpi_kokkos_demo Kokkos::kokkos)

