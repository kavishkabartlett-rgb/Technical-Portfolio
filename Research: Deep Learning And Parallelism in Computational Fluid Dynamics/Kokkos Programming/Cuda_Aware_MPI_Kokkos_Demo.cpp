//! \file test_cuda_aware_mpi_kokkos.cpp
//! \brief Tests CUDA-aware MPI communications with Kokkos views
//!
//! After initializing MPI and Kokkos, create Kokkos views, fill them with
//! values, and communicate them to neighboring ranks via CUDA-aware MPI,
//! followed by some use of Kokkos parallel_for and parallel_reduce
//! operations for additional testing and verification of the communications.
//! - If CUDA-aware MPI is not supported, the MPI_Isend and MPI_Irecv calls
//!   using device buffers of the Kokkos views should fail.

//----------------------------------------------------------------------------

#include <mpi.h>
#include <Kokkos_Core.hpp>
#include <iostream>

int main( int argc, char* argv[] )
{
  // Initialize MPI
  MPI_Init(&argc,&argv);
  // Initialize Kokkos
  Kokkos::initialize( argc, argv );
  {
    // Set mpi_communicator
    MPI_Comm mpi_communicator = MPI_COMM_WORLD;
    // Get MPI rank and size
    int mpi_size;
    int mpi_rank;
    MPI_Comm_size(mpi_communicator, &mpi_size);
    MPI_Comm_rank(mpi_communicator, &mpi_rank);

    // Set up MPI ranks to send to and receive from
    // - send to MPI rank with immediately higher rank (wrap around mpi_size)
    int mpi_rank_send = mpi_rank + 1 < mpi_size ?
                        mpi_rank + 1 :
                        mpi_rank + 1 - mpi_size;
    // - receive from MPI rank with immediately lower rank (wrap around zero)
    int mpi_rank_recv = mpi_rank - 1 >= 0 ?
                        mpi_rank - 1:
                        mpi_rank - 1 + mpi_size;

    using view_type_d = Kokkos::View<double*>;

    bool cuda_aware_mpi = true;
    //int view_size = 1000; // works (no segfault)
    int view_size = 100000; // fails (segfaults)
    int num_iterations = 2;

    // Use Kokkos profilers to time desired code regions
    Kokkos::Profiling::pushRegion("for-loop__num_iterations");
    // Loop through specified number of iterations
    for (int ndx = 0; ndx < num_iterations; ndx++)
    {
      // Initialize Kokkos views of size view_size
      Kokkos::Profiling::pushRegion("allocate_views");
      view_type_d view_1("view_1", view_size);
      view_type_d view_2("view_2", view_size);
      view_type_d view_3("view_3", view_size);
      view_type_d::HostMirror host_view_1 = Kokkos::create_mirror_view(view_1);
      view_type_d::HostMirror host_view_2 = Kokkos::create_mirror_view(view_2);
      view_type_d::HostMirror host_view_3 = Kokkos::create_mirror_view(view_3);
      Kokkos::fence();
      Kokkos::Profiling::popRegion();

      // Initialize vectors of double (to replace Kokkos views in MPI calls
      // when non-CUDA-aware behavior is to be tested)
      std::vector<double> vector_1(view_size);
      std::vector<double> vector_2(view_size);

      // Fill in Kokkos views
      Kokkos::Profiling::pushRegion("parallel_for_fill_loop");
      Kokkos::parallel_for("fill_loop", view_size, KOKKOS_LAMBDA (const int idx)
      {
        view_1(idx) = static_cast<double>(mpi_rank);
        view_2(idx) = -1;
      });
      Kokkos::fence();
      Kokkos::Profiling::popRegion();

      // Set up MPI requests
      MPI_Request * send_request_array;
      MPI_Request * recv_request_array;
      MPI_Status * status_array;
      int request_count = 1;
      if (request_count > 0)
      {
        send_request_array = new (std::nothrow) MPI_Request[request_count];
        recv_request_array = new (std::nothrow) MPI_Request[request_count];
        status_array       = new (std::nothrow) MPI_Status[request_count];
        if ( send_request_array == nullptr or
             recv_request_array == nullptr or
             status_array      == nullptr )
        {
          std::cerr <<  " - Allocating memory\n";
          break;
        }
      }
      std::cout << "for-loop " << mpi_rank << std::endl;
      for (int rdx = 0; rdx < request_count; rdx++)
      {
        std::cout << "Receiving at " << mpi_rank << " from " << mpi_rank_recv
          << std::endl;
        int mpi_return_code;
        if ( cuda_aware_mpi )
        {
          mpi_return_code =
            MPI_Irecv(
              view_2.data(),                // receive buffer (Kokkos view)
              view_size,                    // count
              MPI_DOUBLE,                   // datatype
              mpi_rank_recv,                // source
              50 * rdx,                     // tag
              mpi_communicator,             // MPI comm
              &(recv_request_array[rdx]));  // MPI request
        }
        else
        {
          mpi_return_code =
            MPI_Irecv(
              host_view_2.data(),          // receive buffer (Kokkos host view)
              view_size,                   // count
              MPI_DOUBLE,                  // datatype
              mpi_rank_recv,               // source
              50 * rdx,                    // tag
              mpi_communicator,            // MPI comm
              &(recv_request_array[rdx])); // MPI request
        }
        if (mpi_return_code != MPI_SUCCESS)
        {
          std::cerr
            << "Irecv"
            << std::endl;
          break; // Exit flowControl with error
        }

        std::cout << "Sending from " << mpi_rank << " to " << mpi_rank_send
          << std::endl;
        if ( cuda_aware_mpi )
        {
          mpi_return_code =
            MPI_Isend(
              view_1.data(),               // send buffer (Kokkos view)
              view_size,                   // count
              MPI_DOUBLE,                  // datatype
              mpi_rank_send,               // source
              50 * rdx,                    // tag
              mpi_communicator,            // MPI comm
              &(send_request_array[rdx])); // MPI request
        }
        else
        {
          // Copy view_1 from device to host (before MPI communications)
          Kokkos::deep_copy( host_view_1, view_1 );
          mpi_return_code =
            MPI_Isend(
              host_view_1.data(),          // send buffer (Kokkos host view)
              view_size,                   // count
              MPI_DOUBLE,                  // datatype
              mpi_rank_send,               // source
              50 * rdx,                    // tag
              mpi_communicator,            // MPI comm
              &(send_request_array[rdx])); // MPI request
        }
        if (mpi_return_code != MPI_SUCCESS)
        {
          std::cerr
            << "Isend"
            << std::endl;
          break; // Exit flowControl with error
        }
        // MPI wait for all requests to be completed
        MPI_Waitall(request_count,recv_request_array,status_array);

        if ( cuda_aware_mpi )
        {
          Kokkos::fence();
        }
        else
        {
          // Copy view_ from host back to device (after MPI communications)
          Kokkos::deep_copy( view_2, host_view_2 );
        }
        // Calculate averages from views
        double avg_view_1 = 0.0;
        double avg_view_2 = 0.0;
        // KOKKOS_LAMBDA macro includes capture-by-value specifier [=].
        Kokkos::parallel_reduce("reduce_view_1", view_size,
                                KOKKOS_LAMBDA (const int idx, double& update)
        {
          update += view_1(idx);
        }, avg_view_1);
        avg_view_1 /= static_cast<double>(view_size);
        Kokkos::parallel_reduce("reduce_view_2", view_size,
                                KOKKOS_LAMBDA (const int idx, double& update)
        {
          update += view_2(idx);
        }, avg_view_2);
        avg_view_2 /= static_cast<double>(view_size);
        // Report outcomes to check for correctness
        std::cout << "mpi_rank" << mpi_rank
                  << " avg_view_1 " << avg_view_1 << " vs " << mpi_rank
                  << " avg_view_2 " << avg_view_2 << " vs " << mpi_rank_recv
                  << std::endl;
        Kokkos::fence();
      }
      std::cout << "end for-loop " << mpi_rank << std::endl;
      std::cout << "mpi_rank " << mpi_rank << " before delete "
                << std::endl;

      MPI_Barrier(mpi_communicator);
      delete [] send_request_array;
      delete [] recv_request_array;
      delete [] status_array;

      //std::cout << "mpi_rank " << mpi_rank << " before parallel_for"
      //          << std::endl;
      Kokkos::Profiling::pushRegion("parallel_for__view_addition");
      Kokkos::parallel_for("view_addition", view_size,
                           KOKKOS_LAMBDA (const int idx)
      {
        view_3(idx) = view_1(idx) + view_2(idx);
      });
      Kokkos::fence();
      Kokkos::Profiling::popRegion();
    }
    Kokkos::Profiling::popRegion();
  }
  // Finalize Kokkoks and MPI
  Kokkos::finalize();
  MPI_Finalize();
}
