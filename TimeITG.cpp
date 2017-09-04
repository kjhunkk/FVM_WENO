#include "TimeITG.h"

TimeITG::TimeITG(std::shared_ptr<IO> condition)
{
	_dt = 0.0;
	_dx = condition->getGridSize();
	_PDE = condition->getPDE();
	_boundary = condition->getBoundary();
	_timeITG = condition->getTimeITG();
	_num_totalNFX = condition->getTotalNFX();
	_r = condition->getOrder();
	_num_sideGhost = condition->getSideGhost();
	_CFL = condition->getCFL();

	rhs = std::make_shared<RHS>(_PDE, _r, _num_totalNFX, _num_sideGhost, _dx);
}

TimeITG::~TimeITG()
{

}

vector_d TimeITG::solve(std::shared_ptr<ControlArea> field)
{
	_dt = calDT(field->getU());
	vector_d solution;

	switch (_timeITG)
	{
	case 1:
		solution = Euler(field, _dt);
		break;
	case 3:
		solution = RK3(field, _dt);
		break;
	default:
		std::cout << "!!no solver!!\n";
	}

	return solution;
}

vector_d TimeITG::solve(std::shared_ptr<ControlArea> field, real_t dt)
{
	vector_d solution;

	switch (_timeITG)
	{
	case 1:
		solution = Euler(field, dt);
		break;
	case 3:
		solution = RK3(field, dt);
		break;
	default:
		std::cout << "!!no solver!!\n";
	}

	return solution;
}

real_t TimeITG::calDT(vector_d U) const
{
	if (_PDE == "advection")
		return _CFL*_dx / abs(CONST_A);

	if (_PDE == "burgers")
	{
		vector_d s;
		for (int_t i = 0; i < _num_totalNFX - 1; ++i)
		{
			s.push_back(0.5*abs(U[i] + U[i + 1]));
		}
		real_t maxSpeed = *std::max_element(s.begin(), s.end());

 		return _CFL*_dx / maxSpeed;
	}

	else return 0.0;
}

vector_d TimeITG::RK3(std::shared_ptr<ControlArea> field, real_t dt) const
{
	// ControlArea for solution
	std::shared_ptr<ControlArea> temp_field = std::make_shared<ControlArea>(*field);

	// Runge-Kutta step 1
	// Apply boundary condition
	temp_field->applyBC(_boundary);

	// Declare solution variables
	vector_d U0 = temp_field->getU(); // original solution
	vector_d U1 = temp_field->getU(); // 1 step solution

	// Calculate RHS
	vector_d L0 = rhs->solve(temp_field->getU());

	// Update
	for (int_t i = _num_sideGhost; i < U1.size() - _num_sideGhost; ++i)
	{
		U1[i] = U0[i] + dt*L0[i];
	}
	temp_field->setU(U1);

	// Runge-Kutta step 2
	// Apply boundary condition
	temp_field->applyBC(_boundary);

	vector_d U2 = temp_field->getU(); // 2 step solution

	// Calculate RHS
	L0 = rhs->solve(temp_field->getU());

	// Update
	for (int_t i = _num_sideGhost; i < U2.size() - _num_sideGhost; ++i)
	{
		U2[i] = 0.75*U0[i] + 0.25*(U1[i] + dt*L0[i]);
	}
	temp_field->setU(U2);

	// Runge-Kutta final step
	// Apply boundary condition
	temp_field->applyBC(_boundary);

	vector_d Uf = temp_field->getU(); // final step solution

	// Calculate RHS
	L0 = rhs->solve(temp_field->getU());
	for (int_t i = _num_sideGhost; i < Uf.size() - _num_sideGhost; ++i)
	{
		Uf[i] = CONST13*U0[i] + CONST23*(U2[i] + dt*L0[i]);
	}

	return Uf;
}

vector_d TimeITG::Euler(std::shared_ptr<ControlArea> field, real_t dt) const
{
	field->applyBC(_boundary);
	vector_d Uf = field->getU();
	vector_d L0 = rhs->solve(field->getU());
	for (int_t i = _num_sideGhost; i < Uf.size() - _num_sideGhost; ++i)
	{
		Uf[i] = Uf[i] + dt*L0[i];
	}

	return Uf;
}