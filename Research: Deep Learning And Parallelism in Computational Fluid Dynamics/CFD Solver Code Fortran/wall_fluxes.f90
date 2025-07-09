      subroutine wall_fluxes
      use data_module
      implicit none

!
!.... local variables
      integer            :: i,j
      real               :: ct,st
      real               :: p
      real, dimension(4) :: ul,w,f

!
!.... wall boundary fluxes
      j = 1
      do i = 1,imax-1

         ul(:) = uc(:,i,j)
         ct    = c_j(i,j)
         st    = s_j(i,j)

         call rotate(ul,ct,st)

         call primitive_variables(gamma,ul,w)

         p = w(4)

         f(1) = 0.
         f(2) = p
         f(3) = 0.
         f(4) = 0.

         call rotateinv(f,ct,st)

         f_j(:,i,j) = f(:)

      enddo

      end subroutine wall_fluxes
