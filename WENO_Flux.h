#pragma once
#include "Datatype.h"

#define EP 10.0e-6
class WENO_Flux
{
public:
	// Constructor
	// p.m. direction, PDE type, polynomial degree, number of total grids, number of side ghost
	WENO_Flux(Direction, Type, int_t, int_t, int_t);
	~WENO_Flux();
	
public:
	// Functions
	static inline void setU(vector_d U) { _U = U; };

	// calculate WENO flux
	// r.t. flux vector array
	vector_d calculate();

protected:
	// Variables
	static vector_d _U; /// solution
	vector_d _IS; /// smoothness
	vector_d _alpha;
	vector_d _C;
	vector_d _W; /// weights
	vector_d _L; /// Lagrange polynomial
	real_t _maxU;
	int_t _num_sideGhost; /// number of side ghost
	int_t _p; 
	int_t _r; /// polynomial degree
	Direction _fluxDirec;
	Type _PDE;

	// Constants
	real_t CONST1312 = 13.0 / 12.0;
	real_t CONST16 = 1.0 / 6.0;
	real_t CONST56 = 5.0 / 6.0;
	real_t CONST76 = 7.0 / 6.0;
	real_t CONST116 = 11.0 / 6.0;

protected:
	// Functions
	// Exact flux function
	// p.m. u / r.t. f(u)
	real_t flux(real_t);

	// Calculate smoothness array
	// p.m. index, solution variables / r.t. smoothness array
	vector_d calIS(int_t, vector_d);

	// Calculate alpha array
	// p.m. C, IS / r.t. alpha array
	vector_d calAlpha(vector_d, vector_d);

	// Assign C array / p.m. solution variables, index
	vector_d assignC(vector_d, int_t);

	// Calculate weight
	// p.m. alpha array / r.t. weight array
	vector_d calWeight(vector_d);

	// Calculate constant Lagrange polynomial
	// p.m. index, solution variables / r.t. lagrange array
	vector_d constLagrange(int_t, vector_d);
};