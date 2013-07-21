#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct settings
{
	double simulationWidth;
	double simulationHeight;
	double speedOfLight;
	double timeStep;

	int gridCellsX;
	int gridCellsY;

	int numOfParticles;
	double particleRadius;
	double particleMaxSpeed;
	
	int iterations;
}Settings;


void defaultSettings(Settings *s){
	s->simulationWidth = 100;
	s->simulationHeight = 100;
	s->speedOfLight = 1;
	s->timeStep = 1;

	s->gridCellsX = 10;
	s->gridCellsY = 10;

	s->numOfParticles = 128;
	s->particleRadius = 1;
	s->particleMaxSpeed = s->speedOfLight/3;

	s->iterations = 1;
}


#endif