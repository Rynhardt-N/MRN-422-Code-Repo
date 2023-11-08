import numpy as np
import scipy as sp
#parameters
tf = 20      #time period of simulation
limit = 35   #limit of angle of beam in degrees
L = 0.3    #beam length
m = 0.023    #ball mass
R = 0.02    #ball radius
g = -9.8      #gravitational acceleration
d = 0.03     #lever arm offset
J = (2/3)*m*R**2    #Moment of inertia of a hollow sphere
#J = (2/5)*m*R**2   #Moment of inertia of a solid sphere

def ballbeammodel(Kp,Kd,Ki,dt,SP):
    dt = dt
    t = 0
    x = 0.05
    SP = SP
    v = 0
    u = 0
    Ierror = 0
    uprev = 0
    
    Ulis = []
    Ulis.append(u)
    Xlis = []
    Xlis.append(x)
    Tlis = []
    Tlis.append(t)
    Vlis = []
    Vlis.append(v)
    
    while (t<tf):
        u = max((-limit*(np.pi/180)),min((limit*(np.pi/180)),Kp*(x-SP)+Kd*v+Ki*((x-SP)*(dt))))*(d/L)
        

        udot = (u-uprev)/dt
        t += dt
        v += ((m*g*np.sin(u)+(m*x*udot**2))/(((J/R**2)+m)))*(dt)
        x += v*(dt)
        x = max(0,min(L,x))
        uprev = u
        
        
        Ulis.append(u)
        Tlis.append(t)
        Vlis.append(v)
        Xlis.append(x)
        
        
    return Tlis,Xlis,Ulis,Vlis
