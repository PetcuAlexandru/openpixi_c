#ifndef GRID_H
#define GRID_H

#include "Cell.h"

//boundary
#define X_MIN 0
#define X_CENTER 1
#define X_MAX 2
#define Y_MIN 0
#define Y_CENTER 3
#define Y_MAX 6

#define INTERPOLATION_RADIUS 1
#define HARDWALL_SAFETY_CELLS 1
#define EXTRA_CELLS_BEFORE_GRID INTERPOLATION_RADIUS + HARDWALL_SAFETY_CELLS - 1 + 1
#define EXTRA_CELLS_AFTER_GRID INTERPOLATION_RADIUS + HARDWALL_SAFETY_CELLS

typedef struct grid
{
	Cell **cells;

	/**number of cells in x direction*/
	int numCellsX;
	/**number of cells in y direction*/
	int numCellsY;
	/**total number of cells in x direction*/
	int numCellsXTotal;
	/**total number of cells in y direction*/
	int numCellsYTotal;
	/**width of each cell*/
	double cellWidth;
	/**height of each cell*/
	double cellHeight;
}Grid;

typedef struct boundary
{
	double xoffset;
	double yoffset;
}Boundary;


void addJx(Grid *g, int x, int y, double value);

void addJy(Grid *g, int x, int y, double value);

void addRho(Grid *g, int x, int y, double value);

void addEx(Grid *g, int x, int y, double value);

void addEy(Grid *g, int x, int y, double value);

void addBz(Grid *g, int x, int y, double value);

Cell getCell(Grid *g, int x, int y);

void set(Grid *g, int numCellsX, int numCellsY, double simWidth, double simHeight);

void createGridWithBoundaries();

void updateGrid(double tstep);

void resetCurrent(Grid *g);

void resetCharge();

void storeFields();

int indx(int clientIdx);

int getNumCellsXTotal(Grid *g);

int getNumCellsYTotal(Grid *g);

void createBoundaryMap(Boundary *b, double width, double height);

int get_region(double xmin, double xmax, double ymin, double ymax, double width, double height);

void fourBoundaryMove(int lx, int ly, double x, double y, double deltaX, 
                      double deltaY, double pCharge, Grid *g);

void sevenBoundaryMove(double x, double y, int xStart, int yStart, int xEnd, int yEnd,
					   double deltaX, double deltaY, double pCharge, Grid *g);

void tenBoundaryMove(double x, double y, int xStart, int yStart, int xEnd, int yEnd,
					 double deltaX, double deltaY, double pCharge, Grid *g);

void createBoundaryCell(Grid *g, int x, int y);

void createBoundaryCells(Grid *g);


#endif