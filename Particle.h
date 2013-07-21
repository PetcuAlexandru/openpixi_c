#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct particle
{
	/** x-coordinate */
	double x;

	/** y-coordinate */
	double y;

	/** radius of particle */
	double radius;

	/** velocity in x-direction */
	double vx;

	/** velocity in y-direction */
	double vy;

	/** acceleration in x-direction */
	double ax;

	/** acceleration in y-direction */
	double ay;

	/** mass of the particle */
	double mass;

	/** electric charge of the particle */
	double charge;

	/**previous x position of particle*/
	double prevX;
	/**previous y position of particle*/
	double prevY;
	
	double Ex;
	double Ey;
	double Bz;
	
	/**previous position component of the force in x - direction used in Boris & BorisDamped*/
	double prevpositionComponentForceX;
	
	/**previous position component of the force in y - direction used in Boris & BorisDamped*/
	double prevpositionComponentForceY;

	/**previous tangent velocity component of the force in x - direction used in Boris*/
	double prevtangentVelocityComponentOfForceX;

	/**previous tangent velocity component of the force in y - direction used in Boris*/
	double prevtangentVelocityComponentOfForceY;
	
	/**previous normal velocity component of the force in x - direction used in LeapFrogDamped*/
	double prevnormalVelocityComponentOfForceX;

	/**previous normal velocity component of the force in y - direction used in LeapFrogDamped*/
	double prevnormalVelocityComponentOfForceY;	

	/**previous magnetic field used in Boris & BorisDamped*/
	double prevBz;
	
	/**previous linear drag coefficient used in BorisDamped*/
	double prevLinearDragCoefficient;
}Particle;

void storePositionX(Particle *p);
// void storePositionX(Particle *p){
// 	p->prevX = p->x;
// 	p->prevY = p->y;
// }

#endif