#ifndef _EXCEPTION_H
#define _EXCEPTION_H


#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#define throwError(arg) throw Exception((arg), __FILE__, __LINE__);

using namespace std;

class Exception : public runtime_error 
{

public:

	Exception(const string &s, const char *file, int line) :
	
	runtime_error(s) 
	{
		ostringstream out;
		out << file << ":" << line << ": " << s << endl;
		msg = out.str();
	}
	
	~Exception() throw() {}
	
	const char* what() const throw() 
	{
		perror("Func run: ");
		return msg.c_str();	
	}

private:

	string msg;
};


#endif
