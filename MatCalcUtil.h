#ifndef MAT_CALC_UTIL_H
#define MAT_CALC_UTIL_H

#include <cstddef> // Required by g++ (size_t)

/**
* Matrix Calculator Utility (not Marvel Cinematic Universe) to help with some operations regarding matrices.
*/
namespace mcu
{
	/**
	* The preferred epsilon value by the Matrix Calculator. It is equal to std::numeric_limits<double>::epsilon() * 1000. Meaning, it works for (11 zeroes + 1 non-zero) digits.
	*/
	extern const double EPSILON;
	/**
	* Checks whether left and right doubles are approximately equal, based on the epsilon value. NaN (Not a Number) cases are also handled. If both numbers are NaN, returns true; otherwise false.
	* @see mcu::EPSILON
	* @param left Left double.
	* @param right Right double.
	* @param epsilon Epsilon value to perform "approximate equal" operation. By default, it is equal to mcu::EPSILON.
	* @return True if the arguments are equal; false if not equal.
	*/
	bool doubleAlmostEqual(double left, double right, double epsilon = mcu::EPSILON);
	/**
	* Calculates the number of digits BEFORE the floating point. Helps with getPrintStr of MatrixBase instances.
	* @see MatrixBase::getPrintStr()
	* @param x Floating point value of double precision.
	* @param includeNegativeSign If true, then the negative sign will add 1 to the return value. If false, the return value will not be changed by this option.
	* @return The number of digits BEFORE the floating point of a double precision floating point number.
	*/
	size_t getNumDigitsOfIntegerPart(double x, bool includeNegativeSign);
}

#endif // MAT_CALC_UTIL_H
