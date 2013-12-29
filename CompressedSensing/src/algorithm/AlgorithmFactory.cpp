#include "src/algorithm/AlgorithmFactory.h"
#include "src/exceptions/Exceptions.h"
#include <algorithm>

using namespace CS::algorithm;

ICSAlgorithm* AlgorithmFactory::getInstance(std::string name) {
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if(name == "tvqc") {
		return new TvqcAlgorithm();
	}else {
		throw CS::exception::UnknownTypeException(name);
	}
}