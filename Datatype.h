#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <memory>

// Define direction type
// #1 : forward, #0 : backward
typedef bool Direction;

// Define condition type
// e.g. initial condition, flux, etc
typedef std::string Type;

// Define double vector
typedef std::vector<double> vector_d;

// Define integer type
typedef int int_t;

// Define real type
typedef double real_t;

// Frequently used constants
#define forward 1

#define backward 0

#define epsilon 1.0e-8

const real_t CONST_A = 0.5; /// advection speed

const real_t CONST13 = 1.0 / 3.0;

const real_t CONST23 = 2.0 / 3.0;

const real_t CONST43 = 4.0 / 3.0;

// Class alert
class Alert
{
protected:
	Alert();
	~Alert();

public:
	// Print error message
	static void error(const std::string& str, const std::string& file, int_t line);
	static void message(const std::string& str);

private:
	// Alert variable
	static Alert _alert;
};

// Message macro
#define ERROR(str) Alert::error(str, __FILE__, __LINE__)
#define MESSAGE(str) Alert::message(str);