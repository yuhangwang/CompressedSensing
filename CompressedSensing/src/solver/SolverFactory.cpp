#include "src/solver/SolverFactory.h"
#include "src/exceptions/Exceptions.h"
#include "src/solver/GPUSolver.h"
#include "src/solver/CPUSolver.h"
//#include <algorithm>

using namespace CS::solver;

ISolver* SolverFactory::getInstance(const std::string& _type) {
	std::string type(_type);
	std::transform(type.begin(),type.end(), type.begin(), ::tolower);
	
	if(type == "gpu") {
		return new gpu::GPUSolver();
	}else if(type == "cpu") {
		return new cpu::CPUSolver();
	}else {
		throw CS::exception::UnknownTypeException(type);
	}
}