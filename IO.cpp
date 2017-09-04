#include "IO.h"

IO::IO()
{
	_initial = "";
	_boundary = "";
	_timeITG = 0;
	_num_grids = 0;
	_num_total_grids = 0;
	_num_sideGhost = 0;
	_targetTime = 0;
	_CFL = 0;
	_r = 0;
}

IO::~IO()
{

}

bool IO::initialize()
{
	bool cond = 0;
	cond = readCondition();
	_num_sideGhost = 3;
	_num_grids = int(_area / _gridSize + epsilon);
	_num_total_grids = _num_grids + 2 * _num_sideGhost;
	print();

	return cond;
}

void IO::output(std::string index, vector_d x, vector_d u) const
{
	std::ofstream file;
	std::string FileName = "./output/";
	FileName += "_WENO_";
	std::string scheme = "WENO ";
	FileName += _PDE;
	FileName += "_";
	FileName += _initial;
	FileName += "_CFL";
	FileName += std::to_string(_CFL);
	FileName += "_";
	FileName += index;
	FileName += ".plt";
	scheme += _PDE;
	file.open(FileName, std::ios::trunc);
	if (file.is_open())
	{
		std::cout << "!!output file open!!\n";
		file << "variables = \"X\", \"Velocity\"\n";
		file << "zone t = \"" << scheme << "\", i=" << _num_grids << ", f=point\n";
		for (int i = _num_sideGhost; i < x.size() - _num_sideGhost; ++i)
		{
			file << x[i] << "\t\t" << u[i] << "\n";
		}
		file.close();
	}
	else std::cout << "!!output file error!!\n";
}

bool IO::readCondition()
{
	std::ifstream file;
	file.open("./input.inp");
	bool file_open = file.is_open();

	// Clear stream state
	file.clear();

	// Read file
	while (file.is_open())
	{
		// Read text
		std::string text;
		std::getline(file, text);
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		// Read PDE type
		if (text.find("$$PDETYPE=", 0) != std::string::npos)
			_PDE = text.substr(10);

		// Read initial condition
		if (text.find("$$INITIAL=", 0) != std::string::npos)
			_initial = text.substr(10);

		// Read boudnary condition
		if (text.find("$$BOUNDARY=", 0) != std::string::npos)
			_boundary = text.substr(11);

		// Read time integration type
		if (text.find("$$TIMEINTEGRATION=", 0) != std::string::npos)
			_timeITG = std::stoi(text.substr(18));

		// Read target time
		if (text.find("$$TARGETTIME=", 0) != std::string::npos)
			_targetTime = std::stod(text.substr(13));

		// Read area size
		if (text.find("$$AREA=", 0) != std::string::npos)
			_area = std::stod(text.substr(7));

		// Read grid size
		if (text.find("$$GRIDSIZE=", 0) != std::string::npos)
			_gridSize = std::stod(text.substr(11));

		// Read target time
		if (text.find("$$CFL=", 0) != std::string::npos)
			_CFL = std::stod(text.substr(6));

		// Read order of polynomial
		if (text.find("$$POLYNOMIALORDER=", 0) != std::string::npos)
			_r = std::stoi(text.substr(18));

		if (file.eof()) file.close();
	}

	return file_open;
}

void IO::print() const
{
	std::cout << "----------Conditions----------\n";
	std::cout << "$$ PDE type            : " << _PDE << "\n";
	std::cout << "$$ Initial condition   : " << _initial << "\n";
	std::cout << "$$ Boundary condition  : " << _boundary << "\n";
	std::cout << "$$ Time integration    : " << _timeITG << "\n";
	std::cout << "$$ Area                : " << _area << "\n";
	std::cout << "$$ Number of grids     : " << _num_grids << "\n";
	std::cout << "$$ Grid size           : " << _gridSize << "\n";
	std::cout << "$$ Target time         : " << _targetTime << "\n";
	std::cout << "$$ CFL number          : " << _CFL << "\n";
	std::cout << "$$ Order of polynomial : " << _r << "\n";
	std::cout << "------------------------------\n";
}