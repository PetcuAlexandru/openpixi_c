#ifndef CELL_H
#define CELL_H

typedef struct cell
{
	/**electric current in x-Direction*/
	double jx;
	/**electric current in y-Direction*/
	double jy;

	/**sum of electric charges in a cell*/
	double rho;
	/**electrostatic potential*/
	double phi;

	/**electric field in x direction at time t*/
	double ex;
	/**electric field in y direction at time t*/
	double ey;
	/**magnetic field in z direction at time t+dt/2*/
	double bz;

	/**magnetic field in z direction at time t-dt/2*/
	double bzo;

}Cell;

// void addJx(Cell *cell, double value){
// 	cell->jx += value;
// }


// void addJy(Cell *cell, double value){
// 	cell->jy += value;
// }


// void addRho(Cell *cell, double value){
// 	cell->rho += value;
//}

#endif