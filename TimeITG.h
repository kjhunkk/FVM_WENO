#pragma once
#include "Datatype.h"
#include "RHS.h"
#include "IO.h"
#include "ControlArea.h"

class TimeITG
{
public:
	// Constructor
	// p.m. IO object 
	TimeITG(std::shared_ptr<IO>);
	~TimeITG();

public:
	// Functions
	// get time step / must be used after solve
	inline real_t getDT() const { return _dt; };

	// Time integration
	// p.m. solution variable / r.t. updated solution variable
	vector_d solve(std::shared_ptr<ControlArea>);

	// Time integration with constant dt
	// p.m. solution variable, time step / r.t. updated solution variable
	vector_d solve(std::shared_ptr<ControlArea>, real_t);

protected:
	// Objects
	std::shared_ptr<RHS> rhs;

	// Variables
	real_t _dt;
	real_t _dx;
	real_t _CFL;
	int_t _num_totalNFX;
	int_t _timeITG;
	int_t _r;
	int_t _num_sideGhost;
	Type _PDE;
	Type _boundary;

protected:
	// Functions
	// Calculate time step
	// p.m. solution variables / r.t. time step
	real_t calDT(vector_d) const;

	// Runge-Kutta 3
	// p.m. solution variable, time step / r.t. updated solution variables
	vector_d RK3(std::shared_ptr<ControlArea>, real_t) const;

	// Euler explicit
	// p.m. solution variable / r.t. updated solution variables
	vector_d Euler(std::shared_ptr<ControlArea>, real_t) const;
};