//@HEADER
// ************************************************************************
//                        USC FOCEX 2024
// ************************************************************************
//@HEADER

#include <iostream>
#include <Kokkos_Core.hpp>
#include <mpi.h>

int main( int argc, char* argv[] )
{

  // Initialize MPI
  MPI_Init(&argc,&argv);

  // Assign default settings
  int view_size = 100000; // number of elements in the view
  int num_iterations = 2; // number of outer-loop iterations

  // Read command line arguments and overwrite default settings if specified
  for ( int adx = 0; adx < argc; adx++ ) {
    if ( ( strcmp( argv[ adx ], "-s" ) == 0 ) or
         ( strcmp( argv[ adx ], "-view_size" ) == 0 ) )
    {
      view_size = atoi( argv[++adx] );
    }
    else if ( strcmp( argv[ adx ], "-i" ) == 0 or
              strcmp( argv[ adx ], "-num_iterations" ) == 0 )
    {
      num_iterations = atoi( argv[ ++adx ] );
    }
    else if ( ( strcmp( argv[ adx ], "-h" ) == 0 ) or 
              ( strcmp( argv[ adx ], "-help" ) == 0 ) )
    {
      std::cout << "Usage:"
        << "\n -view_size (-s) <int>: number of elements in view"
        << "\n -num_iterations (-i) <int>: number of iterations"
        << "\n -help (-h):            print this message\n\n";
      exit(1);
    }
  }

  // Report settings
  std::cout << "Running with:"
    << "\n - view_size = " << view_size
    << "\n - num_iterations = " << num_iterations
    << "\n"
    << std::endl;

  Kokkos::initialize( argc, argv );
  { // kokkos

  using view_type_d = Kokkos::View<double*>;

  // Timer 
  Kokkos::Timer timer; 
  double time_in_iterations = 0.0;
  double variance = 0.0;

  // Use Kokkos profilers to time desired code regions
  Kokkos::Profiling::pushRegion("for-loop__num_iterations");
  // Loop through specified number of iterations
  for (int ndx = 0; ndx < num_iterations; ndx++)
  {
    double iteration_start_time = timer.seconds();

    // Initialize Kokkos views of size view_size
    Kokkos::Profiling::pushRegion("allocate_views");
    view_type_d view_1("view_1", view_size);
    view_type_d view_2("view_2", view_size);
    view_type_d view_3("view_3", view_size);
    Kokkos::fence();
    Kokkos::Profiling::popRegion();

    // Fill in Kokkos views
    Kokkos::Profiling::pushRegion("parallel_for_fill_loop");
    Kokkos::parallel_for("fill_loop", view_size, KOKKOS_LAMBDA (const int idx)
    {
      view_1(idx) = idx;
      view_2(idx) = -1;
    });
    Kokkos::fence();
    Kokkos::Profiling::popRegion();

    Kokkos::Profiling::pushRegion("parallel_for__view_addition");
    Kokkos::parallel_for("view_addition", view_size,
                         KOKKOS_LAMBDA (const int idx)
    {
      view_3(idx) = view_1(idx) + view_2(idx);
    });
    Kokkos::fence();
    Kokkos::Profiling::popRegion();

    const double iteration_end_time = timer.seconds();
    const double iteration_time = iteration_end_time - iteration_start_time;

    // Add timer values to statistics
    variance += iteration_time * iteration_time;
    time_in_iterations += iteration_time;
    std::cout << "Iteration " << ndx << " ran in " << iteration_time << " s\n";
  }
  Kokkos::Profiling::popRegion();

  // Obtain total time of execution of the outer for loop
  const double total_time = timer.seconds();

  // Calculate mean and standard deviation of time spent per iteration
  const double mean_prime = total_time / static_cast<double>(num_iterations);
  const double mean = time_in_iterations / static_cast<double>(num_iterations);
  variance = variance / static_cast<double>(num_iterations) - mean * mean;
  const double std_dev = std::sqrt(variance);

  std::cout << "\n---\n"
            << "Execution summary"
            << "\n - Number of iterations                     = "
            << num_iterations
            << "\n - Total execution time                     = "
            << total_time << " s"
            << "\n - Total execution time spent in iterations = "
            << time_in_iterations << " s"
            << "\n - Mean time per iteration                  = "
            << mean << " s"
            << "\n - Standard deviation                       = "
            << std_dev << " s = "
            << 100.0 * std_dev / mean << " % of mean"
            << std::endl;

  } // kokkos
  Kokkos::finalize();
  MPI_Finalize();

}

