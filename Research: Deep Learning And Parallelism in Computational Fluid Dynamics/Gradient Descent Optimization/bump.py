#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun 25 13:48:45 2025

@author: cpantano
"""

import math
import numpy as np
import torch
import matplotlib.pyplot as plt

torch.set_default_dtype(torch.float64)

class NeuralNetwork(torch.nn.Module):
    '''
    Neural network with a fixed number of hidden layers and width.

    Parameters:
        input_dim (int): input dimension
        output_dim (int): output dimension
        width (int): width of hidden layers
        depth (int): number of hidden layers + output layer
        activation (str): activation function. One of 'relu' or 'tanh'.
    '''
    def __init__(self, input_dim=2, output_dim=1, width=5, depth=3, activation='relu'):
        super().__init__()
        self.input_dim = input_dim          # input dimension
        self.output_dim = output_dim        # output dimension
        self.width = width                  # width of hidden layers
        self.depth = depth                  # number of hidden layers + output layer
        assert depth >= 2, 'depth must be at least 2'
        assert activation in ['relu', 'tanh'], 'activation must be one of "relu" or "tanh"'
        if activation == 'relu':
            self.activation = torch.nn.ReLU()
        elif activation == 'tanh':
            self.activation = torch.nn.Tanh()
        else:
            raise ValueError('activation must be one of "relu", "tanh", or "sin"')
        
        self.modules = []
        self.modules.append(torch.nn.Linear(input_dim, width))  # input layer
        self.modules.append(self.activation)                    # activation function
        for _ in range(depth-1):
            self.modules.append(torch.nn.Linear(width, width))  # hidden layers
            self.modules.append(self.activation)                # activation function
        
        self.modules.append(torch.nn.Linear(width, output_dim)) # output layer
        self.net = torch.nn.Sequential(*self.modules)           # combine all layers

    def forward(self, x):
        return self.net(x)
    

def f(net: torch.nn.Module, x: torch.Tensor) -> tuple:
    '''
    Compute the output and gradient of a neural network.

    Inputs:
        net (torch.nn.Module): neural network
        x (torch.Tensor): input tensor

    '''
    u = net(x)
    du_dx = torch.autograd.grad(u, x, grad_outputs=torch.ones_like(u), create_graph=True, retain_graph=True)[0]

    # Calculate the Hessian by calling grad for each equation
    d2u_dx2 = torch.zeros(du_dx.size(0), du_dx.size(0))
    for i in range(du_dx.size(0)):
        d2u_dx2[i,:] = torch.autograd.grad(du_dx[i], x, retain_graph=True)[0] 
    
    return u.detach().numpy(), du_dx.detach().numpy(), d2u_dx2.detach().numpy()

def g(x: torch.Tensor) -> tuple:
    
    x1 = x[0]
    y1 = x[1]
    x2 = x[2]
    y2 = x[3]
    val = 3.0/20.0*(y1*(1+x2)+(2-x1)*y2)
    dg_dx = torch.autograd.grad(val, x, grad_outputs=torch.ones_like(val), create_graph=True, retain_graph=True)[0]
    
    # Calculate the Hessian by calling grad for each equation
    d2g_dx2 = torch.zeros(dg_dx.size(0), dg_dx.size(0))
    for i in range(dg_dx.size(0)):
        d2g_dx2[i,:] = torch.autograd.grad(dg_dx[i], x, retain_graph=True)[0] 
    
    return val.detach().numpy(), dg_dx.detach().numpy(), d2g_dx2.detach().numpy()

input_dim = 4

net = NeuralNetwork(input_dim=input_dim, output_dim=1, width=10, depth=5, activation='tanh')
net.load_state_dict(torch.load('bump_model.pth'))

Jac = np.zeros((input_dim+1,input_dim+1))
func = np.zeros(input_dim+1)

# initial guess
x = np.array([0.0, 0.1, 1.0, 0.1])
lam = -1.0

print ("Itera       x1           y1            x2           y2        lambda          Cd            A") 
# Newton loop
for i in range(11):
    xv = torch.Tensor(x).requires_grad_()
    # drag from NN
    u, du_dx, d2u_dx2 = f(net, xv)
    # constraint
    gval, dg_dx, d2g_dx2 = g(xv)
    
    # saddle-acceleration 
    lam = - np.sum(du_dx[:]*dg_dx[:])/np.sum(dg_dx[:]*dg_dx[:])
    
    print ( '%2d    %12.5e  %12.5e  %12.5e  %12.5e  %12.5e  %12.5e  %12.5e' % (i, x[0], x[1], x[2], x[3], lam, u, gval) )

    # assemble function
    func[0:input_dim] = du_dx[:] + lam * dg_dx[:]
    func[input_dim] = gval - 0.05
    # assemble jacobian
    Jac[0:input_dim, 0:input_dim] = d2u_dx2[:][:] + lam * d2g_dx2[:][:]
    Jac[0:input_dim,input_dim] = dg_dx[:]
    Jac[input_dim,0:input_dim] = dg_dx[:]

    # Calculate Newton correction
    dx = np.linalg.solve(Jac, func)
    x[:] -= dx[0:input_dim]
    lam -= dx[input_dim]
    
