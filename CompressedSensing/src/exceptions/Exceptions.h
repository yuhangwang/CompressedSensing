#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

namespace CS {
namespace exception {

class UnknownTypeException : public runtime_error {
public:
	UnknownTypeException(string type) : runtime_error(type.c_str()) {}

	virtual const char* what() const throw();
private:
	static ostringstream msg;
};

class JaiCameraException : public exception {
public:
	JaiCameraException(string msg, int errorCode);
	virtual const char* what() const throw();
private:
	map<int, string> fillMap();
	map<int, string> errorMessageMap;
	string functionName;
	int errorCode;
	static ostringstream msg;
};

}}

#endif /*EXCEPTIONS_H*/