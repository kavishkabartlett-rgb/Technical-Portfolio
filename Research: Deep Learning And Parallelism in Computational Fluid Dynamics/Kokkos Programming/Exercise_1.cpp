#include <Kokkos_Core.hpp>

int main( int argc, char* argv[] )
{

  Kokkos::initialize( argc, argv );
  {

    const std::int64_t N = 1000;
    Kokkos::parallel_for("for", N,
                         KOKKOS_LAMBDA (std::int64_t i)
    {
      const double val_i = static_cast<double>(i); 
      printf(" Hello from iteration i %" PRId64 ", value_i %f\n", i, val_i);
    });

  }
  Kokkos::finalize();

  return 0;
}
