/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	animal3D Math (A3DM) SDK
	By Daniel S. Buckstein

	a3stats.h
	Declarations for miscellaneous statistical functions.
*/

#ifndef __ANIMAL3D_A3DM_STATS_H
#define __ANIMAL3D_A3DM_STATS_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------
// median, mean, variance, standard deviation (with options to return mean)

// A3: Calculate median of real data set.
//	param data: array of data values
//	param n: number of elements
//	return: median value (half of all values are greater, half are less)
A3_INLINE a3real a3median(const a3real data[], const a3count n);

// A3: Calculate median of integer data set.
//	param data: array of data values
//	param n: number of elements
//	return: median value (half of all values are greater, half are less)
A3_INLINE a3real a3medianInt(const a3integer data[], const a3count n);


// A3: Calculate mean (average) of real data set.
//	param data: array of data values
//	param n: number of elements
//	return: mean value (average of all values)
A3_INLINE a3real a3mean(const a3real data[], const a3count n);

// A3: Calculate mean (average) of integer data set.
//	param data: array of data values
//	param n: number of elements
//	return: mean value (average of all values)
A3_INLINE a3real a3meanInt(const a3integer data[], const a3count n);


// A3: Calculate variance of real data set.
//	param data: array of data values
//	param n: number of elements
//	param mean_out: optional pointer to store mean calculated in function
//	return: variance (squared standard deviation)
A3_INLINE a3real a3variance(const a3real data[], const a3count n, a3real *mean_out);

// A3: Calculate variance of integer data set.
//	param data: array of data values
//	param n: number of elements
//	param mean_out: optional pointer to store mean calculated in function
//	return: variance (squared standard deviation)
A3_INLINE a3real a3varianceInt(const a3integer data[], const a3count n, a3real *mean_out);


// A3: Calculate standard deviation of real data set.
//	param data: array of data values
//	param n: number of elements
//	param mean_out: optional pointer to store mean calculated in function
//	return: standard deviation (average offset from mean)
A3_INLINE a3real a3standardDeviation(const a3real data[], const a3count n, a3real *mean_out);

// A3: Calculate standard deviation of integer data set.
//	param data: array of data values
//	param n: number of elements
//	param mean_out: optional pointer to store mean calculated in function
//	return: standard deviation (average offset from mean)
A3_INLINE a3real a3standardDeviationInt(const a3integer data[], const a3count n, a3real *mean_out);


// A3: Calculate factorial of number.
//	param n: number
//	return: factorial of n (n!)
A3_INLINE a3biginteger a3factorial(a3bigcount n);

// A3: Calculate permutations (number of ways to order k elements in a set of n items).
//	param n: set size
//	param k: number of elements to permute (order)
//	return: nPk (number of permutations of k elements in set of n items)
A3_INLINE a3biginteger a3permutations(const a3bigcount n, const a3bigindex k);

// A3: Calculate combinations (number of ways to choose k elements in a set of n items).
//	param n: set size
//	param k: number of elements to choose
//	return: nCk (number of combinations of k elements in set of n items)
A3_INLINE a3biginteger a3combinations(const a3bigcount n, const a3bigindex k);


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3stats_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_STATS_H