#include "IO.h"
#include "ControlArea.h"
#include "Datatype.h"
#include "TimeITG.h"
#include "OrderTest.h"

int main()
{
	// Read input file
	std::shared_ptr<IO> InOut = std::make_shared<IO>();
	if ((InOut->initialize()) == false) return 0;
	Type boundary = InOut->getBoundary();

	// Control area object
	std::shared_ptr<ControlArea> Field = std::make_shared<ControlArea>(InOut->getPDE(), InOut->getInitial(), InOut->getNFX(), InOut->getArea());

	// Generate solution field
	Field->generate(InOut->getSideGhost(), InOut->getGridSize());
	vector_d U = Field->getU();
	vector_d X = Field->getX();
	InOut->output("initial", X, U);

	// Order test object
	std::shared_ptr<OrderTest> orderTest = std::make_shared<OrderTest>();
	orderTest->setExact(U);

	// Time integration object
	std::shared_ptr<TimeITG> WENO = std::make_shared<TimeITG>(InOut);
	real_t dt = 0.0;
	real_t currentTime = 0.0;
	real_t targetTime = InOut->getTargetTime();

	// Iteration number
	int_t iter = 0;

	// Time integration process
	while (currentTime + dt < targetTime)
	{
		U = WENO->solve(Field);
		Field->setU(U);
		dt = WENO->getDT();
		currentTime += dt;
		iter++;
		InOut->output("result" + std::to_string(iter), X, Field->getU());
	}
	if (targetTime - currentTime > epsilon)
	{
		dt = targetTime - currentTime;
		U = WENO->solve(Field, dt);
		Field->setU(U);
		currentTime += dt;
		iter++;
		InOut->output("result" + std::to_string(iter), X, Field->getU());
	}

	std::cout << "Finished in time = " << currentTime << "\n";
	std::cout << "Last time step   = " << dt << "\n";

	InOut->output("result", X, Field->getU());

	std::cout << "L1 error = " << orderTest->L1error(Field->getU()) << "\n";
	std::cout << "L2 error = " << orderTest->L2error(Field->getU()) << "\n";
	std::cout << "Linf error = " << orderTest->Linf_error(Field->getU()) << "\n";

	return 0;
}