#include "axLib.h"

enum CalcOperator
{
    CALC_ADD, CALC_MIN, CALC_MUL, CALC_DIV, CALC_POW, CALC_MODULO,
    CALC_SIGN, CALC_NUMBER, CALC_EQUAL, CALC_RESET
};

class CalculatorLogic
{
public:
    CalculatorLogic() : _value_left(0), _result(0), _string_value("0"),
    _operator(CALC_EQUAL)
    {
    }
    
    void SetOperation(const CalcOperator& operation)
    {
        switch (operation)
        {
            case CALC_RESET:
                _result = _value_left = 0;
                _string_value = "0";
                _operator = CALC_EQUAL;
                break;
            case CALC_SIGN:
                if (_string_value[0] == '-')
                    _string_value.erase(_string_value.begin());
                else _string_value.insert(0, "-"); break;
                
            case CALC_EQUAL: Equal(); break;
            default: TwoSideOperation(operation);
        }
    }
    
    double GetValue()
    {
        return stod(_string_value);
    }
    
    void AddNumber(const string& number)
    {
        if (number == ".")
        {
            // If not found.
            if (_string_value.find('.') == std::string::npos)
                _string_value += number;
        }
        else
            _string_value += number;
    }
    
private:
    string _string_value;
    double _value_left, _result;
    CalcOperator _operator;
    
    void TwoSideOperation(const CalcOperator& operation)
    {
        if (_operator != CALC_EQUAL) Equal();
        _value_left = stod(_string_value);
        _operator = operation;
        _string_value = "0";
    }
    
    void Equal()
    {
        double v = stod(_string_value);
        
        switch (_operator)
        {
            case CALC_RESET: _result = 0; break;
            case CALC_ADD: _result = _value_left + v; break;
            case CALC_MIN: _result = _value_left - v; break;
            case CALC_MUL: _result = _value_left * v; break;
            case CALC_POW: _result = pow(_value_left, v); break;
            case CALC_MODULO: _result = int(floor(_value_left)) % int(floor(v)); break;
            case CALC_DIV: v == 0 ? _result = 0 : _result = _value_left / v; break;
        }
        
        _operator = CALC_EQUAL;
        _string_value = to_string(_result);
    }
};

class Calculator: public axPanel
{
public:
    Calculator(axWindow* parent, const axRect& rect) :
    axPanel(parent, rect)
    {
        AddEventFunction("OnNumber", GetOnNumber());
        AddEventFunction("OnOperation", GetOnOperation());
        axObjectLoader loader(this, "Calculator.xml");
    }
    
    axEVENT_ACCESSOR(axButton::Msg, OnNumber);
    axEVENT_ACCESSOR(axButton::Msg, OnOperation);
    
private:
    CalculatorLogic _logic;
    
    // Events
    void OnNumber(const axButton::Msg& msg)
    {
        _logic.AddNumber(msg.GetMsg());
        axLabel* screen = GetResourceManager()->GetResource("screen");
        screen->SetLabel(to_string(_logic.GetValue()));
    }
    
    void OnOperation(const axButton::Msg& msg)
    {
        _logic.SetOperation((CalcOperator)stoi(msg.GetMsg()));
        axLabel* screen = GetResourceManager()->GetResource("screen");
        screen->SetLabel(to_string(_logic.GetValue()));
    }
};

void axMain::MainEntryPoint(axApp* app)
{
    Calculator* calc = new Calculator(nullptr, axRect(0, 0, 200, 255));
}
//---------------------------------------------------------------------------
//int main()
//{
//	axApp app(axSize(165, 255));
//	Calculator* calc = new Calculator(nullptr, axRect(0, 0, 250, 250));
//	
//	app.MainLoop();
//
//	return 0;
//}
