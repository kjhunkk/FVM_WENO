#include "ControlArea.h"

ControlArea::ControlArea(Type PDE, Type initial, int_t NFX, real_t area)
{
	_PDE = PDE;
	_initial = initial;
	_NFX = NFX;
	_totalNFX = NFX;
	_num_sideGhost = 0;
	_area = area;
	_dx = 0.0;
}

ControlArea::~ControlArea()
{

}

int_t ControlArea::generate(int_t num_sideGhost, real_t dx)
{
	_num_sideGhost = num_sideGhost;
	_totalNFX += 2 * num_sideGhost;
	_X.resize(_totalNFX);
	_U.resize(_totalNFX);
	_dx = dx;

	// Generate X coordinate
	for (int_t i = 0; i < _X.size(); ++i)
	{
		_X[i] = -(0.5*_area) + (i - _num_sideGhost + 0.5)*_dx;
	}
	
	// Apply initial condition
	if (_initial == "square")
		_U = initial_condition_1();

	else if (_initial == "halfdome")
		_U = initial_condition_2();

	else if (_initial == "gauss")
		_U = initial_condition_3();

	else if (_initial == "shock")
		_U = initial_condition_4();

	else if (_initial == "expansion")
		_U = initial_condition_5();

	else if (_initial == "sine")
		_U = initial_condition_6();

	else std::cout << "!!There is no corresponding initial condition!!\n";

	return _totalNFX;
}

vector_d ControlArea::applyBC(Type BC)
{
	if (BC == "periodic")
	{
		for (int_t i = 0; i < _num_sideGhost; i++)
		{
			_U[i] = _U[_totalNFX - 2 * _num_sideGhost + i];
			_U[_totalNFX - _num_sideGhost + i] = _U[_num_sideGhost + i];
		}
	}

	if (BC == "constant")
	{
		for (int_t i = 0; i < _num_sideGhost; i++)
		{
			_U[i] = _U[_num_sideGhost];
			_U[_totalNFX - _num_sideGhost + i] = _U[_totalNFX - _num_sideGhost - 1];
		}
	}

	return _U;
}

vector_d ControlArea::initial_condition_1() const
{
	vector_d U;
	U.resize(_totalNFX);
	for (int_t i = 0; i < _totalNFX; ++i)
	{
		if ((_X[i] <= 0.5) && (_X[i] >= -0.5)) U[i] = 1.0;
		else U[i] = 0.0;
	}

	return U;
}

vector_d ControlArea::initial_condition_2() const
{
	vector_d U;
	U.resize(_totalNFX);
	for (int_t i = 0; i < _totalNFX; ++i)
	{
		if ((_X[i] <= sqrt(3.0 / 10.0)) && (_X[i] >= -sqrt(3.0 / 10.0)))
			U[i] = sqrt(1 - 10.0*CONST13*_X[i] * _X[i]);
		else U[i] = 0.0;
	}

	return U;
}

vector_d ControlArea::initial_condition_3() const
{
	vector_d U;
	U.resize(_totalNFX);
	for (int_t i = 0; i < _totalNFX; ++i)
	{
		U[i] = exp(-300.0*_X[i] * _X[i]);
	}

	return U;
}

vector_d ControlArea::initial_condition_4() const
{
	vector_d U;
	U.resize(_totalNFX);
	for (int_t i = 0; i < _totalNFX; ++i)
	{
		if (_X[i] <= -0.3) U[i] = 1.2;
		else U[i] = 0.4;
	}

	return U;
}

vector_d ControlArea::initial_condition_5() const
{
	vector_d U;
	U.resize(_totalNFX);
	for (int_t i = 0; i < _totalNFX; ++i)
	{
		if (_X[i] <= -0.5) U[i] = 0.0;
		else U[i] = 1.0;
	}

	return U;
}

vector_d ControlArea::initial_condition_6() const
{
	vector_d U;
	U.resize(_totalNFX);
	for (int_t i = 0; i < _totalNFX; ++i)
	{
		U[i] = 1 + 0.5*sin(2.0*M_PI*_X[i]);
	}

	return U;
}