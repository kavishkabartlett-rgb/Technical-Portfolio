      subroutine start_up
      use data_module
      implicit none

!
!.... read in inputs namelist file      
      open(11,file='solver_2d_inputs.namelist')
      read(11,nml=solver_2d_inputs_namelist)
      close(11)

      end subroutine start_up
    
