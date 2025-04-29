import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


def load_advection_data(filename):
    """Loads diffusion data from the given file.

    Args:
        filename (str): The path to the data file.

    Returns:
        numpy.ndarray: A 2D array where rows are time steps and columns are spatial points.
    """
    dir = f"./data/{filename}.dat"
    data = np.loadtxt(dir)
    
    # Check if the file is properly formatted
    return np.array(data)

def plot_advection_data(file_list):
    """Plots the advection data.

    Args:
        data (numpy.ndarray): A 2D array where rows are time steps and columns are spatial points.
    
    # Extract the two rows
    first_time_step = data[0, :]
    last_time_step = data[-1, :]

    # Generate the x-axis positions normalized to [0, 1]
    x_positions = np.linspace(0, 1, first_time_step.shape[0])

    # Plot the data
    plt.figure(figsize=(10, 6))

    # Plot for the first time step
    plt.plot(x_positions, first_time_step, label="First Time Step")

    # Plot for the last time step
    plt.plot(x_positions, last_time_step, label="Last Time Step")

    # Add labels and legend
    plt.xlabel("Position (x)")
    plt.ylabel("Temperature")
    plt.title("Temperature Profile at First and Last Time Steps")
    plt.legend()

    # Show the plot
    plt.grid()
    plt.savefig(f"./figures/{filename}.png")
    """

    # Create a figure with 6 subplots arranged in 2 rows and 3 columns
    fig, axs = plt.subplots(2, 3, figsize=(15, 10))
    axs = axs.ravel()  # Flatten axs to make it easier to iterate

    # List of scheme names and their corresponding filenames
    schemes = [
        "upwind_smooth", 
        "centered_smooth", 
        "lax_wendroff_smooth",
        "upwind_discontinuous", 
        "centered_discontinuous", 
        "lax_wendroff_discontinuous"
    ]

    # Plot in each subplot
    for i, filename in enumerate(file_list):
        # Load the data for this scheme
        data = load_advection_data(filename)
    
        # Generate the x-axis positions normalized to [0, 1]
        x_positions = np.linspace(0, 1, data[0, :].shape[0])
    
        # Plot for the first time step
        axs[i].plot(x_positions, data[0, :], label="Initial")
        # Plot for the last time step
        axs[i].plot(x_positions, data[-1, :], label="Final")

        # Add labels and grid for each subplot
        axs[i].set_xlabel("Position (x)")
        axs[i].set_ylabel("Value")
        axs[i].set_title(f"{schemes[i]}")
        axs[i].legend()
        axs[i].grid(True)

    # Adjust layout to prevent overlapping
    plt.tight_layout()

    # Save the figure
    plt.savefig(f"./figures/advection_comparison.png")
    plt.close()

    
    
def animate_advection_data(data):
    """
    Animates the advection data over time.

    Args:
        data (numpy.ndarray): A 2D array where rows are time steps and columns are spatial points.
    """
    # Generate the x-axis positions normalized to [0, 1]
    x_positions = np.linspace(0, 1, data.shape[1])

    # Initialize the figure and axis
    fig, ax = plt.subplots(figsize=(10, 6))
    line, = ax.plot([], [], lw=2)  # Placeholder for the animated line
    ax.set_xlim(0, 1)  # x-axis range is the domain (0 to 1)
    ax.set_ylim(np.min(data), np.max(data))  # y-axis range is the temperature range
    ax.set_xlabel("Position (x)")
    ax.set_ylabel("Temperature")
    ax.set_title("Temperature Evolution Over Time")

    # Function to initialize the plot
    def init():
        line.set_data([], [])
        return line,

    # Function to update the plot for each frame
    def update(frame):
        line.set_data(x_positions, data[frame, :])  # Update the line data
        ax.set_title(f"Temperature Evolution at Time Step {frame}, Time = {(frame *  .0078125):.2f} s")
        return line,

    # Create the animation
    n_frames = data.shape[0]  # Number of rows = number of time steps
    ani = FuncAnimation(fig, update, frames=n_frames, init_func=init, blit=True, interval=100)

    # Show the animation
    plt.show()
    ani.save(f"./figures/{filename}_evolution.mp4", writer="ffmpeg")


if __name__ == "__main__":
    filename_list = ['advec_64_smooth_upwind', 'advec_64_smooth_centered','advec_64_smooth_lax-wendroff', 'advec_64_discontinuous_upwind', 'advec_64_discontinuous_centered', 'advec_64_discontinuous_lax-wendroff']

    plot_advection_data(filename_list)
