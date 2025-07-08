      program solver_2d
      use data_module
      implicit none

!
!.... local variables
      integer            :: iteration
      integer            :: i,j
      real               :: rho,u_vel,v_vel,p,a,Mach_cell
      real               :: M1,M2
      real, dimension(4) :: u,w

!
!.... start up procedures
      call start_up

!
!.... grid handling
      if ( generate_grid ) call grid_generator
      call read_grid

!
!.... initialization
      call initialization

!
!.... march solution to convergence
      iteration = 0

      do

         iteration = iteration + 1

         call ghost_cells
         call timesteps
         call fluxes
         call wall_fluxes
         call update

         write(*,*)iteration,log10_l2_residual

         if ( igrid_topology == 2 ) then
            call loading
         endif

         if ( log10_l2_residual < log10_l2_residual_converge ) then

            call output
            stop

         endif

      enddo

      write(*,*)'finished solver_2d'

      end program solver_2d
