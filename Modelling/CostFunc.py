import ballbeammodel as bb
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt


def costfunc(params,dt,SP):
    Kp = params[0]
    Kd = params[1]
    Ki = params[2]
    T,X,U,V = bb.ballbeammodel(Kp,Kd,Ki,dt,SP)
    err = []
    for i in range(0,len(U)):
        error = abs(0.15-X[i])
        err.append(error)
    q = sp.integrate.trapz(err,T)
    return q