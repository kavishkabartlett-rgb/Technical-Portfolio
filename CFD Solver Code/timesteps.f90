      subroutine timesteps
      use data_module
      implicit none

!
!.... local variables
      integer            :: i,j
      real               :: rho,u_vel,v_vel,p
      real               :: velocity_magnitude,a
      real, dimension(4) :: u,w

!
!.... compute cell timesteps
      do j = 1,jmax-1
      do i = 1,imax-1

         u(:) = uc(:,i,j)

         call primitive_variables(gamma,u,w)

         rho   = w(1)
         u_vel = w(2)
         v_vel = w(3)
         p     = w(4)
!         write(*,*)i,j,rho,u_vel,v_vel,p

         a     = sqrt( gamma * p / rho )

         velocity_magnitude = sqrt( u_vel**2 + v_vel**2 )

!         write(*,*)i,j,rho,u_vel,v_vel,p,a
         
         dt_cell(i,j) = cfl * l_cell_min(i,j) / ( velocity_magnitude + a )
         
      enddo
      enddo
      
      end subroutine timesteps
