#include "Grid.h"

void addJx(Grid *g, int x, int y, double value){
	g->cells[indx(x)][indx(y)].jx += value;
}

void addJy(Grid *g, int x, int y, double value){
	g->cells[indx(x)][indx(y)].jy += value;	
}

void addRho(Grid *g, int x, int y, double value){
	g->cells[indx(x)][indx(y)].rho += value;
}

void addEx(Grid *g, int x, int y, double value){
	g->cells[indx(x)][indx(y)].ex += value;
}

void addEy(Grid *g, int x, int y, double value){
	g->cells[indx(x)][indx(y)].ey += value;
}

void addBz(Grid *g, int x, int y, double value){
	g->cells[indx(x)][indx(y)].bz += value;
}

Cell getCell(Grid *g, int x, int y){
	return g->cells[indx(x)][indx(y)];
}

void set(Grid *g, int numCellsX, int numCellsY, double simWidth, double simHeight){
	g->numCellsX = numCellsX;
	g->numCellsY = numCellsY;
	g->cellWidth = simWidth/numCellsX;
	g->cellHeight = simHeight/numCellsY;
	g->numCellsXTotal = getNumCellsXTotal(g);
	g->numCellsYTotal = getNumCellsYTotal(g);
}

void createGridWithBoundaries();

void updateGrid(double tstep);

void resetCurrent(Grid *g){
	int i, j;
	for(i = 0; i < g->numCellsX; i++){
		for(j = 0; j < g->numCellsY; j++){
			g->cells[indx(i)][indx(j)].jx = 0;
			g->cells[indx(i)][indx(j)].jy = 0;
		}
	}
}

void resetCharge();

void storeFields();

int indx(int clientIdx){
	return EXTRA_CELLS_BEFORE_GRID + clientIdx;
}

int getNumCellsXTotal(Grid *g){
	return g->numCellsX + EXTRA_CELLS_BEFORE_GRID + EXTRA_CELLS_AFTER_GRID;
}

int getNumCellsYTotal(Grid *g){
	return g->numCellsY + EXTRA_CELLS_BEFORE_GRID + EXTRA_CELLS_AFTER_GRID;
}

void createBoundaryMap(Boundary *b, double width, double height){
	b[X_MIN + Y_MIN].xoffset = -width;
	b[X_MIN + Y_MIN].yoffset = -height;

	b[X_CENTER + Y_MIN].xoffset = 0;
	b[X_CENTER + Y_MIN].yoffset = -height;

	b[X_MAX + Y_MIN].xoffset = width;
	b[X_MAX + Y_MIN].yoffset = -height;

	b[X_MIN + Y_CENTER].xoffset = -width;
	b[X_MIN + Y_CENTER].yoffset = 0;

	b[X_CENTER + Y_CENTER].xoffset = 0;
	b[X_CENTER + Y_CENTER].yoffset = 0;

	b[X_MAX + Y_CENTER].xoffset = width;
	b[X_MAX + Y_CENTER].yoffset = 0;

	b[X_MIN + Y_MAX].xoffset = -width;
	b[X_MIN + Y_MAX].yoffset = height;

	b[X_CENTER + Y_MAX].xoffset = 0;
	b[X_CENTER + Y_MAX].yoffset = height;

	b[X_MAX + Y_MAX].xoffset = width;
	b[X_MAX + Y_MAX].yoffset = height;
}

int get_region(double xmin, double xmax, double ymin, double ymax, double width, double height) {
    int xidx;
    int yidx;

    if (xmin < 0) {
            xidx  = X_MIN;
    } else if (xmax >= width) {
            xidx = X_MAX;
    } else {
            xidx = X_CENTER;
    }

    if (ymin < 0) {
            yidx = Y_MIN;
    } else if (ymax >= height) {
            yidx = Y_MAX;
    } else {
            yidx = Y_CENTER;
    }

    return xidx + yidx;
}

void fourBoundaryMove(int lx, int ly, double x, double y, double deltaX, 
                      double deltaY, double pCharge, Grid *g){

	addJx(g, lx, 	ly - 1, pCharge * deltaX * ((1 - deltaY) / 2 - y));
	addJx(g, lx, 	ly, 	pCharge * deltaX * ((1 + deltaY) / 2 + y));
	addJy(g, lx - 1, ly, 	pCharge * deltaY * ((1 - deltaX) / 2 - x));
	addJy(g, lx, 	ly, 	pCharge * deltaY * ((1 + deltaX) / 2 + x));
}

void sevenBoundaryMove(double x, double y, int xStart, int yStart, int xEnd, int yEnd,
					   double deltaX, double deltaY, double pCharge, Grid *g){

	//7-boundary move with equal y?
	if (yStart == yEnd) {
		//particle moves right?
		if (xEnd > xStart) {

			double deltaX1 = 0.5 - x;
			double deltaY1 = (deltaY / deltaX) * deltaX1;
			fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

			deltaX -= deltaX1;
			deltaY -= deltaY1;
			y += deltaY1;
			fourBoundaryMove(xEnd, yEnd, - 0.5, y, deltaX, deltaY, pCharge, g);

		}
		//particle moves left
		else {

			double deltaX1 = -(0.5 + x);
			double deltaY1 = (deltaY / deltaX) * deltaX1;
			fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

			deltaX -= deltaX1;
			deltaY -= deltaY1;
			y += deltaY1;
			fourBoundaryMove(xEnd, yEnd, 0.5, y, deltaX, deltaY, pCharge, g);

		}
	}
	//7-boundary move with equal x?
	if (xStart == xEnd) {
		//particle moves up?
		if (yEnd > yStart) {

			double deltaY1 = 0.5 - y;
			double deltaX1 = deltaX  * (deltaY1 / deltaY);
			fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

			deltaX -= deltaX1;
			deltaY -= deltaY1;
			y += deltaY1;
			fourBoundaryMove(xEnd, yEnd, x, -0.5, deltaX, deltaY, pCharge, g);

		}
		//particle moves down
		else {

			double deltaY1 = -(0.5 + y);
			double deltaX1 = (deltaX / deltaY) * deltaY1;
			fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

			deltaX -= deltaX1;
			deltaY -= deltaY1;
			y += deltaY1;
			fourBoundaryMove(xEnd, yEnd, x, 0.5, deltaX, deltaY, pCharge, g);

		}
	}

}

void tenBoundaryMove(double x, double y, int xStart, int yStart, int xEnd, int yEnd,
					 double deltaX, double deltaY, double pCharge, Grid *g){

	//moved right?
	if (xEnd == (xStart+1)) {
		//moved up?
		if (yEnd == (yStart+1)) {

			double deltaX1 = 0.5 - x;

			//lower local origin
			if(((deltaY / deltaX) * deltaX1 + y) < 0.5) {

				double deltaY1 = (deltaY / deltaX) * deltaX1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaY2 = 0.5 - y - deltaY1;
				double deltaX2 = (deltaX1 / deltaY1) * deltaY2;
				y += deltaY1;
				fourBoundaryMove(xStart+1, yStart, -0.5, y, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				x = deltaX2 - 0.5;
				fourBoundaryMove(xEnd, yEnd, x, -0.5, deltaX, deltaY, pCharge, g);

			}
			//upper local origin
			else {

				double deltaY1 = 0.5 - y;
				deltaX1 = (deltaX / deltaY) * deltaY1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaX2 = 0.5 - x - deltaX1;
				double deltaY2 = (deltaY1 / deltaX1) * deltaX2;
				x += deltaX1;
				fourBoundaryMove(xStart, yStart+1, x, -0.5, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				y = deltaY2 - 0.5;
				fourBoundaryMove(xEnd, yEnd, -0.5, y, deltaX, deltaY, pCharge, g);

			}
		}
		//moved down
		else {

			double deltaY1 = -(0.5 + y);

			//lower local origin
			if(((deltaX / deltaY) * deltaY1 + x) < 0.5) {

				double deltaX1 = (deltaX / deltaY) * deltaY1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaX2 = 0.5 - x - deltaX1;
				double deltaY2 = (deltaY / deltaX) * deltaX2;
				x += deltaX1;
				fourBoundaryMove(xStart, yStart-1, x, 0.5, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				y = 0.5 + deltaY2;
				fourBoundaryMove(xEnd, yEnd, -0.5, y, deltaX, deltaY, pCharge, g);

			}
			//upper local origin
			else {

				double deltaX1 = 0.5 - x;
				deltaY1 = (deltaY / deltaX) * deltaX1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaY2 = -(0.5 + y + deltaY1);
				double deltaX2 = (deltaX1 / deltaY1) * deltaY2;
				y += deltaY1;
				fourBoundaryMove(xStart+1, yStart, -0.5, y, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				x = deltaX2 - 0.5;
				fourBoundaryMove(xEnd, yEnd, x, 0.5, deltaX, deltaY, pCharge, g);

			}
		}

	}
	//moved left
	else {
		//moved up?
		if (yEnd == (yStart+1)) {

			double deltaX1 = -(0.5 + x);
			//lower local origin
			if(((deltaY / deltaX) * deltaX1 + y) < 0.5) {

				double deltaY1 = (deltaY / deltaX) * deltaX1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaY2 = 0.5 - y - deltaY1;
				double deltaX2 = (deltaX1 / deltaY1) * deltaY2;
				y += deltaY1;
				fourBoundaryMove(xStart-1, yStart, 0.5, y, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				x = 0.5 + deltaX2;
				fourBoundaryMove(xEnd, yEnd, x, -0.5, deltaX, deltaY, pCharge, g);

			}
			//upper local origin
			else {

				double deltaY1 = 0.5 - y;
				deltaX1 = (deltaX / deltaY) * deltaY1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaX2 = -(0.5 + x + deltaX1);
				double deltaY2 = (deltaY1 / deltaX1) * deltaX2;
				x += deltaX1;
				fourBoundaryMove(xStart, yStart+1, x, -0.5, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				y = deltaY2 - 0.5;
				fourBoundaryMove(xEnd, yEnd, 0.5, y, deltaX, deltaY, pCharge, g);

			}
		}
		//moved down
		else {

			double deltaY1 = -(0.5 + y);
			//lower local origin
			if((-(deltaX / deltaY) * deltaY1 - x) < 0.5) {

				double deltaX1 = (deltaX / deltaY) * deltaY1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaX2 = -(0.5 + x + deltaX1);
				double deltaY2 = (deltaY / deltaX) * deltaX2;
				x += deltaX1;
				fourBoundaryMove(xStart, yStart-1, x, 0.5, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				y = 0.5 + deltaY2;
				fourBoundaryMove(xEnd, yEnd, 0.5, y, deltaX, deltaY, pCharge, g);

			}
			//upper local origin
			else {

				double deltaX1 = -(0.5 + x);
				deltaY1 = (deltaY / deltaX) * deltaX1;
				fourBoundaryMove(xStart, yStart, x, y, deltaX1, deltaY1, pCharge, g);

				double deltaY2 = -(0.5 + y + deltaY1);
				double deltaX2 = (deltaX1 / deltaY1) * deltaY2;
				y += deltaY1;
				fourBoundaryMove(xStart+1, yStart, 0.5, y, deltaX2, deltaY2, pCharge, g);

				deltaX -= (deltaX1 + deltaX2);
				deltaY -= (deltaY1 + deltaY2);
				x = 0.5 + deltaX2;
				fourBoundaryMove(xEnd, yEnd, x, 0.5, deltaX, deltaY, pCharge, g);

			}
		}
	}
}

void createBoundaryCell(Grid *g, int x, int y){
	int xmin = EXTRA_CELLS_BEFORE_GRID;
	int xmax = g->numCellsX + EXTRA_CELLS_BEFORE_GRID - 1;
	int ymin = EXTRA_CELLS_BEFORE_GRID;
	int ymax = g->numCellsY + EXTRA_CELLS_BEFORE_GRID - 1;

	int refX = x;
	int refY = y;
	if (x < xmin) {
		refX += g->numCellsX;
	} else if (x > xmax) {
		refX -= g->numCellsX;
	}
	if (y < ymin) {
		refY += g->numCellsY;
	} else if (y > ymax) {
		refY -= g->numCellsY;
	}

	g->cells[x][y].jx += g->cells[refX][refY].jx;
	g->cells[refX][refY].jx = g->cells[x][y].jx;

	g->cells[x][y].jy += g->cells[refX][refY].jy;
	g->cells[refX][refY].jy = g->cells[x][y].jy;

}

void createBoundaryCells(Grid *g){
	int x, y;

	// left boundary (with corner cells)
	for (x = 0; x < EXTRA_CELLS_BEFORE_GRID; x++) {
		for (y = 0; y < g->numCellsYTotal; y++) {
			createBoundaryCell(g, x, y);
		}
	}
	// right boundary (with corner cells)
	for (x = EXTRA_CELLS_BEFORE_GRID + g->numCellsX; x < g->numCellsXTotal; x++) {
		for (y = 0; y < g->numCellsYTotal; y++) {
			createBoundaryCell(g, x, y);
		}
	}
	// top boundary (without corner cells)
	for (x = EXTRA_CELLS_BEFORE_GRID; x < EXTRA_CELLS_BEFORE_GRID + g->numCellsX; x++) {
		for (y = 0; y < EXTRA_CELLS_BEFORE_GRID; y++) {
			createBoundaryCell(g, x, y);
		}
	}
	// bottom boundary (without corner cells)
	for (x = EXTRA_CELLS_BEFORE_GRID; x < EXTRA_CELLS_BEFORE_GRID + g->numCellsX; x++) {
		for (y = EXTRA_CELLS_BEFORE_GRID + g->numCellsY; y < g->numCellsYTotal; y++) {
			createBoundaryCell(g, x, y);
		}
	}
}
