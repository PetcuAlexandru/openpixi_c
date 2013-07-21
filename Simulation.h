#ifndef SIMULATION_H
#define SIMULATION_H

#include "Particle.h"
typedef struct simulation
{
	/**Timestep*/
	double tstep;
	/**Width of simulated area*/
	double width;
	/**Height of simulated area*/
	double  height;
	double speedOfLight;

	/** Number of iterations in the non-interactive simulation. */
	int iterations;

	/**Contains all Particle2D objects*/
	Particle *particles;

}Simulation;

void step();

void run();


#endif