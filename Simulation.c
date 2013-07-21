#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Cell.h"
#include "Force.h"
#include "Grid.h"
#include "Particle.h"
#include "Settings.h"
#include "Simulation.h"
#include "Solver.h"

void readInputData(Simulation *sim, Grid *g, Settings *sett){
	FILE *inParticles, *inCells;
	int i, j;

	defaultSettings(sett);
	set(g, sett->gridCellsX, sett->gridCellsY, sett->simulationWidth, sett->simulationHeight);

	sim->particles = malloc(sett->numOfParticles * sizeof(struct particle));
	g->cells 	= malloc(g->numCellsXTotal * sizeof(struct cell*));
	for(i = 0; i < g->numCellsXTotal; i++){
		g->cells[i] = malloc(g->numCellsYTotal * sizeof(struct cell));
	}

	inParticles = fopen("particles.txt", "r");
	for(i = 0; i < sett->numOfParticles; i++){
		fscanf(inParticles, "%lg", &sim->particles[i].x);
		//printf("%lg\n", sim->particles[i].x);
		fscanf(inParticles, "%lg", &sim->particles[i].y);
		//printf("%lg\n", sim->particles[i].y);
		fscanf(inParticles, "%lg", &sim->particles[i].radius);
		//printf("%lg\n", sim->particles[i].radius);
		fscanf(inParticles, "%lg", &sim->particles[i].vx);
		//printf("%lg\n", sim->particles[i].vx);
		fscanf(inParticles, "%lg", &sim->particles[i].vy);
		//printf("%lg\n", sim->particles[i].vy);
		fscanf(inParticles, "%lg", &sim->particles[i].ax);
		//printf("%lg\n", sim->particles[i].ax);
		fscanf(inParticles, "%lg", &sim->particles[i].ay);
		//printf("%lg\n", sim->particles[i].ay);
		fscanf(inParticles, "%lg", &sim->particles[i].mass);
		//printf("%lg\n", sim->particles[i].mass);
		fscanf(inParticles, "%lg", &sim->particles[i].charge);
		//printf("%lg\n", sim->particles[i].charge);
		fscanf(inParticles, "%lg", &sim->particles[i].prevX);
		//printf("%lg\n", sim->particles[i].prevX);
		fscanf(inParticles, "%lg", &sim->particles[i].prevY);
		//printf("%lg\n", sim->particles[i].prevY);
		fscanf(inParticles, "%lg", &sim->particles[i].Ex);
		//printf("%lg\n", sim->particles[i].Ex);
		fscanf(inParticles, "%lg", &sim->particles[i].Ey);
		//printf("%lg\n", sim->particles[i].Ey);
		fscanf(inParticles, "%lg", &sim->particles[i].Bz);
		//printf("%lg\n", sim->particles[i].Bz);
		fscanf(inParticles, "%lg", &sim->particles[i].prevpositionComponentForceX);
		//printf("%lg\n", sim->particles[i].prevpositionComponentForceX);
		fscanf(inParticles, "%lg", &sim->particles[i].prevpositionComponentForceY);
		//printf("%lg\n", sim->particles[i].prevpositionComponentForceY);
		fscanf(inParticles, "%lg", &sim->particles[i].prevtangentVelocityComponentOfForceX);
		//printf("%lg\n", sim->particles[i].prevtangentVelocityComponentOfForceX);
		fscanf(inParticles, "%lg", &sim->particles[i].prevtangentVelocityComponentOfForceY);
		//printf("%lg\n", sim->particles[i].prevtangentVelocityComponentOfForceY);
		fscanf(inParticles, "%lg", &sim->particles[i].prevnormalVelocityComponentOfForceX);
		//printf("%lg\n", sim->particles[i].prevnormalVelocityComponentOfForceX);
		fscanf(inParticles, "%lg", &sim->particles[i].prevnormalVelocityComponentOfForceY);
		//printf("%lg\n", sim->particles[i].prevnormalVelocityComponentOfForceY);
		fscanf(inParticles, "%lg", &sim->particles[i].prevBz);
		//printf("%lg\n", sim->particles[i].prevBz);
		fscanf(inParticles, "%lg", &sim->particles[i].prevLinearDragCoefficient);
		//printf("%lg\n", sim->particles[i].prevLinearDragCoefficient);
	}
	fclose(inParticles);

	inCells = fopen("cells.txt", "r");
	for(i = 0; i < g->numCellsXTotal; i++){
		for(j = 0; j < g->numCellsYTotal; j++){
			fscanf(inCells, "%lg", &g->cells[i][j].jx);
			// printf("%lg\n", g->cells[i][j].jx);
			fscanf(inCells, "%lg", &g->cells[i][j].jy);
			// printf("%lg\n", g->cells[i][j].jy);
			fscanf(inCells, "%lg", &g->cells[i][j].rho);
			// printf("%lg\n", g->cells[i][j].rho);
			fscanf(inCells, "%lg", &g->cells[i][j].phi);
			// printf("%lg\n", g->cells[i][j].phi);
			fscanf(inCells, "%lg", &g->cells[i][j].ex);
			// printf("%lg\n", g->cells[i][j].ex);
			fscanf(inCells, "%lg", &g->cells[i][j].ey);
			// printf("%lg\n", g->cells[i][j].ey);
			fscanf(inCells, "%lg", &g->cells[i][j].bz);
			// printf("%lg\n", g->cells[i][j].bz);
			fscanf(inCells, "%lg", &g->cells[i][j].bzo);
			// printf("%lg\n", g->cells[i][j].bzo);
		}
	}
	fclose(inCells);

}

int main(){
	Simulation *sim = malloc(sizeof(struct simulation));
	Settings *sett  = malloc(sizeof(struct settings));
	Grid *g 	    = malloc(sizeof(struct grid));
	Boundary regionBoundaryMap[9];

	int i, j, k;
	double PositionComponentofForceX;
	double PositionComponentofForceY;
	double Bz;
	double TangentVelocityComponentOfForceX;
	double TangentVelocityComponentOfForceY;
	double Mass;
	double vxminus;
	double vyminus;
	double t_z;
	double s_z;
	double vxprime;
	double vyprime;
	double vxplus;
	double vyplus;
	double timeStep = sett->timeStep;
	int region;
	/**X index of local origin i.e. nearest grid point BEFORE particle push*/
	int xStart;
	/**Y index of local origin i.e. nearest grid point BEFORE particle push*/
	int yStart;
	/**X index of local origin i.e. nearest grid point AFTER particle push*/
	int xEnd;
	/**Y index of local origin i.e. nearest grid point AFTER particle push*/
	int yEnd;
	/**Normalized local x coordinate BEFORE particle push*/
	double x;
	/**Normalized local y coordinate BEFORE particle push*/
	double y;
	/**Normalized distance covered in X direction*/
	double deltaX;
	/**Normalized distance covered in X direction*/
	double deltaY;


	readInputData(sim, g, sett);
	createBoundaryMap(regionBoundaryMap, sett->simulationWidth, sett->simulationHeight);
	for(j = 0; j < sett->iterations; j++){
	for(i = 0; i < sett->numOfParticles; i++){

		/*--------------------------------------------------------------------------/
	    /---------------- Step 1: particlePush()------------------------------------/
	    /--------------------------------------------------------------------------*/

	    //a) particle.storePosition() 
	    	sim->particles[i].prevX = sim->particles[i].x;
	    	sim->particles[i].prevY = sim->particles[i].y;

	    //b)solver.timeStep(particle, force, timeStep)/----Boris solver-----/
		    PositionComponentofForceX = getPositionComponentofForceX(sim->particles[i]);
			PositionComponentofForceY = getPositionComponentofForceY(sim->particles[i]);
			Bz = getBz(sim->particles[i]);
			TangentVelocityComponentOfForceX = getTangentVelocityComponentOfForceX(sim->particles[i]);
			TangentVelocityComponentOfForceY = getTangentVelocityComponentOfForceY(sim->particles[i]);
			Mass = sim->particles[i].mass;
			
			// remember for complete()
			sim->particles[i].prevpositionComponentForceX = PositionComponentofForceX;
			sim->particles[i].prevpositionComponentForceY = PositionComponentofForceY;
			sim->particles[i].prevBz = Bz;
			sim->particles[i].prevtangentVelocityComponentOfForceX = TangentVelocityComponentOfForceX;
			sim->particles[i].prevtangentVelocityComponentOfForceY = TangentVelocityComponentOfForceY;

			vxminus = sim->particles[i].vx + PositionComponentofForceX * timeStep / (2.0 * Mass);
			
			vyminus = sim->particles[i].vy + PositionComponentofForceY * timeStep / (2.0 * Mass);
			
			t_z = sim->particles[i].charge * Bz * timeStep / (2.0 * Mass);   //t vector
			
			s_z = 2 * t_z / (1 + t_z * t_z);               //s vector
			
			vxprime = vxminus + vyminus * t_z;
			vyprime = vyminus - vxminus * t_z;
			
			vxplus = vxminus + vyprime * s_z;
			vyplus = vyminus - vxprime * s_z;
			
			sim->particles[i].vx = vxplus + PositionComponentofForceX * timeStep / (2.0 * Mass) + TangentVelocityComponentOfForceX * timeStep / Mass;
			sim->particles[i].vy = vyplus + PositionComponentofForceY * timeStep / (2.0 * Mass) + TangentVelocityComponentOfForceY * timeStep / Mass;
			
			sim->particles[i].x = sim->particles[i].x + sim->particles[i].vx * timeStep;
			sim->particles[i].y = sim->particles[i].y + sim->particles[i].vy * timeStep;

			//c) boundaries.applyOnParticleCenter(solver, force, particle, timeStep)
			region = get_region(sim->particles[i].x, sim->particles[i].x, sim->particles[i].y, 
								sim->particles[i].y, sett->simulationWidth, sett->simulationHeight);

			sim->particles[i].x = sim->particles[i].x - regionBoundaryMap[region].xoffset;
			sim->particles[i].prevX = sim->particles[i].prevX - regionBoundaryMap[region].xoffset;

			sim->particles[i].y = sim->particles[i].y - regionBoundaryMap[region].yoffset;
			sim->particles[i].prevY = sim->particles[i].prevY - regionBoundaryMap[region].yoffset;

			/*--------------------------------------------------------------------------/
	    	/---Step 4: interpolation.interpolateToGrid(particles, grid, tstep)---------/
	    	/--------------------------------------------------------------------------*/

	        resetCurrent(g);

	        x = sim->particles[i].prevX / g->cellWidth;
	        y = sim->particles[i].prevY / g->cellHeight;

	        xStart = (int) floor(x + 0.5);
	        yStart = (int) floor(y + 0.5);

	        deltaX = sim->particles[i].x / g->cellWidth;
	        deltaY = sim->particles[i].y / g->cellHeight; 

	        xEnd = (int) floor(deltaX + 0.5);
	        yEnd = (int) floor(deltaY + 0.5);

	        deltaX -= x;
	        deltaY -= y;

	        x -= xStart;
	        y -= yStart;

	        double pCharge = sim->particles[i].charge;

	        //4-boundary move?
	        if (xStart == xEnd && yStart == yEnd) {
	                fourBoundaryMove(xStart, yStart, x, y, deltaX, deltaY, pCharge, g);
	        }
	        //7-boundary move?
	        else if (xStart == xEnd || yStart == yEnd) {
	                sevenBoundaryMove(x, y, xStart, yStart, xEnd, yEnd, deltaX, deltaY, pCharge, g);
	                }
	                // 10-boundary move
	                        else {
	                                tenBoundaryMove(x, y, xStart, yStart, xEnd, yEnd, deltaX, deltaY, pCharge, g);
	                        }

	        createBoundaryCells(g);


	}
}

	// for(i = 0; i < sett->numOfParticles; i++){
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].x);
	// 	printf("%lg\n", sim->particles[i].x);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].y);
	// 	printf("%lg\n", sim->particles[i].y);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].radius);
	// 	printf("%lg\n", sim->particles[i].radius);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].vx);
	// 	printf("%lg\n", sim->particles[i].vx);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].vy);
	// 	printf("%lg\n", sim->particles[i].vy);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].ax);
	// 	printf("%lg\n", sim->particles[i].ax);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].ay);
	// 	printf("%lg\n", sim->particles[i].ay);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].mass);
	// 	printf("%lg\n", sim->particles[i].mass);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].charge);
	// 	printf("%lg\n", sim->particles[i].charge);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevX);
	// 	printf("%lg\n", sim->particles[i].prevX);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevY);
	// 	printf("%lg\n", sim->particles[i].prevY);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].Ex);
	// 	printf("%lg\n", sim->particles[i].Ex);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].Ey);
	// 	printf("%lg\n", sim->particles[i].Ey);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].Bz);
	// 	printf("%lg\n", sim->particles[i].Bz);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevpositionComponentForceX);
	// 	printf("%lg\n", sim->particles[i].prevpositionComponentForceX);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevpositionComponentForceY);
	// 	printf("%lg\n", sim->particles[i].prevpositionComponentForceY);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevtangentVelocityComponentOfForceX);
	// 	printf("%lg\n", sim->particles[i].prevtangentVelocityComponentOfForceX);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevtangentVelocityComponentOfForceY);
	// 	printf("%lg\n", sim->particles[i].prevtangentVelocityComponentOfForceY);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevnormalVelocityComponentOfForceX);
	// 	printf("%lg\n", sim->particles[i].prevnormalVelocityComponentOfForceX);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevnormalVelocityComponentOfForceY);
	// 	printf("%lg\n", sim->particles[i].prevnormalVelocityComponentOfForceY);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevBz);
	// 	printf("%lg\n", sim->particles[i].prevBz);
	// 	// fscanf(inParticles, "%lg", &sim->particles[i].prevLinearDragCoefficient);
	// 	printf("%lg\n", sim->particles[i].prevLinearDragCoefficient);
	// }

	return 0;
}