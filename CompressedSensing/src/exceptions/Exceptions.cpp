#include "src/exceptions/Exceptions.h"

using namespace std;
using namespace CS::exception;

const char* UnknownTypeException::what() const{
	msg.str("");
	msg << runtime_error::what() << ": not found or supported. \n";

	return msg.str().c_str();
}