#include "axEventManager.h"
#include "axEvent.h"
#include "axTimer.h"

class MyClass : public axObject
{
public: 
	MyClass(double g)
	{
	}
	    
	axEVENT(axTimerMsg, MyFct);
	axEVENT(axTimerMsg, MyFct2);
	
private:
	axEvent MyFct(axTimerMsg* h)
	{
		cout << "MyClass : " << GetID() << " " << h->GetStringMsg() << endl;
		cout << "Test timer : " << h->GetTime() << endl;
	}

	axEvent MyFct2(axTimerMsg* h)
	{
		cout << "MyClass(2) : " << GetID() << " " << h->GetStringMsg() << endl;
	}
};

int main(int argc, char* argv[])
{

	std::cout << "TEST" << std::endl;
	// Create event manager.
	axEventManager* evtManager = axEventManager::Instance();
	
	MyClass myclass1(2.2);
	MyClass myclass2(3.3);
	MyClass myclass3(4.4);

	axTimer timer1(200);
	timer1.AddConnection(myclass1.GetMyFct());
	timer1.AddConnection(myclass2.GetMyFct());
	timer1.AddConnection(myclass3.GetMyFct());

	while (1)
	{
		evtManager->CallNext();
	}

	//system("pause");
	return 0;
}