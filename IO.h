#pragma once
#include "Datatype.h"

class IO
{
public:
	IO();
	~IO();

public:
	// Functions
	inline Type getPDE() const { return _PDE; };

	inline Type getInitial() const { return _initial; };

	inline Type getBoundary() const { return _boundary; };

	inline int_t getTimeITG() const { return _timeITG; };

	inline int_t getNFX() const { return _num_grids; };

	inline int_t getTotalNFX() const { return _num_total_grids; };

	inline int_t getSideGhost() const { return _num_sideGhost; };

	inline int_t getOrder() const { return _r; };

	inline real_t getGridSize() const { return _gridSize; };

	inline real_t getArea() const { return _area; };

	inline real_t getTargetTime() const { return _targetTime; };

	inline real_t getCFL() const { return _CFL; };

	// Read files / r.t. true/false
	bool initialize();

	// export solutions / p.m. index, x coordinate, solution
	void output(std::string, vector_d, vector_d) const;

protected:
	// Variables
	Type _PDE;
	Type _initial;
	Type _boundary;
	int_t _timeITG;
	int_t _num_grids;
	int_t _num_total_grids;
	int_t _num_sideGhost;
	int_t _r;
	real_t _area;
	real_t _gridSize;
	real_t _targetTime;
	real_t _CFL;	

protected:
	// Functions
	// Read condition file / r.t. true/false
	bool readCondition();

	// Print inputs
	void print() const;
};