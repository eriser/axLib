#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#define WIN32
#include "exprtk.hpp"

#define MATH_FCT(x) [](const double& ##x)
#define CALLBACK_PARAMS function<double(double)> f, const Range& range

using namespace std;

struct Range
{
	Range(const double& l, const double& r) :
	left(l), right(r){}

	double left, right;
};

class Math
{
public:
	Math(const double& delta);

	double Integrate(function<double(double)> f, const Range& range);
	double Integrate(string f, const Range& range);

	double Derivate(function<double(double)> f, double x) const;
	double Derivate(string f, double x) const;

	double Length(function<double(double)> f, const Range& range);
	//double Length(string f, const Range& range);

private:
	double _delta, _oneOverDelta;
	unsigned int _nCore;

	vector<std::future<double>> _threads;

	double DispatchThread(double(Math::*operation)(CALLBACK_PARAMS),
						  function<double(double)> f,
						  const Range& range);

	double length_thread(function<double(double)> f, const Range& range);

	double integrate_thread(function<double(double)> f, const Range& range);
};