#include "RHS.h"

RHS::RHS(Type PDE, int_t r, int_t num_totalNFX, int_t num_sideGhost, real_t dx)
{
	_PDE = PDE;
	_r = r;
	_num_sideGhost = num_sideGhost;
	_dx = dx;
	Pflux = std::make_shared<WENO_Flux>(forward, PDE, r, num_totalNFX, num_sideGhost);
	Mflux = std::make_shared<WENO_Flux>(backward, PDE, r, num_totalNFX, num_sideGhost);
}

RHS::~RHS()
{

}

vector_d RHS::solve(vector_d U) const
{
	WENO_Flux::setU(U);
	vector_d pflux = Pflux->calculate();
	vector_d mflux = Mflux->calculate();
	
	vector_d rhs(U.size(), 0.0);
	for (int_t i = _num_sideGhost; i < U.size() - _num_sideGhost; ++i)
	{
		rhs[i] = -(pflux[i] - mflux[i]) / _dx;
	}

	return rhs;
}