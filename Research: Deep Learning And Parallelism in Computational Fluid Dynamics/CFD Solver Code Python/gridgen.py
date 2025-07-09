import numpy as np

def grid_bump(domain, npoints, bump_func, betax=2., betay=1.5):
    x0, xend, y0, yend, bump_x0, bump_xn = domain
    nx1, nxb, nx2, ny = npoints

    #x coordinates

    # x_linear_1
    xi_linear_1=np.linspace(0,1,nx1) #[0,1] uniform
    x_linear_1=(np.tanh(betax*xi_linear_1)/np.tanh(betax)) #[0,1] and clustered near 1
    x_linear_1=x_linear_1*(bump_x0-x0)+x0

    #x_bump
    xi_bump=np.linspace(-1,1,nxb) #[-1,1] and uniform
    x_bump=np.tanh(betax*xi_bump)/np.tanh(betax) #[-1,1] and clustered near both -1 and 1
    x_bump=((x_bump+1.)/2.) #[0,1] and clustered near both 0 and 1
    x_bump=x_bump*(bump_xn-bump_x0)+bump_x0
    x_bump=np.copy(x_bump[1:-1]) #there will be an overlap between the first point of x_bump and last point of x_linear_1
    
    #x_linear2
    xi_linear_2=np.linspace(-1, 0, nx2) #[-1,0] and uniform
    x_linear_2=(np.tanh(betax*xi_linear_2)/np.tanh(betax)) #[-1,0] and clustered near 1
    x_linear_2=x_linear_2+1. #[0,1] and clustered near 0
    x_linear_2=x_linear_2*(xend-bump_xn)+bump_xn
    
    #put it all together
    x1d=np.hstack([x_linear_1, x_bump, x_linear_2])
    nx=len(x1d)
    
    #define the bottom and top wall's grids
    y_bottom_1=np.zeros_like(x_linear_1)+y0
    y_bottom_b=bump_func(x_bump)+y0
    y_bottom_2=np.zeros_like(x_linear_2)+y0
    y_bottom=np.concatenate([y_bottom_1, y_bottom_b, y_bottom_2])
    y_top=np.zeros_like(y_bottom)+(yend)

    # eta: a uniform grid from -1 to 1, divided by the number of grid points in y
    eta=np.linspace(-1, 1, ny)
    # apply tanh clustering to eta
    eta=np.tanh(betay*eta)/np.tanh(betay)
    # convert eta to something between 0 and 1, like before
    eta=(eta+1.)/2.
    
    #2d variables
    X2D,Y2D=np.meshgrid(x1d,eta)
    X2D=X2D.T
    Y2D=Y2D.T

    local_height=y_top-y_bottom
    for i in range(nx):
        Y2D[i,:]=local_height[i]*eta+y_bottom[i]
    
    return X2D,Y2D

def grid_rectangle(domain, npoints, betax=2., betay=1.5):
    x0, xend, y0, yend=domain
    nx, ny = npoints

    #x coordinates
    xi=np.linspace(-1,1,nx) #[-1,1] and uniform
    x=np.tanh(betax*xi)/np.tanh(betax) #[-1,1] and clustered near both -1 and 1
    x=((x+1.)/2.) #[0,1] and clustered near both 0 and 1
    x=x*(xend-x0)+x0    

    # eta: a uniform grid from -1 to 1, divided by the number of grid points in y
    eta=np.linspace(-1, 1, ny)
    # apply tanh clustering to eta
    eta=np.tanh(betay*eta)/np.tanh(betay)
    # convert eta to something between 0 and 1, like before
    eta=(eta+1.)/2.
    
    #y coordinates
    y=eta*(yend-y0)+y0
    
    #2d variables
    X2D,Y2D=np.meshgrid(x,y)
    X2D=X2D.T
    Y2D=Y2D.T

    return X2D,Y2D