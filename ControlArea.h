#pragma once
#include "Datatype.h"

class ControlArea
{
public:
	// Constructor
	// p.m. PDE type, initial condition, NFX, area
	ControlArea(Type, Type, int_t, real_t);
	~ControlArea();

public:
	// Functions
	inline vector_d getX() const { return _X; };

	inline vector_d getU() const { return _U; };

	inline vector_d getSpeed() const { return _S; };

	inline void setU(vector_d U) { _U = U; };

	// Generate control area
	// p.m. number of side ghost cell, grid size / r.t. number of total cell
	int_t generate(int_t, real_t);

	// Apply boundary condition
	// p.m. boundary condition
	vector_d applyBC(Type);

protected:
	// Variables
	Type _PDE;
	Type _initial;
	int_t _NFX;
	int_t _totalNFX;
	int_t _num_sideGhost;
	real_t _area;
	real_t _dx;
	vector_d _X;
	vector_d _U;
	vector_d _S;

protected:
	// Functions
	// Initial conditions
	// r.t. solution variables
	vector_d initial_condition_1() const;
	
	vector_d initial_condition_2() const;

	vector_d initial_condition_3() const;

	vector_d initial_condition_4() const;

	vector_d initial_condition_5() const;

	vector_d initial_condition_6() const;
};