#include "src/exceptions/Exceptions.h"

using namespace std;
using namespace CS::exception;

ostringstream UnknownTypeException::msg;
ostringstream JaiCameraException::msg;

const char* UnknownTypeException::what() const{
	msg.str("");
	msg << runtime_error::what() << ": not found or supported. \n";

	return msg.str().c_str();
}

JaiCameraException::JaiCameraException(string _functionName, int _errorCode) {
	functionName = _functionName;
	errorCode = _errorCode;
}

const char* JaiCameraException::what() const{
	msg.str("");
	msg << "Error occured in " << functionName <<". Error code = "<<errorCode;
	return msg.str().c_str();
}