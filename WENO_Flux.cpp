#include "WENO_Flux.h"

vector_d WENO_Flux::_U;

WENO_Flux::WENO_Flux(Direction fluxDirec, Type PDE, int_t r, int_t totalNFX, int_t num_sideGhost)
{
	_fluxDirec = fluxDirec;
	_PDE = PDE;
	_r = r;
	_num_sideGhost = num_sideGhost;
	_p = 2;
	_maxU = 0.0;
	_IS.resize(_r);
	_alpha.resize(_r);
	_W.resize(_r);
}

WENO_Flux::~WENO_Flux()
{

}

vector_d WENO_Flux::calculate()
{
	vector_d flux(_U.size(), 0.0);

	real_t maxU = *std::max_element(_U.begin(), _U.end());
	real_t minU = *std::min_element(_U.begin(), _U.end());
	_maxU = maxU > abs(minU) ? maxU : abs(minU);
	
	for (int_t i = _num_sideGhost; i < flux.size() - _num_sideGhost; ++i)
	{
		_IS = calIS(i, _U);
		_C = assignC(_U, i);
		_alpha = calAlpha(_C, _IS);
		_W = calWeight(_alpha);
		_L = constLagrange(i, _U);

		for (int_t j = 0; j < _r; ++j)
		{
			flux[i] += _W[j] * _L[j];
		}
	}

	return flux;
}

real_t WENO_Flux::flux(real_t u)
{
	if (_PDE == "advection")
		return CONST_A*u;

	if (_PDE == "burgers")
		return 0.5*u*u;

	else std::cout << "_PDE type error\n";

	return 0;
}

vector_d WENO_Flux::calIS(int_t j, vector_d U)
{
	vector_d IS(_r, 0.0);

	// Define flow direction
	bool flowDirection;
	if (((_PDE == "advection") && (CONST_A >= 0.0)) || ((_PDE == "burgers") && (U[j] >= 0.0)))
		flowDirection = forward;
	else flowDirection = backward;

	// index begin
	int begin;
	if ((flowDirection == forward) && (_fluxDirec == forward)) begin = j - 1;
	else if ((flowDirection == forward) && (_fluxDirec == backward)) begin = j - 2;
	else if ((flowDirection == backward) && (_fluxDirec == forward)) begin = j;
	else begin = j - 1;

	// For 2nd degree of polynomial
	if (_r == 2)
	{
		IS[0] = pow(flux(U[begin + 1]) - flux(U[begin]), 2.0);
		IS[1] = pow(flux(U[begin + 2]) - flux(U[begin + 1]), 2.0);
	}

		// For 3rd degree of polynomial
	if (_r == 3)
	{
		begin--;
		IS[0] = CONST1312*pow(flux(U[begin]) - 2.0*flux(U[begin + 1]) + flux(U[begin + 2]), 2.0) + 0.25*pow(flux(U[begin]) - 4.0*flux(U[begin + 1]) + 3.0*flux(U[begin + 2]), 2.0);
		IS[1] = CONST1312*pow(flux(U[begin + 1]) - 2.0*flux(U[begin + 2]) + flux(U[begin + 3]), 2.0) + 0.25*pow(flux(U[begin + 1]) - flux(U[begin + 3]), 2.0);
		IS[2] = CONST1312*pow(flux(U[begin + 2]) - 2.0*flux(U[begin + 3]) + flux(U[begin + 4]), 2.0) + 0.25*pow(3.0*flux(U[begin + 2]) - 4.0*flux(U[begin + 3]) + flux(U[begin + 4]), 2.0);
	}
	
	return IS;
}

vector_d WENO_Flux::calAlpha(vector_d C, vector_d IS)
{
	vector_d alpha(_r, 0.0);

	for (int_t i = 0; i < _r; ++i)
	{
		alpha[i] = C[i] / pow(EP + IS[i], _p);
	}

	return alpha;
}

vector_d WENO_Flux::assignC(vector_d U, int_t i)
{
	vector_d C;
	bool flowDirection;
	
	// Need to change optimal weights for flow direction
	if (((_PDE == "advection") && (CONST_A >= 0.0)) || ((_PDE == "burgers") && (U[i] >= 0.0)))
		flowDirection = true;
	else flowDirection = false;

	if (_r == 1)
	{
		C.resize(1);
		C[0] = 1.0;
	}

	else if (_r == 2)
	{
		C.resize(2);
		if (flowDirection == true)
		{
			C[0] = CONST13;// 0.25;
			C[1] = CONST23;// 0.75;
		}
		else
		{
			C[0] = CONST23;
			C[1] = CONST13;
		}
	}

	else if (_r == 3)
	{
		C.resize(3);
		if (flowDirection == true)
		{
			C[0] = 0.1;
			C[1] = 0.6;
			C[2] = 0.3;
		}
		else
		{
			C[0] = 0.3;
			C[1] = 0.6;
			C[2] = 0.1;
		}
	}

	return C;
}

vector_d WENO_Flux::calWeight(vector_d alpha)
{
	vector_d W(_r, 0.0);
	real_t sum_alpha = 0.0;

	for (int_t i = 0; i < alpha.size(); ++i)
		sum_alpha += alpha[i];

	for (int_t i = 0; i < _r; ++i)
	{
		W[i] = alpha[i] / sum_alpha;
	}

	if (_r == 1) W[0] = 1.0;

	return W;
}

vector_d WENO_Flux::constLagrange(int_t j, vector_d U)
{
	vector_d q(_r, 0.0);

	// Define flow direction
	double flowDirection;
	if (((_PDE == "advection") && (CONST_A >= 0.0)) || ((_PDE == "burgers") && (U[j] >= 0.0)))
		flowDirection = 1.0;
	else flowDirection = 0.0;

	// index begin
	int begin;
	if ((flowDirection == 1.0) && (_fluxDirec == forward)) begin = j - 1;
	else if ((flowDirection == 1.0) && (_fluxDirec == backward)) begin = j - 2;
	else if ((flowDirection == 0.0) && (_fluxDirec == forward)) begin = j;
	else begin = j - 1;

	if (_r == 1) q[0] = flux(U[begin + 1]);

	else if (_r == 2)
	{
		q[0] = (0.5 - flowDirection)*flux(U[begin]) + (0.5 + flowDirection)*flux(U[begin + 1]);
		q[1] = (1.5 - flowDirection)*flux(U[begin + 1]) + (-0.5 + flowDirection)*flux(U[begin + 2]);
	}

	else if (_r == 3)
	{
		begin--;
		if (flowDirection == 1.0)
		{
			q[0] = 0.375*flux(U[begin]) - 1.25*flux(U[begin + 1]) + 1.875*flux(U[begin + 2]);
			q[1] = -0.125*flux(U[begin + 1]) + 0.75*flux(U[begin + 2]) + 0.375*flux(U[begin + 3]);
			q[2] = 0.375*flux(U[begin + 2]) + 0.75*flux(U[begin + 3]) - 0.125*flux(U[begin + 4]);
		}
		else
		{
			q[0] = -0.125*flux(U[begin]) + 0.75*flux(U[begin + 1]) + 0.375*flux(U[begin + 2]);
			q[1] = 0.375*flux(U[begin + 1]) + 0.75*flux(U[begin + 2]) - 0.125*flux(U[begin + 3]);
			q[2] = 1.875*flux(U[begin + 2]) - 1.25*flux(U[begin + 3]) + 0.375*flux(U[begin + 4]);
		}

	}

	return q;
}