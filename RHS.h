#pragma once
#include "Datatype.h"
#include "WENO_Flux.h"

class RHS
{
public:
	// Constructor
	// p.m. PDE type, polynomial degree, total number of grids, number of side ghost, grid size
	RHS(Type, int_t, int_t, int_t, real_t);
	~RHS();

public:
	// Functions
	// Solve Right hand side
	// p.m. solution variable
	vector_d solve(vector_d) const;

protected:
	// Variables
	std::shared_ptr<WENO_Flux> Pflux;
	std::shared_ptr<WENO_Flux> Mflux;
	Type _PDE;
	int_t _r; /// polynomial degree
	int_t _num_sideGhost; /// number of side ghost
	real_t _dx;

protected:
	// Functions
};