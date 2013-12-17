#include "src/utils/mathUtils.h"
#include <cstdlib>

void CS::math::MathUtils::randperm(int n, int* perm) {
	int i, j, t;

	for(i=0; i<n; i++)
		perm[i] = i;
	for(i=0; i<n; i++) {
		j = rand()%(n-i)+i;
		t = perm[j];
		perm[j] = perm[i];
		perm[i] = t;
	}
}

