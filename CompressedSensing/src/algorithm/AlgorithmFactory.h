#pragma once

#include "src/algorithm/ICSAlgorithm.h"
#include "src/algorithm/TvqcAlgorithm.h"

namespace CS {
namespace algorithm {

class AlgorithmFactory {
public:
	static ICSAlgorithm* getInstance(std::string name);
};

}} //namespace end