import matplotlib.pyplot as plt
import numpy as np

if __name__=="__main__":
    # Read in data file
    data = np.loadtxt('sol.dat')

    # Create figure and plot trajectories
    fig,ax = plt.subplots()
    
    ax.plot(data[:,0],data[:,1],'-k.',data[:,2],data[:,3],'-b.',linewidth=2)
    ax.set_aspect('equal')
    ax.legend(['Particle 1','Particle 2'])
    ax.set_xlabel('x',fontsize='x-large')
    ax.set_ylabel('y',fontsize='x-large')
    ax.set_title('Trajectory of particles',fontsize='xx-large')

    # Show figure
    plt.show()
