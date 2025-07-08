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

  // Assign default settings
  int view_size = 100000; // number of elements in the view
  int num_iterations = 1; // number of outer-loop iterations

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

  using view_type_d   = Kokkos::View<double*>;
  using view_type_i64 = Kokkos::View<std::int64_t*>;

  // Loop through specified number of iterations
  for (int ndx = 0; ndx < num_iterations; ndx++)
  {

    // Initialize Kokkos views of size view_size
    view_type_i64 view_1("view_1", view_size);
  
    view_type_i64 x("x",view_size);
    view_type_i64 post("postfix_sum",view_size);
    view_type_i64 pre("prefix_sum",view_size);
    std::int64_t result;

    Kokkos::parallel_for("init_x", view_size,
      KOKKOS_LAMBDA(const std::int64_t idx)
    {
      x(idx) = idx+1;
      //x(idx) = 2*idx+1;
    });
  
    Kokkos::parallel_scan("scan", view_size,
      KOKKOS_LAMBDA(std::int64_t i,
                    std::int64_t& partial_sum,
                    bool is_final)
    {
      // Load old value in case we update it before accumulating
      const float val_i = x(i); 
      if (is_final)
      {
        pre(i) = partial_sum;
        x(i) = partial_sum; // uncomment for exclusive; comment for inclusive
      }
      partial_sum += val_i;
      if (is_final)
      {
        post(i) = partial_sum;
        //x(i) = partial_sum; // uncomment for inclusive; comment for exclusive
      }
      printf(" i %" PRId64 ", partial_sum: %" PRId64
             ", x: %" PRId64 ", is_final %d\n",
             i, partial_sum, x(i), is_final);
    }, result);
  
    auto x_host = Kokkos::create_mirror_view(x);
    auto pre_host = Kokkos::create_mirror_view(pre);
    auto post_host = Kokkos::create_mirror_view(post);
    Kokkos::deep_copy(x_host, x);
    Kokkos::deep_copy(pre_host, pre);
    Kokkos::deep_copy(post_host, post);
    std::cout << "idx x pre post\n";
    std::cout << "--------------\n";
    for (std::int64_t idx = 0; idx < x_host.extent(0); idx++)
    {
      std::cout << idx
                << " " << x_host(idx)
                << " " << pre_host(idx)
                << " " << post_host(idx)
                << "\n";
    }

  }
  
  } // kokkos
  Kokkos::finalize();
  
}

