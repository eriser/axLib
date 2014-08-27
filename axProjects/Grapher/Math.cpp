#include "Math.h"

Math::Math(const double& delta) :
_delta(delta),
_oneOverDelta(1.0 / _delta)
{
	_nCore = std::thread::hardware_concurrency();
}

double Math::Integrate(function<double(double)> f, const Range& range)
{
	return DispatchThread(&Math::integrate_thread, f, range);
	//return integrate_thread(f, range);
}

double Math::Integrate(string f, const Range& range)
{
	double x;
	exprtk::symbol_table<double> symbol_table;
	symbol_table.add_variable("x", x);
	symbol_table.add_constants();

	exprtk::expression<double> expression;
	expression.register_symbol_table(symbol_table);

	exprtk::parser<double> parser;
	parser.compile(f, expression);

	double v = 0;
	x = range.left;
	v += expression.value();
	x = range.right;
	v += expression.value();
	v *= 0.5;

	for (x = range.left + _delta;
		 x < range.right - _delta; 
		 x += _delta)
	{
		v += expression.value();
	}

	return _delta * v;
}

double Math::Length(function<double(double)> f, const Range& range)
{
	return DispatchThread(&Math::length_thread, f, range);
}

double Math::Derivate(function<double(double)> f, double x) const
{
	return (f(x + _delta) - f(x)) * _oneOverDelta;
}

double Math::Derivate(string f, double x) const
{
	double t;
	exprtk::symbol_table<double> symbol_table;
	symbol_table.add_variable("x", t);
	symbol_table.add_constants();

	exprtk::expression<double> expression;
	expression.register_symbol_table(symbol_table);

	exprtk::parser<double> parser;
	parser.compile(f, expression);

	t = x + _delta;
	double v = expression.value();
	t = x;
	v -= expression.value();
	v *= _oneOverDelta;;
	return v;
}

double Math::integrate_thread(function<double(double)> f, const Range& range)
{
	//Trapeze formula.
	//Integral from a->b of f(x)dx is approximate by : 
	//(b-a)/2n * [f(x0) + 2f(x1) + 2f(x2) + ... + 2f(x n-1) + f(x n)]
	double v = (f(range.left) + f(range.right)) * 0.5;

	for (double x = range.left + _delta;
		x < range.right - _delta; x += _delta)
	{
		v += f(x);
	}

	return _delta * v;
}

double Math::DispatchThread(double(Math::*operation)(CALLBACK_PARAMS),
							function<double(double)> f,
							const Range& range)
{
	double one_over_ncore = 1.0 / _nCore;

	double length = range.right - range.left;

	for (unsigned int i = 0; i < _nCore; i++)
	{
		double left = range.left + one_over_ncore * i * length;
		double right = range.left + one_over_ncore * (i + 1) * length;

		_threads.push_back(std::async(std::ref(operation),
			std::ref(*this),
			f, Range(left, right)));
	}

	double value = 0.0;
	for (unsigned int i = 0; i < _threads.size(); i++)
	{
		// Waits for it to return.
		value += _threads[i].get();
	}
	_threads.clear();
	return value;
}

double Math::length_thread(function<double(double)> f, const Range& range)
{
	//Formule Trapeze +
	//Longueur d'une courbe :
	//Integral de a->b de sqrt(1 + (f'(x))^2)
	double v = sqrt(1 + pow(Derivate(f, range.left), 2)) +
		sqrt(1 + pow(Derivate(f, range.right), 2));

	for (double x = range.left + _delta;
		x < range.right - _delta; x += _delta)
	{
		v += sqrt(1.0 + pow(Derivate(f, x), 2));
	}

	return _delta * v;
}

