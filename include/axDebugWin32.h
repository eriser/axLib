#ifndef __AX_DEBUG_WIN32__
#define __AX_DEBUG_WIN32__

#include <Windows.h> 
#include <ostream>
#include <sstream> 
#include <iomanip>
#include <string> 
#include <sstream> 
#include <fstream>
#include <assert.h>

using namespace std;

#define AXLIB_DEBUG_MODE (1)
#define COMMENT(C) C ##/

enum PRINT_COLOR_LIST
{
	COLOR_BLACK = 0,
	COLOR_DARK_BLUE,
	COLOR_DRAK_GREEN,
	COLOR_BLUEISH,
	COLOR_DARK_RED,
	COLOR_DRAK_PURPLE,
	COLOR_YELLOW,
	COLOR_PALE_GREY = 7,
	COLOR_DEFAULT = 7,
	COLOR_DARK_GREY,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_TURQ,
	COLOR_RED,
	COLOR_PURPLE
};

ostream& operator << (ostream& stream, const PRINT_COLOR_LIST& color);
ostream& operator << (const PRINT_COLOR_LIST& color, ostream& stream);

#define ENDL COLOR_DEFAULT << endl

#if AXLIB_DEBUG_MODE == 1

#define _DEBUG_ 

#define FCT_NAME __FUNCTION__
#define FCT_SIGNATURE __FUNCSIG__

// Assert function with assert line and messsage.
#define ASSERT(condition, message) \
if (!(condition)) {cerr << "Assert `" #condition "` failed in " \
	<< __FILE__ << " line " << __LINE__ << ": " << message << endl; \
	assert((condition));}


template <class CharT, class TraitsT = std::char_traits<CharT> >
class basic_debugbuf : public std::basic_stringbuf<CharT, TraitsT>
{
public:
	virtual ~basic_debugbuf() { sync(); }

protected:
	int sync()
	{
		output_debug_string(str().c_str());
		str(std::basic_string<CharT>());    // Clear the string buffer
		return 0;
	}

	void output_debug_string(const CharT *text) {}
};

//--------------------------------------------------------------------------------------------------
template<>
void basic_debugbuf<char>::output_debug_string(const char *text)
{
	::OutputDebugStringA(text);
}

template<>
void basic_debugbuf<wchar_t>::output_debug_string(const wchar_t *text)
{
	::OutputDebugStringW(text);
}
template<class CharT, class TraitsT = std::char_traits<CharT> >
//--------------------------------------------------------------------------------------------------

class basic_dostream : public std::basic_ostream<CharT, TraitsT>
{
public:
	basic_dostream() : std::basic_ostream<CharT, TraitsT>(new basic_debugbuf<CharT, TraitsT>()) {}
	~basic_dostream() { delete rdbuf(); }
};

typedef basic_dostream<char>    dostream;
typedef basic_dostream<wchar_t> wdostream;

// DSTREAM to write in windows debug console
__declspec(selectany) dostream DEBUG_STREAM;

#define DSTREAM _DEBUG_ DEBUG_STREAM 

//-----------------------------------------------------------------------------
#define DEBUG_OUTPUT cout << COLOR_DARK_RED << FCT_NAME << ENDL;
#define DEBUG _DEBUG_ DSTREAM << COLOR_DARK_RED << FCT_NAME << endl;
//-----------------------------------------------------------------------------

#else 
#define _DEBUG_ COMMENT(/)
#define DSTREAM COMMENT(/)
#define DEBUG COMMENT(/)
#endif

#endif // __AX_DEBUG_WIN32__