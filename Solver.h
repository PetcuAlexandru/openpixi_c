#ifndef SOLVER_H
#define SOLVER_H

#include "Particle.h"
#include "Force.h"

void step(Particle p, double step);

void prepare(Particle p, double dt);

void complete(Particle p, double dt);

#endif