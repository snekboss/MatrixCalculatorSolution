#include "MatCalcUtil.h"
#include <algorithm>
#include <limits>
#include <cmath>

const double mcu::EPSILON = std::numeric_limits<double>::epsilon() * 1000;

bool mcu::doubleAlmostEqual(double left, double right, double epsilon)
{
	if (std::isnan(epsilon))
	{
		return false;
	}

	if (std::isnan(left) && std::isnan(right))
	{
		return true;
	}
	else if (std::isnan(left) || std::isnan(right))
	{
		return false;
	}

	// Source: http://realtimecollisiondetection.net/blog/?p=89

	// Default epsilon (DBL_EPSILON * 1000) works for: (11 zeroes + 1 non-zero) digits.

	double absoluteTolerance = epsilon;
	double relativeTolerance = epsilon * std::max(left, right);
	double absDiff = std::abs(left - right);

	if ((absDiff <= absoluteTolerance) || (absDiff <= relativeTolerance))
	{
		return true;
	}

	return false;
}

size_t mcu::getNumDigitsOfIntegerPart(double x, bool includeNegativeSign)
{
	// Calculates the number of digits before the floating point.
	size_t numDigits = 0;

	double absX = x;

	if (absX < 0) // TODO: Is there an epsilon problem for 'less than'? Maybe not when comparing to zero?
	{
		absX = -absX; // Making the number positive, so that the cast (below) doesn't overflow.
	}

	unsigned long long int intPart = (long long)absX;

	while (intPart != 0)
	{
		intPart /= 10;
		numDigits++;
	}

	if (numDigits == 0)
	{
		// 'x' is a number between -1 and 1 (-1 < x < 1). Every number has at least 1 digit.
		numDigits = 1;
	}

	if (includeNegativeSign && (x < 0.0)) // TODO: Is there an epsilon problem for 'less than'? Maybe not when comparing to zero?
	{
		// Checking for 'less than': For non-zero values, negative sign adds 1 to the length.
		// Ignore if the number is zero. (Negative zero is still zero).
		numDigits++;
	}

	return numDigits;
}
