#ifndef _AX_MSG_
#define _AX_MSG_

#include <iostream>
#include <string>

using namespace std;

class axMsg
{
public:
	axMsg(string msg = "");
	axMsg(axMsg* msg)
	{
		cout << "axMsg::axMsg" << endl;
		//CopyFromParentPointer(msg);
	}

	virtual ~axMsg(){}

	string GetStringMsg() const;

	virtual axMsg* CopyChildFromParentPointer()
	{

		//cout << "axMsg::CopyFromParentPointer" << endl;
		//return new axTimerMsg(GetStringMsg(), GetTimer;
		return nullptr;
	}

protected:
	void SetStrigMsg(const string& msg)
	{
		_msg = msg;
	}

///private:
	string _msg;
};

#endif //_AX_MSG_