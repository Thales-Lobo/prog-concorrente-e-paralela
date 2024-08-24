#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Macros for branch prediction hints
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define REG register

/**
 * @brief Solves the 1D wave equation using the optimized leapfrog method.
 * 
 * This function simulates the propagation of a wave in a one-dimensional string. 
 * It uses the leapfrog method, which is a second-order accurate numerical method.
 *
 * @param num_points The number of spatial grid points.
 * @param x_final The final value of the spatial coordinate.
 * @param t_final The final time.
 * @param x A pointer to a float pointer that will store the spatial grid points.
 * @param u A pointer to a float pointer that will store the displacement of the string at each grid point and time step.
 */
void optimized_leapfrog_1D_wave_equation(unsigned short int num_points, float x_final, float t_final, float **x, float **u) {
    float x_start = 0, t_start = 0;
    float dx = (x_final - x_start) / (num_points - 1);
    float dt = dx / 2;  // Set dt = dx/2 for stability
    unsigned short int num_time_steps = (unsigned short int)((t_final - t_start) / dt) + 1;

    // Allocate memory for spatial grid, displacement, velocity, and acceleration
    *x = (float *)malloc(num_points * sizeof(float));
    *u = (float *)malloc(num_points * sizeof(float));
    float *v = (float *)calloc(num_points, sizeof(float));
    float *a = (float *)calloc(num_points, sizeof(float));

    if (UNLIKELY(*x == NULL || *u == NULL || v == NULL || a == NULL)) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize arrays
    for (REG unsigned short int i = 0; i < num_points; i++) {
        (*x)[i] = x_start + i * dx;
        (*u)[i] = expf(-powf((*x)[i] - 5, 2)); // Initial condition: Gaussian pulse
    }

    // Time-stepping loop
    for (unsigned short int t = 0; t < num_time_steps; t++) {
        float dt_leapfrog = (t == 0) ? dt * 0.5f : dt; // First time step uses dt/2

        // Update acceleration
        for (REG unsigned short int i = 1; i < num_points - 1; i++) {
            a[i] = ((*u)[i-1] + (*u)[i+1] - 2*(*u)[i]) / (dx * dx); // Central difference approximation
        }

        // Update velocity
        for (REG unsigned short int i = 1; i < num_points - 1; i++) {
            v[i] += a[i] * dt_leapfrog;
        }

        // Update displacement
        for (REG unsigned short int i = 1; i < num_points - 1; i++) {
            (*u)[i] += v[i] * dt;
        }
    }

    // Free allocated memory for velocity and acceleration
    free(v);
    free(a);
}

/**
 * @brief Measures the execution time of the optimized_leapfrog_1D_wave_equation function.
 * 
 * This function runs the optimized_leapfrog_1D_wave_equation function multiple times and calculates 
 * the mean and standard deviation of the execution time.
 *
 * @param num_runs The number of times to run the function.
 * @param num_points The number of spatial grid points to use in the simulation.
 * @param x_final The final value of the spatial coordinate.
 * @param t_final The final time of the simulation.
 * @param mean_time A pointer to a float that will store the mean execution time.
 * @param std_time A pointer to a float that will store the standard deviation of the execution time.
 */
void measure_execution_time(unsigned short int num_runs, unsigned short int num_points, float x_final, float t_final, float *mean_time, float *std_time) {
    float *times = (float *)malloc(num_runs * sizeof(float));

    // Run the function multiple times and store the execution times
    for (REG unsigned short int i = 0; i < num_runs; i++) {
        clock_t start_time = clock();
        
        float *x, *u;
        optimized_leapfrog_1D_wave_equation(num_points, x_final, t_final, &x, &u);
        
        clock_t end_time = clock();
        times[i] = (float)(end_time - start_time) / CLOCKS_PER_SEC;

        free(x);
        free(u);
    }

    // Calculate mean time
    float sum_time = 0.0f;
    for (REG unsigned short int i = 0; i < num_runs; i++) {
        sum_time += times[i];
    }
    *mean_time = sum_time / num_runs;

    // Calculate standard deviation
    float sum_square_diff = 0.0f;
    for (REG unsigned short int i = 0; i < num_runs; i++) {
        sum_square_diff += powf(times[i] - *mean_time, 2);
    }
    *std_time = sqrtf(sum_square_diff / num_runs);

    free(times);
}

/**
 * @brief Main function of the program.
 * 
 * This function sets up the parameters for the simulation, calls the function to measure 
 * the execution time, and prints the results.
 * 
 * @return 0 if the program runs successfully.
 */
int main() {
    unsigned short int num_points = 500;      // Number of grid points
    float x_final = 10.0f;     // Final spatial coordinate
    float t_final = 10.0f;     // Final time
    unsigned short int num_runs = 10000;        // Number of runs for time measurement

    float mean_time, std_time;

    // Measure execution time
    measure_execution_time(num_runs, num_points, x_final, t_final, &mean_time, &std_time);

    // Print mean and standard deviation of execution time
    printf("Mean execution time over %d runs: %.6f seconds\n", num_runs, mean_time);
    printf("Standard deviation of execution time: %.6f seconds\n", std_time);

    // Compute the final solution
    float *x, *u;
    optimized_leapfrog_1D_wave_equation(num_points, x_final, t_final, &x, &u);

    // Print a section of the solution to check for convergence
    printf("Solution near the center:\n");
    for (REG unsigned short int i = (num_points >> 1) - 10; i < (num_points >> 1) + 10; i++) {
        printf("x[%d] = %.3f, u[%d] = %.6f\n", i, x[i], i, u[i]);
    }

    // Free allocated memory
    free(x);
    free(u);

    return 0;
}
