#ifndef FORCE_H
#define FORCE_H

#include "Particle.h"

/**
 * Total force in the x-direction.
 *
 * This should always equal getPositionComponentofForceX(p) +
 * getNormalVelocityComponentofForceX(p) +
 * getTangentVelocityComponentOfForceX(p).
 */
double getForceX(Particle p);

/**
 * Total force in the y-direction.
 *
 * This should always equal getPositionComponentofForceY(p) +
 * getNormalVelocityComponentofForceY(p) +
 * getTangentVelocityComponentOfForceY(p).
 */
double getForceY(Particle p);

/**
 * Position dependent component of the force in x-direction.
 */
double getPositionComponentofForceX(Particle p);

/**
 * Position dependent component of the force in y-direction.
 */
double getPositionComponentofForceY(Particle p);

/**
 * Velocity dependent component of the force in the propagating direction of
 * the particle (x-component).
 *
 * This should always equal -getLinearDragCoefficient(p) * p.vx.
 */
double getTangentVelocityComponentOfForceX(Particle p);

/**
 * Velocity dependent component of the force in the propagating direction of
 * the particle (y-component).
 *
 * This should always equal -getLinearDragCoefficient(p) * p.vy.
 */
double getTangentVelocityComponentOfForceY(Particle p);

/**
 * Velocity dependent component of the force orthogonal to the propagating
 * direction of the particle (x-component).
 *
 * This should always equal p.charge * p.vy * getBz(p).
 */
double getNormalVelocityComponentofForceX(Particle p);

/**
 * 
 * Velocity dependent component of the force orthogonal to the propagating
 * direction of the particle (y-component).
 *
 * This should always equal -p.charge * p.vx * getBz(p)
 */
double getNormalVelocityComponentofForceY(Particle p);

/**
 * Magnetic field the particle is exposed to.
 */
double getBz(Particle p);

/**
 * Drag coefficient for a drag term that is linear in the velocity of the
 * particle.
 */
double getLinearDragCoefficient(Particle p);


#endif