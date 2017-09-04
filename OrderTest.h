#pragma once
#include "DataType.h"

class OrderTest
{
public:
	// Constructor
	OrderTest();

	// Destructor
	~OrderTest();

public:
	// Functions
	// Set exact solution
	void setExact(std::vector<real_t> exact) { _exact = exact; _num = exact.size(); }

	// Calculate error / p.m. computed solution
	real_t L1error(std::vector<real_t>);

	real_t L2error(std::vector<real_t>);

	real_t Linf_error(std::vector<real_t>);

protected:
	// Variables
	std::vector<real_t> _exact;
	int_t _num;
};