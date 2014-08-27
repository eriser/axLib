//#include "Math.h"
#include "exprtk.hpp"
#include "axLib.h"


class Plot : public axPanel
{
public:
	Plot(axApp* app, axWindow* parent, 
			const axRect& rect, 
			const string& fct,
			const axRange<double>& range,
			const int& deltaX):
		axPanel(app, parent, rect),
		_range(range),
		_delta(deltaX)
	{
		/*double x;*/
		//exprtk::symbol_table<double> symbol_table;
		symbol_table.add_variable("x", _var_x);
		symbol_table.add_constants();

		//exprtk::expression<double> expression;
		expression.register_symbol_table(symbol_table);

		//exprtk::parser<double> parser;
		parser.compile(fct, expression);

	/*	double v = 0;*/
		_var_x = 0;

		double delta_x = _delta;
		double delta = (_range.right - _range.left) / delta_x;

		_plotFct.resize(int(delta_x));
		//double half_y = rect.size.y * 0.5;
		double max_value = -1000000;
		double min_value = 10000000;
		for (int i = 0; i < delta_x; i++)
		{
			_var_x = _range.left + i * delta;
			_plotFct[i] = expression.value();

			cout << _plotFct[i] << endl;

			if (_plotFct[i] < min_value)
				min_value = _plotFct[i];

			if (_plotFct[i] > max_value)
				max_value = _plotFct[i];
		}

		cout << "MIN : " << min_value << " MAX : " << max_value << endl;

		for (int i = 0; i < delta_x; i++)
		{
			// Everyting above 0.
			double t = _plotFct[i];// +abs(min_value);

			if (min_value < 0)
			{
				t += abs(min_value);
			}
			// Everything between 0 and 1.0.
			t /= (max_value - min_value);
			// 0 and 2.0
			t *= 2.0;

			// -1.0 to 1.0
			t -= 1.0;
			_plotFct[i] = t;
		}

		
		//double delta_x = _delta;
		//double delta = (_range.right - _range.left) / delta_x;
		double half_y = rect.size.y * 0.5;
		_points.resize(delta_x);
		for (int i = 0; i < delta_x; i++)
		{
			_points[i] = axPoint(i / delta_x * rect.size.x, 
								 half_y - _plotFct[i] * half_y);
		}

		double v = 0.0;
		if (min_value < 0)
			_grid.push_back(((v + abs(min_value)) / (max_value - min_value)) * 2.0 - 1.0);
		else
			_grid.push_back((v / (max_value - min_value)) * 2.0 - 1.0);
	}

	virtual void OnPaint()
	{
		axGC* gc = GetGC();
		axRect rect(GetRect());
		axRect rect0(0, 0, rect.size.x, rect.size.y);

		gc->SetColor(axColor(1.0, 1.0, 1.0));
		gc->DrawRectangle(rect0);

		double delta_x = _delta;
		double delta = (_range.right - _range.left) / delta_x;
		double half_y = rect.size.y * 0.5;

		gc->SetColor(axColor(0.7, 0.7, 0.7));
		//for (int& v : _grid)
		//{
			gc->DrawLine(axPoint(0,
				half_y - _grid[0] * half_y),
				axPoint(rect.size.x,
				half_y - _grid[0] * half_y), 1.0);
		//}


		gc->SetColor(axColor(0.0, 0.0, 0.0));
		gc->DrawLines(_points, 2.0);

		gc->SetColor(axColor(0.0, 0.0, 0.0));
		gc->DrawRectangleContour(axRect(0, 1, rect.size.x - 1, rect.size.y - 1), 1.0);


	/*	gc->DrawLine(axPoint((i - 1) / delta_x * rect.size.x,
			half_y - _plotFct[i - 1] * half_y),
			axPoint((i) / delta_x * rect.size.x,
			half_y - _plotFct[i] * half_y));*/
		
		//for (int i = 1; i < delta_x; i++)
		//{
		//	gc->DrawLine(axPoint((i-1) / delta_x * rect.size.x, 
		//						 half_y - _plotFct[i-1] * half_y),
		//		axPoint((i) / delta_x * rect.size.x, 
		//				half_y - _plotFct[i] * half_y), 2.0);
		//}
	}

	double _var_x;
	exprtk::symbol_table<double> symbol_table;
	exprtk::expression<double> expression;
	exprtk::parser<double> parser;

	vector<double> _plotFct;
	vector<axPoint> _points;
	axRange<double> _range;
	int _delta;
	vector<int> _grid;
};

class Grapher : public axPanel
{
public:
	Grapher(axApp* app, axWindow* parent,
		const axRect& rect,
		const string& fct,
		const axRange<double>& range,
		const int& deltaX) :
		axPanel(app, parent, rect)
	{
		Plot* plot = new Plot(app, this, axRect(20, 20, 460, 460), fct, range, deltaX);
	}

	virtual void OnPaint()
	{
		axGC* gc = GetGC();
		axRect rect(GetRect());
		axRect rect0(0, 0, rect.size.x, rect.size.y);

		gc->SetColor(axColor(1.0, 1.0, 1.0));
		gc->DrawRectangle(rect0);

		gc->SetColor(axColor(0.0, 0.0, 0.0));
		gc->DrawRectangleContour(axRect(1, 1, rect.size.x -1, rect.size.y -1));
	}

};


int main(int argc, char* argv[])
{
	axApp app(axSize(500, 500));

	axRange<double> range(stof(argv[2]), stof(argv[3]));
	//cout << "Range : " << range.left << " " << range.right << endl;
	Grapher* graph = new Grapher(&app, nullptr, 
								 axRect(0, 0, 500, 500), 
								 argv[1], range, stoi(argv[4]));

	app.MainLoop();
	return 0;
}

