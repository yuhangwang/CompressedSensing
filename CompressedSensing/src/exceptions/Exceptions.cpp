#include "src/exceptions/Exceptions.h"
#include <Jai_Factory.h>

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

	errorMessageMap = fillMap();
}

map<int, string> JaiCameraException::fillMap() {
	map<int, string> output;
	output[J_ST_INVALID_BUFFER_SIZE]	= "Invalid buffer size ";
	output[J_ST_INVALID_HANDLE]			= "Invalid handle ";
	output[J_ST_INVALID_ID]				="Invalid ID ";			                
	output[J_ST_ACCESS_DENIED]			="Access denied ";		               
	output[J_ST_NO_DATA]				="No data ";				           
	output[J_ST_ERROR]					="Generic error ";		                
	output[J_ST_INVALID_PARAMETER]		="Invalid parameter ";	                
	output[J_ST_TIMEOUT]				="Timeout ";				              
	output[J_ST_INVALID_FILENAME]		="Invalid file name ";	                
	output[J_ST_INVALID_ADDRESS]		="Invalid address ";		                
	output[J_ST_FILE_IO]				="File IO error ";		                
	output[J_ST_GC_ERROR]				="GenICam error ";		                
	output[J_ST_VALIDATION_ERROR]		="Settings File Validation Error ";		
	output[J_ST_VALIDATION_WARNING]		="Settings File Validation Warning ";    

	return output;
}

const char* JaiCameraException::what() const{
	msg.str("");
	map<int, string>::const_iterator it = errorMessageMap.find(errorCode);
	msg << "Error occured in " << functionName << " Error code = "<<errorCode << "message = "<< it->second << std::endl;
	return msg.str().c_str();
}