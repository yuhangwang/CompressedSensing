#ifndef SOLVER_FACTORY_H
#define SOLVER_FACTORY_H

#include <string>
#include "src/solver/ISolver.h"


namespace CS {
namespace solver {

class SolverFactory {
	
public:
	static ISolver* getInstance(const std::string&);
};

}}

#endif /*SOLVER_FACTORY_H*/