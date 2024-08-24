import numpy as np
import time

def leapfrog_1D_wave_equation(num_points, x_final, t_final):
    """
    Solves the 1D wave equation using the leapfrog integration method.
    
    Parameters:
        num_points (int): Number of spatial grid points.
        x_final (float): Final position in space.
        t_final (float): Final time.
    
    Returns:
        x (ndarray): Array of spatial points.
        u (ndarray): Array of displacement values at the final time.
    """
    x_start = t_start = 0
    dx = (x_final - x_start) / (num_points - 1)  # Spatial step size
    dt = dx / 2  # Temporal step size
    num_time_steps = int((t_final - t_start) / dt) + 1  # Number of time steps
    
    # Initialize arrays
    x = np.linspace(x_start, x_final, num_points)
    u = np.exp(-(x - 5)**2)  # Initial displacement
    v = np.zeros(num_points)  # Initial velocity
    a = np.zeros(num_points)  # Initial acceleration
    
    # Time-stepping loop
    for t in range(num_time_steps):
        dt_leapfrog = dt / 2 if t == 0 else dt
        
        # Update acceleration
        for i in range(1, num_points - 1):
            a[i] = (u[i-1] + u[i+1] - 2*u[i]) / dx**2
        
        # Update velocity
        for i in range(1, num_points - 1):
            v[i] += a[i] * dt_leapfrog
        
        # Update displacement
        for i in range(1, num_points - 1):
            u[i] += v[i] * dt
    
    return x, u

def measure_execution_time(num_runs, num_points, x_final, t_final):
    """
    Measures the average execution time of the leapfrog_1D_wave_equation function.
    
    Parameters:
        num_runs (int): Number of times to run the simulation for averaging.
        num_points (int): Number of spatial grid points.
        x_final (float): Final position in space.
        t_final (float): Final time.
    
    Returns:
        mean_time (float): Mean execution time over the specified number of runs.
        std_time (float): Standard deviation of the execution time.
    """
    times = []
    for _ in range(num_runs):
        start_time = time.time()
        leapfrog_1D_wave_equation(num_points, x_final, t_final)
        end_time = time.time()
        times.append(end_time - start_time)
    
    mean_time = np.mean(times)
    std_time = np.std(times)
    
    return mean_time, std_time

if __name__ == '__main__':
    num_points = 500
    x_final = 10
    t_final = 10
    num_runs = 100

    # Measure execution time
    mean_time, std_time = measure_execution_time(num_runs, num_points, x_final, t_final)
    
    # Print mean and standard deviation of execution time
    print(f'Mean execution time over {num_runs} runs: {mean_time:.6f} seconds')
    print(f'Standard deviation of execution time: {std_time:.6f} seconds')

    # Compute the final solution
    x, u = leapfrog_1D_wave_equation(num_points, x_final, t_final)
    
    # Print the solution to check for convergence
    for i in range((num_points // 2) - 10, (num_points // 2) + 10):
        print(f'x[{i}] = {x[i]:.3f}, u[{i}] = {u[i]:.6f}')
