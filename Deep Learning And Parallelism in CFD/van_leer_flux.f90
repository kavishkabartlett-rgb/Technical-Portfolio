      subroutine van_leer_flux(ql,qr,gamma,f)
      implicit none

!
!.... passed variables
      real                 :: gamma
      real, dimension(4)   :: ql,qr,f        

!
!.... local variables
      integer            :: k
      real               :: rhol,rrhol,rhor,rrhor
      real               :: ul,ur,vl,vr,Etl,Etr
      real               :: pl,pr!,hl,hr,dp,du,dv,drho
      real               :: al,ar,aml,amr
      real, dimension(4) :: fp,fm

      rhol = ql(1)
      rrhol = 1./rhol
      rhor = qr(1)
      rrhor = 1./rhor
      ul = ql(2)*rrhol
      ur = qr(2)*rrhor
      vl = ql(3)*rrhol
      vr = qr(3)*rrhor
      Etl = ql(4)
      Etr = qr(4)
      pl = (gamma-1)*(Etl - 0.5*rhol*(ul**2+vl**2))
      pr = (gamma-1)*(Etr - 0.5*rhor*(ur**2+vr**2))

      al = sqrt(gamma*pl/rhol)
      ar = sqrt(gamma*pr/rhor)
      aml = ul/al
      amr = ur/ar

      if (aml .lt. -1.) then
         fp(1) = 0.
         fp(2) = 0.
         fp(3) = 0.
         fp(4) = 0.
      else if (abs(aml) .le. 1.) then
         fp(1) = rhol*al*(0.5*(aml+1))**2
         fp(2) = fp(1)*((gamma-1)*ul+2.*al)/gamma
         fp(3) = fp(1)*vl
         fp(4) = fp(1)*(((gamma-1)*ul+2.*al)**2/(2.*(gamma**2-1)) + 0.5*vl**2)
      else if (aml .gt. 1.) then
         fp(1) = rhol*ul
         fp(2) = rhol*(ul**2+al**2/gamma)
         fp(3) = rhol*ul*vl
         fp(4) = rhol*ul*(0.5*(ul**2+vl**2)+al**2/(gamma-1))
      end if

      if (amr .lt. -1.) then
         fm(1) = rhor*ur
         fm(2) = rhor*(ur**2+ar**2/gamma)
         fm(3) = rhor*ur*vr
         fm(4) = rhor*ur*(0.5*(ur**2+vr**2)+ar**2/(gamma-1))
      else if (abs(amr) .le. 1.) then
         fm(1) = -rhor*ar*(0.5*(-amr+1))**2
         fm(2) = -fm(1)*((gamma-1)*(-ur)+2.*ar)/gamma
         fm(3) = fm(1)*vr
         fm(4) = fm(1)*(((gamma-1)*(-ur)+2.*ar)**2/(2.*(gamma**2-1)) + 0.5*vr**2)
      else if (amr .gt. 1) then
         fm(1) = 0.
         fm(2) = 0.
         fm(3) = 0.
         fm(4) = 0.
      end if

      do k = 1,4
         f(k) = fp(k)+fm(k)
      enddo

      end subroutine van_leer_flux
