#ifndef _EXCEPTION_H
#define _EXCEPTION_H


#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#define throwError(arg) throw parseException((arg), __FILE__, __LINE__);

using namespace std;

class parseException : public runtime_error 
{

public:

	parseException(const string &s, const char *file, int line) :
	
	runtime_error(s) 
	{
		ostringstream out;
		out << file << ":" << line << ": " << s << endl;
		msg = out.str();
	}
	
	~parseException() throw() {}
	
	const char* what() const throw() 
	{
		return msg.c_str();	
	}

private:

	string msg;
};


#endif
