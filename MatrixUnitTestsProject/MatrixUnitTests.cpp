#include "Matrix.h"
#include "MatCalcUtil.h"
#include <iostream>
#include <assert.h>

/**
* A static helper function to check whether two doubles are equal (deq = double (almost) equal). Uses mcu::doubleAlmostEqual to perform the check.
* @see mcu::doubleAlmostEqual
* @return True if equal, false if not equal.
*/
static bool deq(double left, double right)
{
	return mcu::doubleAlmostEqual(left, right);
}

/**
* A static helper function to check whether two strings are equal (streq = string equal).
* @return True if equal, false if not equal.
*/
static bool streq(const std::string& left, const std::string& right)
{
	if (left.length() != right.length())
	{
		return false;
	}

	size_t len = left.length();

	for (size_t i = 0; i < len; i++)
	{
		if (left[i] != right[i])
		{
			return false;
		}
	}

	return true;
}

/**
* A program with a single, long function (main) which contains unit tests for Matrix class.
*/
int main()
{
	/*
	Some of the examples were taken from BIE-LIN exercises.

	Gaussian Elimination examples are from:
	https://courses.fit.cvut.cz/BIE-LIN/topics/exercise/bie-lin-exercise-02.pdf

	Rank examples are from:
	https://courses.fit.cvut.cz/BIE-LIN/topics/exercise/bie-lin-exercise-04.pdf

	I think the rest are my own examples. In case I forgot some, here's the entire material page for BIE-LIN:
	https://courses.fit.cvut.cz/BIE-LIN/topics/index.html
	*/


	// getCell and createDense
	Matrix m1 = Matrix::createDense(2, 2, 1);
	assert(deq(m1.getCell(0, 0), 1));
	assert(deq(m1.getCell(0, 1), 1));
	assert(deq(m1.getCell(1, 0), 1));
	assert(deq(m1.getCell(1, 1), 1));

	// createIdentity
	Matrix m2 = Matrix::createIdentity(2);
	assert(deq(m2.getCell(0, 0), 1));
	assert(deq(m2.getCell(0, 1), 0));
	assert(deq(m2.getCell(1, 0), 0));
	assert(deq(m2.getCell(1, 1), 1));

	// setCell
	m2.setCell(0, 1, 1);
	m2.setCell(1, 0, 1);
	assert(deq(m2.getCell(0, 1), 1));
	assert(deq(m2.getCell(1, 0), 1));

	// operator==
	assert(m1 == m2);
	assert(m2 == m1);

	// operator!=
	assert(!(m1 != m2));
	assert(!(m2 != m1));

	// createSparse and getCell on SparseMatrix
	Matrix m3 = Matrix::createSparse(2, 2);
	assert(deq(m3.getCell(0, 0), 0));
	assert(deq(m3.getCell(0, 1), 0));
	assert(deq(m3.getCell(1, 0), 0));
	assert(deq(m3.getCell(1, 1), 0));

	// Dense != Sparse
	assert(m1 != m3);
	assert(m2 != m3);

	// Dense == Sparse
	m3.setCell(0, 0, 1);
	m3.setCell(0, 1, 1);
	m3.setCell(1, 0, 1);
	m3.setCell(1, 1, 1);
	assert(m1 == m3);
	assert(m2 == m3);

	// createZero
	Matrix m5 = Matrix::createZero(2, 2);
	Matrix m6 = Matrix::createZero(2, 2);

	// Sparse != Sparse
	m5.setCell(0, 0, 1);
	assert(m5 != m6);

	// Sparse == Sparse
	m6.setCell(0, 0, 1);
	assert(m5 == m6);

	// getNumRows and getNumColumns
	// Dense
	assert(m1.getNumRows() == 2);
	assert(m1.getNumColumns() == 2);

	// Sparse
	assert(m2.getNumRows() == 2);
	assert(m2.getNumColumns() == 2);

	// Copy constructor
	Matrix m7 = Matrix::createIdentity(2);
	Matrix m8 = m7;
	assert(m7 == m8);
	m7.setCell(0, 1, 1);
	assert(m7 != m8);
	m8.setCell(0, 1, 1);
	assert(m7 == m8);

	// Copy assignment
	Matrix m9 = Matrix::createDense(3, 3, 5);
	m8 = m9;
	assert(m8 == m9);
	assert(m8 != m7);

	// ****************************** Resizing ******************************
	// Resize Rows (Dense, decrease)
	m9.resizeNumRows(1);
	assert(m9.getNumRows() == 1);
	assert(m9.getNumColumns() == 3);
	assert(deq(m9.getCell(0, 0), 5));
	assert(deq(m9.getCell(0, 1), 5));
	assert(deq(m9.getCell(0, 2), 5));

	// Resize Rows (Dense, increase)
	m9.resizeNumRows(2);
	assert(m9.getNumRows() == 2);
	assert(m9.getNumColumns() == 3);
	assert(deq(m9.getCell(0, 0), 5));
	assert(deq(m9.getCell(0, 1), 5));
	assert(deq(m9.getCell(0, 2), 5));
	assert(deq(m9.getCell(1, 0), 0));
	assert(deq(m9.getCell(1, 1), 0));
	assert(deq(m9.getCell(1, 2), 0));

	// Resize Rows (Sparse, decrease)
	Matrix m10 = Matrix::createIdentity(3);
	m10.resizeNumRows(1);
	assert(m10.getNumRows() == 1);
	assert(m10.getNumColumns() == 3);
	assert(deq(m10.getCell(0, 0), 1));
	assert(deq(m10.getCell(0, 1), 0));
	assert(deq(m10.getCell(0, 2), 0));

	// Resize Rows (Sparse, increase)
	m10.resizeNumRows(2);
	assert(m10.getNumRows() == 2);
	assert(m10.getNumColumns() == 3);
	assert(deq(m10.getCell(0, 0), 1));
	assert(deq(m10.getCell(0, 1), 0));
	assert(deq(m10.getCell(0, 2), 0));
	assert(deq(m10.getCell(1, 0), 0));
	assert(deq(m10.getCell(1, 1), 0));
	assert(deq(m10.getCell(1, 2), 0));

	// Resize Columns (Dense, decrease)
	Matrix m11 = Matrix::createDense(2, 2, 3);
	m11.resizeNumColumns(1);
	assert(m11.getNumRows() == 2);
	assert(m11.getNumColumns() == 1);
	assert(deq(m11.getCell(0, 0), 3));
	assert(deq(m11.getCell(1, 0), 3));

	// Resize Columns (Dense, increase)
	m11.resizeNumColumns(2);
	assert(m11.getNumRows() == 2);
	assert(m11.getNumColumns() == 2);
	assert(deq(m11.getCell(0, 0), 3));
	assert(deq(m11.getCell(0, 1), 0));
	assert(deq(m11.getCell(1, 0), 3));
	assert(deq(m11.getCell(1, 1), 0));

	// Resize Columns (Sparse, decrease)
	Matrix m12 = Matrix::createZero(2, 2);
	m12.setCell(0, 0, 1);
	m12.resizeNumColumns(1);
	assert(m12.getNumRows() == 2);
	assert(m12.getNumColumns() == 1);
	assert(deq(m12.getCell(0, 0), 1));
	assert(deq(m12.getCell(1, 0), 0));

	// Resize Columns (Sparse, increase)
	m12.resizeNumColumns(2);
	assert(m12.getNumRows() == 2);
	assert(m12.getNumColumns() == 2);
	assert(deq(m12.getCell(0, 0), 1));
	assert(deq(m12.getCell(0, 1), 0));
	assert(deq(m12.getCell(1, 0), 0));
	assert(deq(m12.getCell(1, 1), 0));

	// Resize (both) (Dense) (increase, increase)
	Matrix m13 = Matrix::createDense(1, 1, 4);
	m13.resize(2, 3);
	assert(m13.getNumRows() == 2);
	assert(m13.getNumColumns() == 3);
	assert(deq(m13.getCell(0, 0), 4));
	assert(deq(m13.getCell(0, 1), 0));
	assert(deq(m13.getCell(0, 2), 0));
	assert(deq(m13.getCell(1, 0), 0));
	assert(deq(m13.getCell(1, 1), 0));
	assert(deq(m13.getCell(1, 2), 0));

	// Resize (both) (Dense) (decrease, decrease)
	m13.resize(1, 1);
	assert(m13.getNumRows() == 1);
	assert(m13.getNumColumns() == 1);
	assert(deq(m13.getCell(0, 0), 4));

	// Resize (both) (Sparse) (increase, increase)
	Matrix m14 = Matrix::createSparse(1, 1);
	m14.resize(2, 3);
	assert(m14.getNumRows() == 2);
	assert(m14.getNumColumns() == 3);
	assert(deq(m14.getCell(0, 0), 0));
	assert(deq(m14.getCell(0, 1), 0));
	assert(deq(m14.getCell(0, 2), 0));
	assert(deq(m14.getCell(1, 0), 0));
	assert(deq(m14.getCell(1, 1), 0));
	assert(deq(m14.getCell(1, 2), 0));

	// Resize (both) (Sparse) (decrease, decrease)
	m14.setCell(0, 1, 13);
	m14.resize(1, 2);
	assert(m14.getNumRows() == 1);
	assert(m14.getNumColumns() == 2);
	assert(deq(m14.getCell(0, 0), 0));
	assert(deq(m14.getCell(0, 1), 13));

	// Resize (both) (Dense) (increase, decrease)
	Matrix m15 = Matrix::createDense(1, 2, 19);
	m15.resize(2, 1);
	assert(m15.getNumRows() == 2);
	assert(m15.getNumColumns() == 1);
	assert(deq(m15.getCell(0, 0), 19));
	assert(deq(m15.getCell(1, 0), 0));

	// Resize (both) (Dense) (decrease, increase)
	m15.resize(1, 2);
	assert(m15.getNumRows() == 1);
	assert(m15.getNumColumns() == 2);
	assert(deq(m15.getCell(0, 0), 19));
	assert(deq(m15.getCell(0, 1), 0));

	// Resize (both) (Sparse) (increase, decrease)
	Matrix m16 = Matrix::createZero(1, 2);
	m16.setCell(0, 0, 18);
	m16.resize(2, 1);
	assert(m16.getNumRows() == 2);
	assert(m16.getNumColumns() == 1);
	assert(deq(m16.getCell(0, 0), 18));
	assert(deq(m16.getCell(1, 0), 0));

	// Resize (both) (Sparse) (decrease, increase)
	m16.resize(1, 2);
	assert(m16.getNumRows() == 1);
	assert(m16.getNumColumns() == 2);
	assert(deq(m16.getCell(0, 0), 18));
	assert(deq(m16.getCell(0, 1), 0));

	// Transpose (Dense)
	Matrix m17 = Matrix::createDense(2, 3, 0);
	m17.setCell(0, 0, 1);
	m17.setCell(0, 1, 2);
	m17.setCell(0, 2, 3);
	m17.setCell(1, 0, 4);
	m17.setCell(1, 1, 5);
	m17.setCell(1, 2, 6);

	assert(deq(m17.getCell(0, 0), 1));
	assert(deq(m17.getCell(0, 1), 2));
	assert(deq(m17.getCell(0, 2), 3));
	assert(deq(m17.getCell(1, 0), 4));
	assert(deq(m17.getCell(1, 1), 5));
	assert(deq(m17.getCell(1, 2), 6));

	m17.transpose();
	assert(m17.getNumRows() == 3);
	assert(m17.getNumColumns() == 2);
	assert(deq(m17.getCell(0, 0), 1));
	assert(deq(m17.getCell(0, 1), 4));
	assert(deq(m17.getCell(1, 0), 2));
	assert(deq(m17.getCell(1, 1), 5));
	assert(deq(m17.getCell(2, 0), 3));
	assert(deq(m17.getCell(2, 1), 6));

	// Transpose (Sparse)
	Matrix m18 = Matrix::createSparse(2, 3);
	m18.setCell(0, 1, 1);
	m18.setCell(0, 2, 2);
	m18.setCell(1, 1, 3);
	m18.setCell(1, 2, 4);

	assert(deq(m18.getCell(0, 0), 0));
	assert(deq(m18.getCell(0, 1), 1));
	assert(deq(m18.getCell(0, 2), 2));
	assert(deq(m18.getCell(1, 0), 0));
	assert(deq(m18.getCell(1, 1), 3));
	assert(deq(m18.getCell(1, 2), 4));

	m18.transpose();
	assert(m18.getNumRows() == 3);
	assert(m18.getNumColumns() == 2);
	assert(deq(m18.getCell(0, 0), 0));
	assert(deq(m18.getCell(0, 1), 0));
	assert(deq(m18.getCell(1, 0), 1));
	assert(deq(m18.getCell(1, 1), 3));
	assert(deq(m18.getCell(2, 0), 2));
	assert(deq(m18.getCell(2, 1), 4));

	// getDensity, getSparsity ; isDense, isSparse (DenseMatrix)
	Matrix m19 = Matrix::createDense(2, 2, 5);
	assert(deq(m19.getDensity(), 1));
	assert(deq(m19.getSparsity(), 0));
	assert(m19.isDense());
	assert(!(m19.isSparse()));

	m19.setCell(0, 0, 0);
	assert(deq(m19.getDensity(), 0.75));
	assert(deq(m19.getSparsity(), 0.25));
	assert(m19.isDense());
	assert(!(m19.isSparse()));

	m19.setCell(0, 1, 0);
	assert(deq(m19.getDensity(), 0.5));
	assert(deq(m19.getSparsity(), 0.5));
	assert(m19.isDense());
	assert(!(m19.isSparse()));

	m19.setCell(1, 0, 0);
	assert(deq(m19.getDensity(), 0.25));
	assert(deq(m19.getSparsity(), 0.75));
	assert(!(m19.isDense()));
	assert(m19.isSparse());

	m19.setCell(1, 1, 0);
	assert(deq(m19.getDensity(), 0));
	assert(deq(m19.getSparsity(), 1));
	assert(!(m19.isDense()));
	assert(m19.isSparse());

	// getDensity, getSparsity ; isDense, isSparse (SparseMatrix)
	Matrix m20 = Matrix::createSparse(2, 2);
	assert(deq(m20.getDensity(), 0));
	assert(deq(m20.getSparsity(), 1));
	assert(!(m20.isDense()));
	assert(m20.isSparse());

	m20.setCell(0, 0, 1);
	assert(deq(m20.getDensity(), 0.25));
	assert(deq(m20.getSparsity(), 0.75));
	assert(!(m20.isDense()));
	assert(m20.isSparse());

	m20.setCell(0, 1, 2);
	assert(deq(m20.getDensity(), 0.5));
	assert(deq(m20.getSparsity(), 0.5));
	assert(m20.isDense());
	assert(!(m20.isSparse()));

	m20.setCell(1, 0, 3);
	assert(deq(m20.getDensity(), 0.75));
	assert(deq(m20.getSparsity(), 0.25));
	assert(m20.isDense());
	assert(!(m20.isSparse()));

	m20.setCell(1, 1, 4);
	assert(deq(m20.getDensity(), 1));
	assert(deq(m20.getSparsity(), 0));
	assert(m20.isDense());
	assert(!(m20.isSparse()));

	// ****************************** Operators ******************************
	// operator+ (Dense, Dense)
	Matrix m21 = Matrix::createDense(2, 3, 1);
	Matrix m22 = Matrix::createDense(2, 3, 4);
	Matrix m21plusm22 = m21 + m22;
	assert(m21plusm22.getNumRows() == 2);
	assert(m21plusm22.getNumColumns() == 3);
	assert(deq(m21plusm22.getCell(0, 0), 5));
	assert(deq(m21plusm22.getCell(0, 1), 5));
	assert(deq(m21plusm22.getCell(0, 2), 5));
	assert(deq(m21plusm22.getCell(1, 0), 5));
	assert(deq(m21plusm22.getCell(1, 1), 5));
	assert(deq(m21plusm22.getCell(1, 2), 5));

	Matrix m22plusm21 = m22 + m21;
	assert(m22plusm21 == m21plusm22);

	// operator+ (Sparse, Dense)
	Matrix m23 = Matrix::createSparse(2, 3);
	Matrix m23plusm21 = m23 + m21;
	assert(m23plusm21.getNumRows() == 2);
	assert(m23plusm21.getNumColumns() == 3);
	assert(deq(m23plusm21.getCell(0, 0), 1));
	assert(deq(m23plusm21.getCell(0, 1), 1));
	assert(deq(m23plusm21.getCell(0, 2), 1));
	assert(deq(m23plusm21.getCell(1, 0), 1));
	assert(deq(m23plusm21.getCell(1, 1), 1));
	assert(deq(m23plusm21.getCell(1, 2), 1));

	// operator+ (Dense, Sparse)
	Matrix m21plusm23 = m21 + m23;
	assert(m21plusm23.getNumRows() == 2);
	assert(m21plusm23.getNumColumns() == 3);
	assert(deq(m21plusm23.getCell(0, 0), 1));
	assert(deq(m21plusm23.getCell(0, 1), 1));
	assert(deq(m21plusm23.getCell(0, 2), 1));
	assert(deq(m21plusm23.getCell(1, 0), 1));
	assert(deq(m21plusm23.getCell(1, 1), 1));
	assert(deq(m21plusm23.getCell(1, 2), 1));
	// operator+ (Sparse, Sparse)
	Matrix m24 = Matrix::createSparse(2, 3);
	m24.setCell(0, 0, 1);
	Matrix m23plusm24 = m23 + m24;
	assert(m23plusm24.getNumRows() == 2);
	assert(m23plusm24.getNumColumns() == 3);
	assert(deq(m23plusm24.getCell(0, 0), 1));
	assert(deq(m23plusm24.getCell(0, 1), 0));
	assert(deq(m23plusm24.getCell(0, 2), 0));
	assert(deq(m23plusm24.getCell(1, 0), 0));
	assert(deq(m23plusm24.getCell(1, 1), 0));
	assert(deq(m23plusm24.getCell(1, 2), 0));

	Matrix m24plusm23 = m24 + m23;
	assert(m24plusm23 == m23plusm24);

	// operator- (Dense, Dense)
	Matrix m25 = Matrix::createDense(3, 2, 5);
	Matrix m26 = Matrix::createDense(3, 2, 1);
	Matrix m25minusm26 = m25 - m26;
	assert(m25minusm26.getNumRows() == 3);
	assert(m25minusm26.getNumColumns() == 2);
	assert(deq(m25minusm26.getCell(0, 0), 4));
	assert(deq(m25minusm26.getCell(0, 1), 4));
	assert(deq(m25minusm26.getCell(1, 0), 4));
	assert(deq(m25minusm26.getCell(1, 1), 4));
	assert(deq(m25minusm26.getCell(2, 0), 4));
	assert(deq(m25minusm26.getCell(2, 1), 4));

	Matrix m26minusm25 = m26 - m25;
	assert(m26minusm25.getNumRows() == 3);
	assert(m26minusm25.getNumColumns() == 2);
	assert(deq(m26minusm25.getCell(0, 0), -4));
	assert(deq(m26minusm25.getCell(0, 1), -4));
	assert(deq(m26minusm25.getCell(1, 0), -4));
	assert(deq(m26minusm25.getCell(1, 1), -4));
	assert(deq(m26minusm25.getCell(2, 0), -4));
	assert(deq(m26minusm25.getCell(2, 1), -4));

	// operator- (Sparse, Dense)
	Matrix m27 = Matrix::createSparse(3, 2);
	Matrix m27minusm25 = m27 - m25;
	assert(m27minusm25.getNumRows() == 3);
	assert(m27minusm25.getNumColumns() == 2);
	assert(deq(m27minusm25.getCell(0, 0), -5));
	assert(deq(m27minusm25.getCell(0, 1), -5));
	assert(deq(m27minusm25.getCell(1, 0), -5));
	assert(deq(m27minusm25.getCell(1, 1), -5));
	assert(deq(m27minusm25.getCell(2, 0), -5));
	assert(deq(m27minusm25.getCell(2, 1), -5));
	// operator- (Dense, Sparse)
	Matrix m25minusm27 = m25 - m27;
	assert(m25minusm27.getNumRows() == 3);
	assert(m25minusm27.getNumColumns() == 2);
	assert(deq(m25minusm27.getCell(0, 0), 5));
	assert(deq(m25minusm27.getCell(0, 1), 5));
	assert(deq(m25minusm27.getCell(1, 0), 5));
	assert(deq(m25minusm27.getCell(1, 1), 5));
	assert(deq(m25minusm27.getCell(2, 0), 5));
	assert(deq(m25minusm27.getCell(2, 1), 5));
	// operator- (Sparse, Sparse)
	Matrix m28 = Matrix::createSparse(3, 2);
	m28.setCell(0, 0, 15);
	m28.setCell(0, 1, 15);
	Matrix m27minusm28 = m27 - m28;
	assert(m27minusm28.getNumRows() == 3);
	assert(m27minusm28.getNumColumns() == 2);
	assert(deq(m27minusm28.getCell(0, 0), -15));
	assert(deq(m27minusm28.getCell(0, 1), -15));
	assert(deq(m27minusm28.getCell(1, 0), 0));
	assert(deq(m27minusm28.getCell(1, 1), 0));
	assert(deq(m27minusm28.getCell(2, 0), 0));
	assert(deq(m27minusm28.getCell(2, 1), 0));

	Matrix m28minusm27 = m28 - m27;
	assert(m28minusm27.getNumRows() == 3);
	assert(m28minusm27.getNumColumns() == 2);
	assert(deq(m28minusm27.getCell(0, 0), 15));
	assert(deq(m28minusm27.getCell(0, 1), 15));
	assert(deq(m28minusm27.getCell(1, 0), 0));
	assert(deq(m28minusm27.getCell(1, 1), 0));
	assert(deq(m28minusm27.getCell(2, 0), 0));
	assert(deq(m28minusm27.getCell(2, 1), 0));

	// operator* (Dense, Dense)
	Matrix m29 = Matrix::createDense(2, 3, 0);
	m29.setCell(0, 0, 1);
	m29.setCell(0, 1, 2);
	m29.setCell(0, 2, 3);
	m29.setCell(1, 0, 4);
	m29.setCell(1, 1, 5);
	m29.setCell(1, 2, 6);

	Matrix m30 = Matrix::createDense(3, 2, 5);

	Matrix m29timesm30 = m29 * m30;
	assert(m29timesm30.getNumRows() == 2);
	assert(m29timesm30.getNumColumns() == 2);
	assert(deq(m29timesm30.getCell(0, 0), 30));
	assert(deq(m29timesm30.getCell(0, 1), 30));
	assert(deq(m29timesm30.getCell(1, 0), 75));
	assert(deq(m29timesm30.getCell(1, 1), 75));

	// operator* (Sparse, Dense)
	Matrix m31 = Matrix::createSparse(2, 3);
	m31.setCell(0, 0, 2);
	m31.setCell(0, 1, 2);
	m31.setCell(0, 2, 2);
	m31.setCell(1, 0, 2);
	m31.setCell(1, 1, 2);
	m31.setCell(1, 2, 2);

	Matrix m31timesm30 = m31 * m30;
	assert(m31timesm30.getNumRows() == 2);
	assert(m31timesm30.getNumColumns() == 2);
	assert(deq(m31timesm30.getCell(0, 0), 30));
	assert(deq(m31timesm30.getCell(0, 1), 30));
	assert(deq(m31timesm30.getCell(1, 0), 30));
	assert(deq(m31timesm30.getCell(1, 1), 30));
	// operator* (Dense, Sparse)
	Matrix m32 = Matrix::createSparse(3, 1);
	m32.setCell(0, 0, 2);
	m32.setCell(1, 0, 4);
	m32.setCell(2, 0, 6);

	Matrix m29timesm32 = m29 * m32;
	assert(m29timesm32.getNumRows() == 2);
	assert(m29timesm32.getNumColumns() == 1);
	assert(deq(m29timesm32.getCell(0, 0), 28));
	assert(deq(m29timesm32.getCell(1, 0), 64));

	// operator* (Sparse, Sparse)
	Matrix m33 = Matrix::createSparse(1, 2);
	m33.setCell(0, 0, 8);
	m33.setCell(0, 1, 10);

	Matrix m32timesm33 = m32 * m33;
	assert(m32timesm33.getNumRows() == 3);
	assert(m32timesm33.getNumColumns() == 2);
	assert(deq(m32timesm33.getCell(0, 0), 16));
	assert(deq(m32timesm33.getCell(0, 1), 20));
	assert(deq(m32timesm33.getCell(1, 0), 32));
	assert(deq(m32timesm33.getCell(1, 1), 40));
	assert(deq(m32timesm33.getCell(2, 0), 48));
	assert(deq(m32timesm33.getCell(2, 1), 60));

	// operator* (double, Dense)
	Matrix m34 = Matrix::createDense(2, 3, 0);
	m34.setCell(0, 0, 1);
	m34.setCell(0, 1, 2);
	m34.setCell(0, 2, 3);
	m34.setCell(1, 0, 4);
	m34.setCell(1, 1, 5);
	m34.setCell(1, 2, 6);
	Matrix m34times8 = m34 * 8;
	assert(m34times8.getNumRows() == 2);
	assert(m34times8.getNumColumns() == 3);
	assert(deq(m34times8.getCell(0, 0), 8));
	assert(deq(m34times8.getCell(0, 1), 16));
	assert(deq(m34times8.getCell(0, 2), 24));
	assert(deq(m34times8.getCell(1, 0), 32));
	assert(deq(m34times8.getCell(1, 1), 40));
	assert(deq(m34times8.getCell(1, 2), 48));

	// operator* (Dense, double)
	Matrix number8timesm34 = 8 * m34;
	assert(m34times8 == number8timesm34);
	// operator* (double, Sparse)
	Matrix m35 = Matrix::createIdentity(3);
	Matrix m35timesMinus5 = m35 * -5;
	assert(m35timesMinus5.getNumRows() == 3);
	assert(m35timesMinus5.getNumColumns() == 3);
	assert(deq(m35timesMinus5.getCell(0, 0), -5));
	assert(deq(m35timesMinus5.getCell(0, 1), 0));
	assert(deq(m35timesMinus5.getCell(0, 2), 0));
	assert(deq(m35timesMinus5.getCell(1, 0), 0));
	assert(deq(m35timesMinus5.getCell(1, 1), -5));
	assert(deq(m35timesMinus5.getCell(1, 2), 0));
	assert(deq(m35timesMinus5.getCell(2, 0), 0));
	assert(deq(m35timesMinus5.getCell(2, 1), 0));
	assert(deq(m35timesMinus5.getCell(2, 2), -5));

	// operator* (Sparse, double)
	Matrix minus5timesm35 = -5 * m35;
	assert(minus5timesm35 == m35timesMinus5);

	// ****************************** Merging ******************************
	// mergeByColumns (Dense, Dense)
	Matrix m36 = Matrix::createDense(2, 2, 0);
	m36.setCell(0, 0, 1);
	m36.setCell(0, 1, 2);
	m36.setCell(1, 0, 3);
	m36.setCell(1, 1, 4);

	Matrix m37 = Matrix::createDense(2, 2, 0);
	m37.setCell(0, 0, 5);
	m37.setCell(0, 1, 6);
	m37.setCell(1, 0, 7);
	m37.setCell(1, 1, 8);

	Matrix m36mergem37 = m36.mergeByColumns(m37);
	assert(m36mergem37.getNumRows() == 2);
	assert(m36mergem37.getNumColumns() == 4);
	assert(deq(m36mergem37.getCell(0, 0), 1));
	assert(deq(m36mergem37.getCell(0, 1), 2));
	assert(deq(m36mergem37.getCell(0, 2), 5));
	assert(deq(m36mergem37.getCell(0, 3), 6));
	assert(deq(m36mergem37.getCell(1, 0), 3));
	assert(deq(m36mergem37.getCell(1, 1), 4));
	assert(deq(m36mergem37.getCell(1, 2), 7));
	assert(deq(m36mergem37.getCell(1, 3), 8));

	Matrix m37mergem36 = m37.mergeByColumns(m36);
	assert(m37mergem36.getNumRows() == 2);
	assert(m37mergem36.getNumColumns() == 4);
	assert(deq(m37mergem36.getCell(0, 0), 5));
	assert(deq(m37mergem36.getCell(0, 1), 6));
	assert(deq(m37mergem36.getCell(0, 2), 1));
	assert(deq(m37mergem36.getCell(0, 3), 2));
	assert(deq(m37mergem36.getCell(1, 0), 7));
	assert(deq(m37mergem36.getCell(1, 1), 8));
	assert(deq(m37mergem36.getCell(1, 2), 3));
	assert(deq(m37mergem36.getCell(1, 3), 4));

	// mergeByColumns (Sparse, Dense)
	Matrix m38 = Matrix::createSparse(2, 1);
	Matrix m38mergem36 = m38.mergeByColumns(m36);
	assert(m38mergem36.getNumRows() == 2);
	assert(m38mergem36.getNumColumns() == 3);
	assert(deq(m38mergem36.getCell(0, 0), 0));
	assert(deq(m38mergem36.getCell(0, 1), 1));
	assert(deq(m38mergem36.getCell(0, 2), 2));
	assert(deq(m38mergem36.getCell(1, 0), 0));
	assert(deq(m38mergem36.getCell(1, 1), 3));
	assert(deq(m38mergem36.getCell(1, 2), 4));

	// mergeByColumns (Dense, Sparse)
	Matrix m36mergem38 = m36.mergeByColumns(m38);
	assert(m36mergem38.getNumRows() == 2);
	assert(m36mergem38.getNumColumns() == 3);
	assert(deq(m36mergem38.getCell(0, 0), 1));
	assert(deq(m36mergem38.getCell(0, 1), 2));
	assert(deq(m36mergem38.getCell(0, 2), 0));
	assert(deq(m36mergem38.getCell(1, 0), 3));
	assert(deq(m36mergem38.getCell(1, 1), 4));
	assert(deq(m36mergem38.getCell(1, 2), 0));

	// mergeByColumns (Sparse, Sparse)
	Matrix m39 = Matrix::createSparse(2, 3);
	m39.setCell(0, 0, 1);
	m39.setCell(0, 1, 1);

	Matrix m39mergem38 = m39.mergeByColumns(m38);
	assert(m39mergem38.getNumRows() == 2);
	assert(m39mergem38.getNumColumns() == 4);
	assert(deq(m39mergem38.getCell(0, 0), 1));
	assert(deq(m39mergem38.getCell(0, 1), 1));
	assert(deq(m39mergem38.getCell(0, 2), 0));
	assert(deq(m39mergem38.getCell(0, 3), 0));
	assert(deq(m39mergem38.getCell(1, 0), 0));
	assert(deq(m39mergem38.getCell(1, 1), 0));
	assert(deq(m39mergem38.getCell(1, 2), 0));
	assert(deq(m39mergem38.getCell(1, 3), 0));

	Matrix m38mergem39 = m38.mergeByColumns(m39);
	assert(m38mergem39.getNumRows() == 2);
	assert(m38mergem39.getNumColumns() == 4);
	assert(deq(m38mergem39.getCell(0, 0), 0));
	assert(deq(m38mergem39.getCell(0, 1), 1));
	assert(deq(m38mergem39.getCell(0, 2), 1));
	assert(deq(m38mergem39.getCell(0, 3), 0));
	assert(deq(m38mergem39.getCell(1, 0), 0));
	assert(deq(m38mergem39.getCell(1, 1), 0));
	assert(deq(m38mergem39.getCell(1, 2), 0));
	assert(deq(m38mergem39.getCell(1, 3), 0));

	// mergeByRows (Dense, Dense)
	Matrix m40 = Matrix::createDense(2, 1, 0);
	m40.setCell(0, 0, 1);
	m40.setCell(1, 0, 2);

	Matrix m41 = Matrix::createDense(5, 1, 0);
	m41.setCell(0, 0, 3);
	m41.setCell(1, 0, 4);
	m41.setCell(2, 0, 5);
	m41.setCell(3, 0, 6);
	m41.setCell(4, 0, 7);

	Matrix m40mergem41 = m40.mergeByRows(m41);
	assert(m40mergem41.getNumRows() == 7);
	assert(m40mergem41.getNumColumns() == 1);
	assert(deq(m40mergem41.getCell(0, 0), 1));
	assert(deq(m40mergem41.getCell(1, 0), 2));
	assert(deq(m40mergem41.getCell(2, 0), 3));
	assert(deq(m40mergem41.getCell(3, 0), 4));
	assert(deq(m40mergem41.getCell(4, 0), 5));
	assert(deq(m40mergem41.getCell(5, 0), 6));
	assert(deq(m40mergem41.getCell(6, 0), 7));

	Matrix m41mergem40 = m41.mergeByRows(m40);
	assert(m41mergem40.getNumRows() == 7);
	assert(m41mergem40.getNumColumns() == 1);
	assert(deq(m41mergem40.getCell(0, 0), 3));
	assert(deq(m41mergem40.getCell(1, 0), 4));
	assert(deq(m41mergem40.getCell(2, 0), 5));
	assert(deq(m41mergem40.getCell(3, 0), 6));
	assert(deq(m41mergem40.getCell(4, 0), 7));
	assert(deq(m41mergem40.getCell(5, 0), 1));
	assert(deq(m41mergem40.getCell(6, 0), 2));

	// mergeByRows (Sparse, Dense)
	Matrix m42 = Matrix::createSparse(3, 1);
	m42.setCell(0, 0, 333);

	Matrix m42mergem40 = m42.mergeByRows(m40);
	assert(m42mergem40.getNumRows() == 5);
	assert(m42mergem40.getNumColumns() == 1);
	assert(deq(m42mergem40.getCell(0, 0), 333));
	assert(deq(m42mergem40.getCell(1, 0), 0));
	assert(deq(m42mergem40.getCell(2, 0), 0));
	assert(deq(m42mergem40.getCell(3, 0), 1));
	assert(deq(m42mergem40.getCell(4, 0), 2));

	// mergeByRows (Dense, Sparse)
	Matrix m40mergem42 = m40.mergeByRows(m42);
	assert(m40mergem42.getNumRows() == 5);
	assert(m40mergem42.getNumColumns() == 1);
	assert(deq(m40mergem42.getCell(0, 0), 1));
	assert(deq(m40mergem42.getCell(1, 0), 2));
	assert(deq(m40mergem42.getCell(2, 0), 333));
	assert(deq(m40mergem42.getCell(3, 0), 0));
	assert(deq(m40mergem42.getCell(4, 0), 0));

	// mergeByRows (Sparse, Sparse)
	Matrix m43 = Matrix::createSparse(4, 1);
	m43.setCell(0, 0, 111);
	m43.setCell(3, 0, 444);

	Matrix m42mergem43 = m42.mergeByRows(m43);
	assert(m42mergem43.getNumRows() == 7);
	assert(m42mergem43.getNumColumns() == 1);
	assert(deq(m42mergem43.getCell(0, 0), 333));
	assert(deq(m42mergem43.getCell(1, 0), 0));
	assert(deq(m42mergem43.getCell(2, 0), 0));
	assert(deq(m42mergem43.getCell(3, 0), 111));
	assert(deq(m42mergem43.getCell(4, 0), 0));
	assert(deq(m42mergem43.getCell(5, 0), 0));
	assert(deq(m42mergem43.getCell(6, 0), 444));

	Matrix m43mergem42 = m43.mergeByRows(m42);
	assert(m43mergem42.getNumRows() == 7);
	assert(m43mergem42.getNumColumns() == 1);
	assert(deq(m43mergem42.getCell(0, 0), 111));
	assert(deq(m43mergem42.getCell(1, 0), 0));
	assert(deq(m43mergem42.getCell(2, 0), 0));
	assert(deq(m43mergem42.getCell(3, 0), 444));
	assert(deq(m43mergem42.getCell(4, 0), 333));
	assert(deq(m43mergem42.getCell(5, 0), 0));
	assert(deq(m43mergem42.getCell(6, 0), 0));

	// ****************************** Splitting ******************************

	// splitByColumn (Dense, get left matrix)
	Matrix m44 = Matrix::createDense(2, 2, 0);
	m44.setCell(0, 0, 1);
	m44.setCell(0, 1, 2);
	m44.setCell(1, 0, 3);
	m44.setCell(1, 1, 4);

	Matrix m44L = m44.splitByColumn(1, true);
	assert(m44L.getNumRows() == 2);
	assert(m44L.getNumColumns() == 1);
	assert(deq(m44L.getCell(0, 0), 1));
	assert(deq(m44L.getCell(1, 0), 3));

	Matrix m44L2 = m44.splitByColumn(2, true);
	assert(m44L2 == m44);

	// splitByColumn (Dense, get right matrix)
	Matrix m44R = m44.splitByColumn(1, false);
	assert(m44R.getNumRows() == 2);
	assert(m44R.getNumColumns() == 1);
	assert(deq(m44R.getCell(0, 0), 2));
	assert(deq(m44R.getCell(1, 0), 4));

	Matrix m44R2 = m44.splitByColumn(0, false);
	assert(m44R2 == m44);

	// splitByColumn (Sparse, get left matrix)
	Matrix m45 = Matrix::createSparse(2, 5);
	m45.setCell(0, 0, 0);
	m45.setCell(0, 1, 1);
	m45.setCell(0, 2, 0);
	m45.setCell(0, 3, 2);
	m45.setCell(0, 4, 0);
	m45.setCell(1, 0, 3);
	m45.setCell(1, 1, 0);
	m45.setCell(1, 2, 4);
	m45.setCell(1, 3, 0);
	m45.setCell(1, 4, 5);

	Matrix m45L = m45.splitByColumn(3, true);
	assert(m45L.getNumRows() == 2);
	assert(m45L.getNumColumns() == 3);
	assert(deq(m45L.getCell(0, 0), 0));
	assert(deq(m45L.getCell(0, 1), 1));
	assert(deq(m45L.getCell(0, 2), 0));
	assert(deq(m45L.getCell(1, 0), 3));
	assert(deq(m45L.getCell(1, 1), 0));
	assert(deq(m45L.getCell(1, 2), 4));

	Matrix m45L2 = m45.splitByColumn(5, true);
	assert(m45L2 == m45);

	// splitByColumn (Sparse, get right matrix)
	Matrix m45R = m45.splitByColumn(3, false);
	assert(m45R.getNumRows() == 2);
	assert(m45R.getNumColumns() == 2);
	assert(deq(m45R.getCell(0, 0), 2));
	assert(deq(m45R.getCell(0, 1), 0));
	assert(deq(m45R.getCell(1, 0), 0));
	assert(deq(m45R.getCell(1, 1), 5));

	Matrix m45R2 = m45.splitByColumn(0, false);
	assert(m45R2 == m45);

	// splitByRow (Dense, get top matrix)
	Matrix m46 = Matrix::createDense(5, 2, 0);
	m46.setCell(0, 0, 1);
	m46.setCell(1, 1, 2);
	m46.setCell(2, 0, 3);
	m46.setCell(3, 1, 4);
	m46.setCell(4, 0, 5);

	Matrix m46T = m46.splitByRow(3, true);
	assert(m46T.getNumRows() == 3);
	assert(m46T.getNumColumns() == 2);
	assert(deq(m46T.getCell(0, 0), 1));
	assert(deq(m46T.getCell(0, 1), 0));
	assert(deq(m46T.getCell(1, 0), 0));
	assert(deq(m46T.getCell(1, 1), 2));
	assert(deq(m46T.getCell(2, 0), 3));
	assert(deq(m46T.getCell(2, 1), 0));

	Matrix m46T2 = m46.splitByRow(5, true);
	assert(m46T2 == m46);

	// splitByRow (Dense, get bottom matrix)
	Matrix m46B = m46.splitByRow(3, false);
	assert(m46B.getNumRows() == 2);
	assert(m46B.getNumColumns() == 2);
	assert(deq(m46B.getCell(0, 0), 0));
	assert(deq(m46B.getCell(0, 1), 4));
	assert(deq(m46B.getCell(1, 0), 5));
	assert(deq(m46B.getCell(1, 1), 0));

	Matrix m46B2 = m46.splitByRow(0, false);
	assert(m46B2 == m46);

	// splitByRow (Sparse, get top matrix)
	Matrix m47 = Matrix::createSparse(5, 2);
	m47.setCell(0, 0, 1);
	m47.setCell(0, 1, 2);
	m47.setCell(4, 0, 3);
	m47.setCell(4, 1, 4);

	Matrix m47T = m47.splitByRow(2, true);
	assert(m47T.getNumRows() == 2);
	assert(m47T.getNumColumns() == 2);
	assert(deq(m47T.getCell(0, 0), 1));
	assert(deq(m47T.getCell(0, 1), 2));
	assert(deq(m47T.getCell(1, 0), 0));
	assert(deq(m47T.getCell(1, 1), 0));

	Matrix m47T2 = m47.splitByRow(5, true);
	assert(m47T2 == m47);

	// splitByRow (Sparse, get bottom matrix)
	Matrix m47B = m47.splitByRow(2, false);
	assert(m47B.getNumRows() == 3);
	assert(m47B.getNumColumns() == 2);
	assert(deq(m47B.getCell(0, 0), 0));
	assert(deq(m47B.getCell(0, 1), 0));
	assert(deq(m47B.getCell(1, 0), 0));
	assert(deq(m47B.getCell(1, 1), 0));
	assert(deq(m47B.getCell(2, 0), 3));
	assert(deq(m47B.getCell(2, 1), 4));

	Matrix m47B2 = m47.splitByRow(0, false);
	assert(m47B2 == m47);

	// ****************************** Sub Matrix ******************************
	// These won't be used in the program, but I still need to test the functionality.

	// getSubMatrix (Dense, valid)
	Matrix m48 = Matrix::createDense(4, 4, 0);
	m48.setCell(0, 0, 1);
	m48.setCell(0, 1, 2);
	m48.setCell(0, 2, 3);
	m48.setCell(0, 3, 4);
	m48.setCell(1, 0, 5);
	m48.setCell(1, 1, 6);
	m48.setCell(1, 2, 7);
	m48.setCell(1, 3, 8);
	m48.setCell(2, 0, 9);
	m48.setCell(2, 1, 10);
	m48.setCell(2, 2, 11);
	m48.setCell(2, 3, 12);
	m48.setCell(3, 0, 13);
	m48.setCell(3, 1, 14);
	m48.setCell(3, 2, 15);
	m48.setCell(3, 3, 16);

	// Middle.
	Matrix m48s1 = m48.getSubMatrix(1, 2, 1, 2);
	assert(m48s1.getNumRows() == 2);
	assert(m48s1.getNumColumns() == 2);
	assert(deq(m48s1.getCell(0, 0), 6));
	assert(deq(m48s1.getCell(0, 1), 7));
	assert(deq(m48s1.getCell(1, 0), 10));
	assert(deq(m48s1.getCell(1, 1), 11));

	// Original.
	Matrix m48s2 = m48.getSubMatrix(0, 4, 0, 4);
	assert(m48s2 == m48);

	// Left column.
	Matrix m48s3 = m48.getSubMatrix(0, 4, 0, 1);
	assert(m48s3.getNumRows() == 4);
	assert(m48s3.getNumColumns() == 1);
	assert(deq(m48s3.getCell(0, 0), 1));
	assert(deq(m48s3.getCell(1, 0), 5));
	assert(deq(m48s3.getCell(2, 0), 9));
	assert(deq(m48s3.getCell(3, 0), 13));

	// Top row.
	Matrix m48s4 = m48.getSubMatrix(0, 1, 0, 4);
	assert(m48s4.getNumRows() == 1);
	assert(m48s4.getNumColumns() == 4);
	assert(deq(m48s4.getCell(0, 0), 1));
	assert(deq(m48s4.getCell(0, 1), 2));
	assert(deq(m48s4.getCell(0, 2), 3));
	assert(deq(m48s4.getCell(0, 3), 4));

	// getSubMatrix (Dense, invalid)
	Matrix m48s5i = m48.getSubMatrix(1234, 0, 4321, 1111);
	assert(m48s5i.getNumRows() == 0);
	assert(m48s5i.getNumColumns() == 0);


	// getSubMatrix (Sparse, valid)
	Matrix m49 = Matrix::createSparse(4, 4);
	m49.setCell(0, 0, 1);
	m49.setCell(1, 1, 2);
	m49.setCell(2, 2, 3);
	m49.setCell(3, 3, 4);

	// Middle.
	Matrix m49s = m49.getSubMatrix(1, 2, 1, 2);
	assert(m49s.getNumRows() == 2);
	assert(m49s.getNumColumns() == 2);
	assert(deq(m49s.getCell(0, 0), 2));
	assert(deq(m49s.getCell(0, 1), 0));
	assert(deq(m49s.getCell(1, 0), 0));
	assert(deq(m49s.getCell(1, 1), 3));

	// Original.
	Matrix m49s2 = m49.getSubMatrix(0, 4, 0, 4);
	assert(m49s2 == m49);

	// Left column.
	Matrix m49s3 = m49.getSubMatrix(0, 4, 0, 1);
	assert(m49s3.getNumRows() == 4);
	assert(m49s3.getNumColumns() == 1);
	assert(deq(m49s3.getCell(0, 0), 1));
	assert(deq(m49s3.getCell(0, 1), 0));
	assert(deq(m49s3.getCell(0, 2), 0));
	assert(deq(m49s3.getCell(0, 3), 0));

	// Top row.
	Matrix m49s4 = m49.getSubMatrix(0, 1, 0, 4);
	assert(m49s4.getNumRows() == 1);
	assert(m49s4.getNumColumns() == 4);
	assert(deq(m49s4.getCell(0, 0), 1));
	assert(deq(m49s4.getCell(0, 1), 0));
	assert(deq(m49s4.getCell(0, 2), 0));
	assert(deq(m49s4.getCell(0, 3), 0));

	// getSubMatrix (Sparse, invalid)
	Matrix m49s5i = m49.getSubMatrix(1234, 0, 4321, 1111);
	assert(m49s5i.getNumRows() == 0);
	assert(m49s5i.getNumColumns() == 0);

	// ****************************** Determinant ******************************
	// Dense (1x1) zero matrix.
	Matrix denseZero1x1 = Matrix::createDense(1, 1, 0);
	assert(deq(denseZero1x1.getDeterminant(), 0));

	// Dense (1x1)
	Matrix m50 = Matrix::createDense(1, 1, 10);
	assert(deq(m50.getDeterminant(), 10));

	// Dense (2x2)
	Matrix m51 = Matrix::createDense(2, 2, 0);
	m51.setCell(0, 0, 1);
	m51.setCell(0, 1, 2);
	m51.setCell(1, 0, 3);
	m51.setCell(1, 1, 4);

	assert(deq(m51.getDeterminant(), -2));

	// Dense (3x3)
	Matrix m52 = Matrix::createDense(3, 3, 0);
	m52.setCell(0, 0, 12);
	m52.setCell(0, 1, 1);
	m52.setCell(0, 2, 2);
	m52.setCell(1, 0, 3);
	m52.setCell(1, 1, 4);
	m52.setCell(1, 2, 5);
	m52.setCell(2, 0, 6);
	m52.setCell(2, 1, 7);
	m52.setCell(2, 2, 8);

	assert(deq(m52.getDeterminant(), -36));

	// Dense (4x4)
	Matrix m53 = Matrix::createDense(4, 4, 0);
	m53.setCell(0, 0, 1);
	m53.setCell(0, 1, 2);
	m53.setCell(0, 2, 3);
	m53.setCell(0, 3, 4);
	m53.setCell(1, 0, 12);
	m53.setCell(1, 1, 55);
	m53.setCell(1, 2, 55);
	m53.setCell(1, 3, 5);
	m53.setCell(2, 0, 11);
	m53.setCell(2, 1, 55);
	m53.setCell(2, 2, 55);
	m53.setCell(2, 3, 6);
	m53.setCell(3, 0, 10);
	m53.setCell(3, 1, 9);
	m53.setCell(3, 2, 8);
	m53.setCell(3, 3, 7);

	assert(deq(m53.getDeterminant(), 1023));

	// Sparse (1x1) zero matrix.
	Matrix sparseZero1x1 = Matrix::createSparse(1, 1);
	assert(deq(sparseZero1x1.getDeterminant(), 0));

	// Sparse (1x1)
	Matrix m54 = Matrix::createSparse(1, 1);
	m54.setCell(0, 0, 19);

	assert(deq(m54.getDeterminant(), 19));

	// Sparse (2x2)
	Matrix m55 = Matrix::createSparse(2, 2);
	m55.setCell(0, 0, 10);
	m55.setCell(1, 1, 12);

	assert(deq(m55.getDeterminant(), 120));

	// Sparse (3x3)
	Matrix m56 = Matrix::createSparse(3, 3);
	m56.setCell(0, 1, 12);
	m56.setCell(1, 0, 1);
	m56.setCell(1, 2, 2);
	m56.setCell(2, 0, 3);
	m56.setCell(2, 2, 4);

	assert(deq(m56.getDeterminant(), 24));

	// Sparse (4x4)
	Matrix m57 = Matrix::createSparse(4, 4);
	m57.setCell(0, 1, 55);
	m57.setCell(0, 3, 55);
	m57.setCell(1, 1, 44);
	m57.setCell(1, 2, 44);
	m57.setCell(2, 0, 1);
	m57.setCell(2, 1, 5);
	m57.setCell(2, 2, 2);
	m57.setCell(2, 3, 6);
	m57.setCell(3, 0, 3);
	m57.setCell(3, 1, 7);
	m57.setCell(3, 2, 4);
	m57.setCell(3, 3, 8);

	assert(deq(m57.getDeterminant(), -9680));

	// ****************************** Overloaded Sub Matrix (ignoredRowIndex, ignoredColumnIndex) ******************************
	// These won't be used in the program, but I still need to test the functionality.

	// Dense, ignore some coordinates
	Matrix m58 = Matrix::createDense(3, 3, 0);
	m58.setCell(0, 0, 1);
	m58.setCell(0, 1, 2);
	m58.setCell(0, 2, 3);
	m58.setCell(1, 0, 4);
	m58.setCell(1, 1, 5);
	m58.setCell(1, 2, 6);
	m58.setCell(2, 0, 7);
	m58.setCell(2, 1, 8);
	m58.setCell(2, 2, 9);

	Matrix m58s1 = m58.getSubMatrix(2, 1);
	assert(m58s1.getNumRows() == 2);
	assert(m58s1.getNumColumns() == 2);
	assert(deq(m58s1.getCell(0, 0), 1));
	assert(deq(m58s1.getCell(0, 1), 3));
	assert(deq(m58s1.getCell(1, 0), 4));
	assert(deq(m58s1.getCell(1, 1), 6));

	// Dense, ignore topLeft
	Matrix m58s2 = m58.getSubMatrix(0, 0);
	assert(m58s2.getNumRows() == 2);
	assert(m58s2.getNumColumns() == 2);
	assert(deq(m58s2.getCell(0, 0), 5));
	assert(deq(m58s2.getCell(0, 1), 6));
	assert(deq(m58s2.getCell(1, 0), 8));
	assert(deq(m58s2.getCell(1, 1), 9));

	// Dense, ignore topRight
	Matrix m58s3 = m58.getSubMatrix(0, 2);
	assert(m58s3.getNumRows() == 2);
	assert(m58s3.getNumColumns() == 2);
	assert(deq(m58s3.getCell(0, 0), 4));
	assert(deq(m58s3.getCell(0, 1), 5));
	assert(deq(m58s3.getCell(1, 0), 7));
	assert(deq(m58s3.getCell(1, 1), 8));

	// Dense, ignore bottomLeft
	Matrix m58s4 = m58.getSubMatrix(2, 0);
	assert(m58s4.getNumRows() == 2);
	assert(m58s4.getNumColumns() == 2);
	assert(deq(m58s4.getCell(0, 0), 2));
	assert(deq(m58s4.getCell(0, 1), 3));
	assert(deq(m58s4.getCell(1, 0), 5));
	assert(deq(m58s4.getCell(1, 1), 6));

	// Dense, ignore bottomRight
	Matrix m58s5 = m58.getSubMatrix(2, 2);
	assert(m58s5.getNumRows() == 2);
	assert(m58s5.getNumColumns() == 2);
	assert(deq(m58s5.getCell(0, 0), 1));
	assert(deq(m58s5.getCell(0, 1), 2));
	assert(deq(m58s5.getCell(1, 0), 4));
	assert(deq(m58s5.getCell(1, 1), 5));

	// Sparse, ignore some coordinates
	Matrix m59 = Matrix::createSparse(3, 3);
	m59.setCell(0, 0, 1);
	m59.setCell(0, 1, 2);
	m59.setCell(0, 2, 3);
	m59.setCell(1, 0, 4);
	m59.setCell(1, 1, 5);
	m59.setCell(1, 2, 6);
	m59.setCell(2, 0, 7);
	m59.setCell(2, 1, 8);
	m59.setCell(2, 2, 9);

	Matrix m59s1 = m59.getSubMatrix(2, 1);
	assert(m59s1.getNumRows() == 2);
	assert(m59s1.getNumColumns() == 2);
	assert(deq(m59s1.getCell(0, 0), 1));
	assert(deq(m59s1.getCell(0, 1), 3));
	assert(deq(m59s1.getCell(1, 0), 4));
	assert(deq(m59s1.getCell(1, 1), 6));

	// Sparse, ignore topLeft
	Matrix m59s2 = m59.getSubMatrix(0, 0);
	assert(m59s2.getNumRows() == 2);
	assert(m59s2.getNumColumns() == 2);
	assert(deq(m59s2.getCell(0, 0), 5));
	assert(deq(m59s2.getCell(0, 1), 6));
	assert(deq(m59s2.getCell(1, 0), 8));
	assert(deq(m59s2.getCell(1, 1), 9));

	// Sparse, ignore topRight
	Matrix m59s3 = m59.getSubMatrix(0, 2);
	assert(m59s3.getNumRows() == 2);
	assert(m59s3.getNumColumns() == 2);
	assert(deq(m59s3.getCell(0, 0), 4));
	assert(deq(m59s3.getCell(0, 1), 5));
	assert(deq(m59s3.getCell(1, 0), 7));
	assert(deq(m59s3.getCell(1, 1), 8));

	// Sparse, ignore bottomLeft
	Matrix m59s4 = m59.getSubMatrix(2, 0);
	assert(m59s4.getNumRows() == 2);
	assert(m59s4.getNumColumns() == 2);
	assert(deq(m59s4.getCell(0, 0), 2));
	assert(deq(m59s4.getCell(0, 1), 3));
	assert(deq(m59s4.getCell(1, 0), 5));
	assert(deq(m59s4.getCell(1, 1), 6));

	// Sparse, ignore bottomRight
	Matrix m59s5 = m59.getSubMatrix(2, 2);
	assert(m59s5.getNumRows() == 2);
	assert(m59s5.getNumColumns() == 2);
	assert(deq(m59s5.getCell(0, 0), 1));
	assert(deq(m59s5.getCell(0, 1), 2));
	assert(deq(m59s5.getCell(1, 0), 4));
	assert(deq(m59s5.getCell(1, 1), 5));

	// ****************************** getSubMatrixTopLeft ******************************
	// Dense, valid
	Matrix m60 = Matrix::createDense(3, 3, 0);
	m60.setCell(0, 0, 1);
	m60.setCell(0, 1, 2);
	m60.setCell(0, 2, 3);
	m60.setCell(1, 0, 4);
	m60.setCell(1, 1, 5);
	m60.setCell(1, 2, 6);
	m60.setCell(2, 0, 7);
	m60.setCell(2, 1, 8);
	m60.setCell(2, 2, 9);

	Matrix m60s1 = m60.getSubMatrixTopLeft(1, 1);
	assert(m60s1.getNumRows() == 1);
	assert(m60s1.getNumColumns() == 1);
	assert(deq(m60s1.getCell(0, 0), 1));

	Matrix m60s2 = m60.getSubMatrixTopLeft(1, 2);
	assert(m60s2.getNumRows() == 1);
	assert(m60s2.getNumColumns() == 2);
	assert(deq(m60s2.getCell(0, 0), 1));
	assert(deq(m60s2.getCell(0, 1), 2));

	// Dense, invalid
	Matrix m60s3i = m60.getSubMatrixTopLeft(0, 0);
	assert(m60s3i.getNumRows() == 0);
	assert(m60s3i.getNumColumns() == 0);

	// Sparse, valid
	Matrix m61 = Matrix::createSparse(3, 3);
	m61.setCell(0, 0, 1);
	m61.setCell(0, 1, 2);
	m61.setCell(0, 2, 3);
	m61.setCell(1, 0, 4);
	m61.setCell(1, 1, 5);
	m61.setCell(1, 2, 6);
	m61.setCell(2, 0, 7);
	m61.setCell(2, 1, 8);
	m61.setCell(2, 2, 9);

	Matrix m61s1 = m61.getSubMatrixTopLeft(1, 1);
	assert(m61s1.getNumRows() == 1);
	assert(m61s1.getNumColumns() == 1);
	assert(deq(m61s1.getCell(0, 0), 1));

	Matrix m61s2 = m61.getSubMatrixTopLeft(1, 2);
	assert(m61s2.getNumRows() == 1);
	assert(m61s2.getNumColumns() == 2);
	assert(deq(m61s2.getCell(0, 0), 1));
	assert(deq(m61s2.getCell(0, 1), 2));

	// Sparse, invalid
	Matrix m61s3i = m61.getSubMatrixTopLeft(0, 0);
	assert(m61s3i.getNumRows() == 0);
	assert(m61s3i.getNumColumns() == 0);

	// ****************************** getSubMatrixTopRight ******************************
	// Dense, valid
	Matrix m62 = Matrix::createDense(3, 3, 0);
	m62.setCell(0, 0, 1);
	m62.setCell(0, 1, 2);
	m62.setCell(0, 2, 3);
	m62.setCell(1, 0, 4);
	m62.setCell(1, 1, 5);
	m62.setCell(1, 2, 6);
	m62.setCell(2, 0, 7);
	m62.setCell(2, 1, 8);
	m62.setCell(2, 2, 9);

	Matrix m62s1 = m62.getSubMatrixTopRight(1, 1);
	assert(m62s1.getNumRows() == 1);
	assert(m62s1.getNumColumns() == 1);
	assert(deq(m62s1.getCell(0, 0), 3));

	Matrix m62s2 = m62.getSubMatrixTopRight(1, 0);
	assert(m62s2.getNumRows() == 1);
	assert(m62s2.getNumColumns() == 2);
	assert(deq(m62s2.getCell(0, 0), 2));
	assert(deq(m62s2.getCell(0, 1), 3));

	// Dense, invalid
	Matrix m62s3i = m62.getSubMatrixTopRight(0, 2);
	assert(m62s3i.getNumRows() == 0);
	assert(m62s3i.getNumColumns() == 0);

	// Sparse, valid
	Matrix m63 = Matrix::createSparse(3, 3);
	m63.setCell(0, 0, 1);
	m63.setCell(0, 1, 2);
	m63.setCell(0, 2, 3);
	m63.setCell(1, 0, 4);
	m63.setCell(1, 1, 5);
	m63.setCell(1, 2, 6);
	m63.setCell(2, 0, 7);
	m63.setCell(2, 1, 8);
	m63.setCell(2, 2, 9);

	Matrix m63s1 = m63.getSubMatrixTopRight(1, 1);
	assert(m63s1.getNumRows() == 1);
	assert(m63s1.getNumColumns() == 1);
	assert(deq(m63s1.getCell(0, 0), 3));

	Matrix m63s2 = m63.getSubMatrixTopRight(1, 0);
	assert(m63s2.getNumRows() == 1);
	assert(m63s2.getNumColumns() == 2);
	assert(deq(m63s2.getCell(0, 0), 2));
	assert(deq(m63s2.getCell(0, 1), 3));

	// Sparse, invalid
	Matrix m63s3i = m63.getSubMatrixTopRight(0, 2);
	assert(m63s3i.getNumRows() == 0);
	assert(m63s3i.getNumColumns() == 0);

	// ****************************** getSubMatrixBottomLeft ******************************
		// Dense, valid
	Matrix m64 = Matrix::createDense(3, 3, 0);
	m64.setCell(0, 0, 1);
	m64.setCell(0, 1, 2);
	m64.setCell(0, 2, 3);
	m64.setCell(1, 0, 4);
	m64.setCell(1, 1, 5);
	m64.setCell(1, 2, 6);
	m64.setCell(2, 0, 7);
	m64.setCell(2, 1, 8);
	m64.setCell(2, 2, 9);

	Matrix m64s1 = m64.getSubMatrixBottomLeft(1, 1);
	assert(m64s1.getNumRows() == 1);
	assert(m64s1.getNumColumns() == 1);
	assert(deq(m64s1.getCell(0, 0), 7));

	Matrix m64s2 = m64.getSubMatrixBottomLeft(1, 2);
	assert(m64s2.getNumRows() == 1);
	assert(m64s2.getNumColumns() == 2);
	assert(deq(m64s2.getCell(0, 0), 7));
	assert(deq(m64s2.getCell(0, 1), 8));

	// Dense, invalid
	Matrix m64s3i = m64.getSubMatrixBottomLeft(2, 0);
	assert(m64s3i.getNumRows() == 0);
	assert(m64s3i.getNumColumns() == 0);

	// Sparse, valid
	Matrix m65 = Matrix::createSparse(3, 3);
	m65.setCell(0, 0, 1);
	m65.setCell(0, 1, 2);
	m65.setCell(0, 2, 3);
	m65.setCell(1, 0, 4);
	m65.setCell(1, 1, 5);
	m65.setCell(1, 2, 6);
	m65.setCell(2, 0, 7);
	m65.setCell(2, 1, 8);
	m65.setCell(2, 2, 9);

	Matrix m65s1 = m65.getSubMatrixBottomLeft(1, 1);
	assert(m65s1.getNumRows() == 1);
	assert(m65s1.getNumColumns() == 1);
	assert(deq(m65s1.getCell(0, 0), 7));

	Matrix m65s2 = m65.getSubMatrixBottomLeft(1, 2);
	assert(m65s2.getNumRows() == 1);
	assert(m65s2.getNumColumns() == 2);
	assert(deq(m65s2.getCell(0, 0), 7));
	assert(deq(m65s2.getCell(0, 1), 8));

	// Sparse, invalid
	Matrix m65s3i = m65.getSubMatrixBottomLeft(2, 0);
	assert(m65s3i.getNumRows() == 0);
	assert(m65s3i.getNumColumns() == 0);

	// ****************************** getSubMatrixBottomRight ******************************
	// Dense, valid
	Matrix m66 = Matrix::createDense(3, 3, 0);
	m66.setCell(0, 0, 1);
	m66.setCell(0, 1, 2);
	m66.setCell(0, 2, 3);
	m66.setCell(1, 0, 4);
	m66.setCell(1, 1, 5);
	m66.setCell(1, 2, 6);
	m66.setCell(2, 0, 7);
	m66.setCell(2, 1, 8);
	m66.setCell(2, 2, 9);

	Matrix m66s1 = m66.getSubMatrixBottomRight(1, 1);
	assert(m66s1.getNumRows() == 1);
	assert(m66s1.getNumColumns() == 1);
	assert(deq(m66s1.getCell(0, 0), 9));

	Matrix m66s2 = m66.getSubMatrixBottomRight(1, 0);
	assert(m66s2.getNumRows() == 1);
	assert(m66s2.getNumColumns() == 2);
	assert(deq(m66s2.getCell(0, 0), 8));
	assert(deq(m66s2.getCell(0, 1), 9));

	// Dense, invalid
	Matrix m66s3i = m66.getSubMatrixBottomRight(2, 2);
	assert(m66s3i.getNumRows() == 0);
	assert(m66s3i.getNumColumns() == 0);

	// Sparse, valid
	Matrix m67 = Matrix::createSparse(3, 3);
	m67.setCell(0, 0, 1);
	m67.setCell(0, 1, 2);
	m67.setCell(0, 2, 3);
	m67.setCell(1, 0, 4);
	m67.setCell(1, 1, 5);
	m67.setCell(1, 2, 6);
	m67.setCell(2, 0, 7);
	m67.setCell(2, 1, 8);
	m67.setCell(2, 2, 9);

	Matrix m67s1 = m67.getSubMatrixBottomRight(1, 1);
	assert(m67s1.getNumRows() == 1);
	assert(m67s1.getNumColumns() == 1);
	assert(deq(m67s1.getCell(0, 0), 9));

	Matrix m67s2 = m67.getSubMatrixBottomRight(1, 0);
	assert(m67s2.getNumRows() == 1);
	assert(m67s2.getNumColumns() == 2);
	assert(deq(m67s2.getCell(0, 0), 8));
	assert(deq(m67s2.getCell(0, 1), 9));

	// Sparse, invalid
	Matrix m67s3i = m67.getSubMatrixBottomRight(2, 2);
	assert(m67s3i.getNumRows() == 0);
	assert(m67s3i.getNumColumns() == 0);

	// ****************************** Get Minor Matrix ******************************
	// Dense
	Matrix m68 = Matrix::createDense(4, 4, 0);
	m68.setCell(0, 0, 12);
	m68.setCell(0, 1, 23);
	m68.setCell(0, 2, 34);
	m68.setCell(0, 3, 45);
	m68.setCell(1, 0, 1);
	m68.setCell(1, 1, 2);
	m68.setCell(1, 2, 2);
	m68.setCell(1, 3, 3);
	m68.setCell(2, 0, 3);
	m68.setCell(2, 1, 4);
	m68.setCell(2, 2, 4);
	m68.setCell(2, 3, 5);
	m68.setCell(3, 0, 56);
	m68.setCell(3, 1, 67);
	m68.setCell(3, 2, 78);
	m68.setCell(3, 3, 89);

	Matrix m68minor = m68.getMinorMatrix();
	assert(m68minor.getNumRows() == 4);
	assert(m68minor.getNumColumns() == 4);
	assert(deq(m68minor.getCell(0, 0), 22));
	assert(deq(m68minor.getCell(0, 1), 22));
	assert(deq(m68minor.getCell(0, 2), -22));
	assert(deq(m68minor.getCell(0, 3), -22));
	assert(deq(m68minor.getCell(1, 0), 484));
	assert(deq(m68minor.getCell(1, 1), 484));
	assert(deq(m68minor.getCell(1, 2), -484));
	assert(deq(m68minor.getCell(1, 3), -484));
	assert(deq(m68minor.getCell(2, 0), 484));
	assert(deq(m68minor.getCell(2, 1), 484));
	assert(deq(m68minor.getCell(2, 2), -484));
	assert(deq(m68minor.getCell(2, 3), -484));
	assert(deq(m68minor.getCell(3, 0), 22));
	assert(deq(m68minor.getCell(3, 1), 22));
	assert(deq(m68minor.getCell(3, 2), -22));
	assert(deq(m68minor.getCell(3, 3), -22));

	// Sparse
	Matrix m69 = Matrix::createSparse(4, 4);
	m69.setCell(0, 0, 12);
	m69.setCell(0, 1, 23);
	m69.setCell(0, 2, 34);
	m69.setCell(0, 3, 45);
	m69.setCell(1, 0, 1);
	m69.setCell(1, 1, 2);
	m69.setCell(1, 2, 2);
	m69.setCell(1, 3, 3);
	m69.setCell(2, 0, 3);
	m69.setCell(2, 1, 4);
	m69.setCell(2, 2, 4);
	m69.setCell(2, 3, 5);
	m69.setCell(3, 0, 56);
	m69.setCell(3, 1, 67);
	m69.setCell(3, 2, 78);
	m69.setCell(3, 3, 89);

	Matrix m69minor = m69.getMinorMatrix();
	assert(m69minor.getNumRows() == 4);
	assert(m69minor.getNumColumns() == 4);
	assert(deq(m69minor.getCell(0, 0), 22));
	assert(deq(m69minor.getCell(0, 1), 22));
	assert(deq(m69minor.getCell(0, 2), -22));
	assert(deq(m69minor.getCell(0, 3), -22));
	assert(deq(m69minor.getCell(1, 0), 484));
	assert(deq(m69minor.getCell(1, 1), 484));
	assert(deq(m69minor.getCell(1, 2), -484));
	assert(deq(m69minor.getCell(1, 3), -484));
	assert(deq(m69minor.getCell(2, 0), 484));
	assert(deq(m69minor.getCell(2, 1), 484));
	assert(deq(m69minor.getCell(2, 2), -484));
	assert(deq(m69minor.getCell(2, 3), -484));
	assert(deq(m69minor.getCell(3, 0), 22));
	assert(deq(m69minor.getCell(3, 1), 22));
	assert(deq(m69minor.getCell(3, 2), -22));
	assert(deq(m69minor.getCell(3, 3), -22));

	// ****************************** Apply Checkerboard Pattern ******************************
	// Dense
	Matrix m70 = Matrix::createDense(1, 1, 5);
	m70.applyCheckerboardPattern();
	assert(m70.getNumRows() == 1);
	assert(m70.getNumColumns() == 1);
	assert(deq(m70.getCell(0, 0), 5));

	Matrix m71 = Matrix::createDense(2, 2, 6);
	m71.applyCheckerboardPattern();
	assert(m71.getNumRows() == 2);
	assert(m71.getNumColumns() == 2);
	assert(deq(m71.getCell(0, 0), 6));
	assert(deq(m71.getCell(0, 1), -6));
	assert(deq(m71.getCell(1, 0), -6));
	assert(deq(m71.getCell(1, 1), 6));

	Matrix m72 = Matrix::createDense(3, 3, 7);
	m72.applyCheckerboardPattern();
	assert(m72.getNumRows() == 3);
	assert(m72.getNumColumns() == 3);
	assert(deq(m72.getCell(0, 0), 7));
	assert(deq(m72.getCell(0, 1), -7));
	assert(deq(m72.getCell(0, 2), 7));
	assert(deq(m72.getCell(1, 0), -7));
	assert(deq(m72.getCell(1, 1), 7));
	assert(deq(m72.getCell(1, 2), -7));
	assert(deq(m72.getCell(2, 0), 7));
	assert(deq(m72.getCell(2, 1), -7));
	assert(deq(m72.getCell(2, 2), 7));

	Matrix m73 = Matrix::createDense(4, 4, 8);
	m73.applyCheckerboardPattern();
	assert(m73.getNumRows() == 4);
	assert(m73.getNumColumns() == 4);
	assert(deq(m73.getCell(0, 0), 8));
	assert(deq(m73.getCell(0, 1), -8));
	assert(deq(m73.getCell(0, 2), 8));
	assert(deq(m73.getCell(0, 3), -8));
	assert(deq(m73.getCell(1, 0), -8));
	assert(deq(m73.getCell(1, 1), 8));
	assert(deq(m73.getCell(1, 2), -8));
	assert(deq(m73.getCell(1, 3), 8));
	assert(deq(m73.getCell(2, 0), 8));
	assert(deq(m73.getCell(2, 1), -8));
	assert(deq(m73.getCell(2, 2), 8));
	assert(deq(m73.getCell(2, 3), -8));
	assert(deq(m73.getCell(3, 0), -8));
	assert(deq(m73.getCell(3, 1), 8));
	assert(deq(m73.getCell(3, 2), -8));
	assert(deq(m73.getCell(3, 3), 8));

	// Sparse
	Matrix m74 = Matrix::createSparse(1, 1);
	m74.setCell(0, 0, 5);
	m74.applyCheckerboardPattern();
	assert(m74.getNumRows() == 1);
	assert(m74.getNumColumns() == 1);
	assert(deq(m74.getCell(0, 0), 5));

	Matrix m75 = Matrix::createSparse(2, 2);
	m75.setCell(0, 0, 6);
	m75.setCell(0, 1, 6);
	m75.setCell(1, 0, 6);
	m75.setCell(1, 1, 6);
	m75.applyCheckerboardPattern();
	assert(m75.getNumRows() == 2);
	assert(m75.getNumColumns() == 2);
	assert(deq(m75.getCell(0, 0), 6));
	assert(deq(m75.getCell(0, 1), -6));
	assert(deq(m75.getCell(1, 0), -6));
	assert(deq(m75.getCell(1, 1), 6));

	Matrix m76 = Matrix::createSparse(3, 3);
	m76.setCell(0, 0, 7);
	m76.setCell(0, 1, 7);
	m76.setCell(0, 2, 7);
	m76.setCell(1, 0, 7);
	m76.setCell(1, 1, 7);
	m76.setCell(1, 2, 7);
	m76.setCell(2, 0, 7);
	m76.setCell(2, 1, 7);
	m76.setCell(2, 2, 7);
	m76.applyCheckerboardPattern();
	assert(m76.getNumRows() == 3);
	assert(m76.getNumColumns() == 3);
	assert(deq(m76.getCell(0, 0), 7));
	assert(deq(m76.getCell(0, 1), -7));
	assert(deq(m76.getCell(0, 2), 7));
	assert(deq(m76.getCell(1, 0), -7));
	assert(deq(m76.getCell(1, 1), 7));
	assert(deq(m76.getCell(1, 2), -7));
	assert(deq(m76.getCell(2, 0), 7));
	assert(deq(m76.getCell(2, 1), -7));
	assert(deq(m76.getCell(2, 2), 7));

	Matrix m77 = Matrix::createSparse(4, 4);
	m77.setCell(0, 0, 8);
	m77.setCell(0, 1, 8);
	m77.setCell(0, 2, 8);
	m77.setCell(0, 3, 8);
	m77.setCell(1, 0, 8);
	m77.setCell(1, 1, 8);
	m77.setCell(1, 2, 8);
	m77.setCell(1, 3, 8);
	m77.setCell(2, 0, 8);
	m77.setCell(2, 1, 8);
	m77.setCell(2, 2, 8);
	m77.setCell(2, 3, 8);
	m77.setCell(3, 0, 8);
	m77.setCell(3, 1, 8);
	m77.setCell(3, 2, 8);
	m77.setCell(3, 3, 8);
	m77.applyCheckerboardPattern();
	assert(m77.getNumRows() == 4);
	assert(m77.getNumColumns() == 4);
	assert(deq(m77.getCell(0, 0), 8));
	assert(deq(m77.getCell(0, 1), -8));
	assert(deq(m77.getCell(0, 2), 8));
	assert(deq(m77.getCell(0, 3), -8));
	assert(deq(m77.getCell(1, 0), -8));
	assert(deq(m77.getCell(1, 1), 8));
	assert(deq(m77.getCell(1, 2), -8));
	assert(deq(m77.getCell(1, 3), 8));
	assert(deq(m77.getCell(2, 0), 8));
	assert(deq(m77.getCell(2, 1), -8));
	assert(deq(m77.getCell(2, 2), 8));
	assert(deq(m77.getCell(2, 3), -8));
	assert(deq(m77.getCell(3, 0), -8));
	assert(deq(m77.getCell(3, 1), 8));
	assert(deq(m77.getCell(3, 2), -8));
	assert(deq(m77.getCell(3, 3), 8));

	// ****************************** Get Inverse ******************************
	// ********** Dense Inverse **********
	// Dense (1x1) (invalid)
	Matrix m78 = Matrix::createDense(1, 1, 0);
	assert(deq(m78.getDeterminant(), 0));
	Matrix m78_inv = m78.getInverse(m78.getDeterminant());
	assert(m78_inv.getNumRows() == 0);
	assert(m78_inv.getNumColumns() == 0);

	// Dense (1x1) (valid)
	Matrix m79 = Matrix::createDense(1, 1, 5);
	assert(deq(m79.getDeterminant(), 5));

	Matrix m79_inv = m79.getInverse(m79.getDeterminant());
	assert(m79_inv.getNumRows() == 1);
	assert(m79_inv.getNumColumns() == 1);
	assert(deq(m79_inv.getCell(0, 0), 5));

	Matrix m79_inv_inv = m79_inv.getInverse(m79_inv.getDeterminant());
	assert(m79_inv_inv == m79);

	// Dense (2x2) (valid)
	Matrix m80 = Matrix::createDense(2, 2, 0);
	m80.setCell(0, 0, 1);
	m80.setCell(0, 1, 2);
	m80.setCell(1, 0, 3);
	m80.setCell(1, 1, 4);

	Matrix m80_inv = m80.getInverse(m80.getDeterminant());
	assert(m80_inv.getNumRows() == 2);
	assert(m80_inv.getNumColumns() == 2);
	assert(deq(m80_inv.getCell(0, 0), -2));
	assert(deq(m80_inv.getCell(0, 1), 1));
	assert(deq(m80_inv.getCell(1, 0), (3.0 / 2.0)));
	assert(deq(m80_inv.getCell(1, 1), -(1.0 / 2.0)));

	Matrix m80_inv_inv = m80_inv.getInverse(m80_inv.getDeterminant());
	assert(m80_inv_inv.getNumRows() == 2);
	assert(m80_inv_inv.getNumColumns() == 2);
	assert(m80_inv_inv == m80);

	// Dense (3x3) (valid)
	Matrix m81 = Matrix::createDense(3, 3, 0);
	m81.setCell(0, 0, 12);
	m81.setCell(0, 1, 1);
	m81.setCell(0, 2, 2);
	m81.setCell(1, 0, 3);
	m81.setCell(1, 1, 4);
	m81.setCell(1, 2, 5);
	m81.setCell(2, 0, 6);
	m81.setCell(2, 1, 7);
	m81.setCell(2, 2, 8);

	Matrix m81_inv = m81.getInverse(m81.getDeterminant());
	assert(m81_inv.getNumRows() == 3);
	assert(m81_inv.getNumColumns() == 3);
	assert(deq(m81_inv.getCell(0, 0), (1.0 / 12.0)));
	assert(deq(m81_inv.getCell(0, 1), -(1.0 / 6.0)));
	assert(deq(m81_inv.getCell(0, 2), (1.0 / 12.0)));
	assert(deq(m81_inv.getCell(1, 0), -(1.0 / 6.0)));
	assert(deq(m81_inv.getCell(1, 1), -(7.0 / 3.0)));
	assert(deq(m81_inv.getCell(1, 2), (3.0 / 2.0)));
	assert(deq(m81_inv.getCell(2, 0), (1.0 / 12.0)));
	assert(deq(m81_inv.getCell(2, 1), (13.0 / 6.0)));
	assert(deq(m81_inv.getCell(2, 2), -(5.0 / 4.0)));

	Matrix m81_inv_inv = m81_inv.getInverse(m81_inv.getDeterminant());
	assert(m81_inv_inv == m81);

	// Dense (4x4) (valid)
	Matrix m82 = Matrix::createDense(4, 4, 0);
	m82.setCell(0, 0, 1);
	m82.setCell(0, 1, 2);
	m82.setCell(0, 2, 3);
	m82.setCell(0, 3, 4);
	m82.setCell(1, 0, 5);
	m82.setCell(1, 1, 12);
	m82.setCell(1, 2, 1);
	m82.setCell(1, 3, 2);
	m82.setCell(2, 0, 6);
	m82.setCell(2, 1, 3);
	m82.setCell(2, 2, 4);
	m82.setCell(2, 3, 5);
	m82.setCell(3, 0, 7);
	m82.setCell(3, 1, 6);
	m82.setCell(3, 2, 7);
	m82.setCell(3, 3, 8);

	Matrix m82_inv = m82.getInverse(m82.getDeterminant());
	assert(m82_inv.getNumRows() == 4);
	assert(m82_inv.getNumColumns() == 4);
	assert(deq(m82_inv.getCell(0, 0), -(3.0 / 14.0)));
	assert(deq(m82_inv.getCell(0, 1), 0));
	assert(deq(m82_inv.getCell(0, 2), (2.0 / 7.0)));
	assert(deq(m82_inv.getCell(0, 3), -(1.0 / 14.0)));
	assert(deq(m82_inv.getCell(1, 0), 0));
	assert(deq(m82_inv.getCell(1, 1), (1.0 / 12.0)));
	assert(deq(m82_inv.getCell(1, 2), -(1.0 / 6.0)));
	assert(deq(m82_inv.getCell(1, 3), (1.0 / 12.0)));
	assert(deq(m82_inv.getCell(2, 0), -(13.0 / 14.0)));
	assert(deq(m82_inv.getCell(2, 1), -(1.0 / 6.0)));
	assert(deq(m82_inv.getCell(2, 2), -(23.0 / 21.0)));
	assert(deq(m82_inv.getCell(2, 3), (25.0 / 21.0)));
	assert(deq(m82_inv.getCell(3, 0), 1));
	assert(deq(m82_inv.getCell(3, 1), (1.0 / 12.0)));
	assert(deq(m82_inv.getCell(3, 2), (5.0 / 6.0)));
	assert(deq(m82_inv.getCell(3, 3), -(11.0 / 12.0)));

	Matrix m82_inv_inv = m82_inv.getInverse(m82_inv.getDeterminant());
	assert(m82_inv_inv == m82);

	// Dense Identity (5x5) (valid)
	Matrix m83 = Matrix::createDense(5, 5, 0);
	m83.setCell(0, 0, 1);
	m83.setCell(1, 1, 1);
	m83.setCell(2, 2, 1);
	m83.setCell(3, 3, 1);
	m83.setCell(4, 4, 1);

	Matrix m83_inv = m83.getInverse(m83.getDeterminant());
	assert(m83_inv == m83);

	Matrix m83_inv_inv = m83_inv.getInverse(m83_inv.getDeterminant());
	assert(m83_inv_inv == m83);
	assert(m83_inv_inv == m83_inv);

	// ********** Sparse Inverse **********
	// Sparse (1x1) (invalid)
	Matrix m84 = Matrix::createSparse(1, 1);
	assert(deq(m84.getDeterminant(), 0));
	Matrix m84_inv = m84.getInverse(m84.getDeterminant());
	assert(m84_inv.getNumRows() == 0);
	assert(m84_inv.getNumColumns() == 0);

	// Sparse (1x1) (valid)
	Matrix m85 = Matrix::createSparse(1, 1);
	m85.setCell(0, 0, 5);
	assert(deq(m85.getDeterminant(), 5));

	Matrix m85_inv = m85.getInverse(m85.getDeterminant());
	assert(m85_inv.getNumRows() == 1);
	assert(m85_inv.getNumColumns() == 1);
	assert(deq(m85_inv.getCell(0, 0), 5));

	Matrix m85_inv_inv = m85_inv.getInverse(m85_inv.getDeterminant());
	assert(m85_inv_inv == m85);

	// Sparse (2x2) (valid)
	Matrix m86 = Matrix::createSparse(2, 2);
	m86.setCell(0, 0, 1);
	m86.setCell(0, 1, 2);
	m86.setCell(1, 0, 3);
	m86.setCell(1, 1, 4);

	Matrix m86_inv = m86.getInverse(m86.getDeterminant());
	assert(m86_inv.getNumRows() == 2);
	assert(m86_inv.getNumColumns() == 2);
	assert(deq(m86_inv.getCell(0, 0), -2));
	assert(deq(m86_inv.getCell(0, 1), 1));
	assert(deq(m86_inv.getCell(1, 0), (3.0 / 2.0)));
	assert(deq(m86_inv.getCell(1, 1), -(1.0 / 2.0)));

	Matrix m86_inv_inv = m86_inv.getInverse(m86_inv.getDeterminant());
	assert(m86_inv_inv.getNumRows() == 2);
	assert(m86_inv_inv.getNumColumns() == 2);
	assert(m86_inv_inv == m86);

	// Sparse (3x3) (valid)
	Matrix m87 = Matrix::createSparse(3, 3);
	m87.setCell(0, 0, 12);
	m87.setCell(0, 1, 1);
	m87.setCell(0, 2, 2);
	m87.setCell(1, 0, 3);
	m87.setCell(1, 1, 4);
	m87.setCell(1, 2, 5);
	m87.setCell(2, 0, 6);
	m87.setCell(2, 1, 7);
	m87.setCell(2, 2, 8);

	Matrix m87_inv = m87.getInverse(m87.getDeterminant());
	assert(m87_inv.getNumRows() == 3);
	assert(m87_inv.getNumColumns() == 3);
	assert(deq(m87_inv.getCell(0, 0), (1.0 / 12.0)));
	assert(deq(m87_inv.getCell(0, 1), -(1.0 / 6.0)));
	assert(deq(m87_inv.getCell(0, 2), (1.0 / 12.0)));
	assert(deq(m87_inv.getCell(1, 0), -(1.0 / 6.0)));
	assert(deq(m87_inv.getCell(1, 1), -(7.0 / 3.0)));
	assert(deq(m87_inv.getCell(1, 2), (3.0 / 2.0)));
	assert(deq(m87_inv.getCell(2, 0), (1.0 / 12.0)));
	assert(deq(m87_inv.getCell(2, 1), (13.0 / 6.0)));
	assert(deq(m87_inv.getCell(2, 2), -(5.0 / 4.0)));

	Matrix m87_inv_inv = m87_inv.getInverse(m87_inv.getDeterminant());
	assert(m87_inv_inv == m87);

	// Sparse (4x4) (valid)
	Matrix m88 = Matrix::createSparse(4, 4);
	m88.setCell(0, 0, 1);
	m88.setCell(0, 1, 2);
	m88.setCell(0, 2, 3);
	m88.setCell(0, 3, 4);
	m88.setCell(1, 0, 5);
	m88.setCell(1, 1, 12);
	m88.setCell(1, 2, 1);
	m88.setCell(1, 3, 2);
	m88.setCell(2, 0, 6);
	m88.setCell(2, 1, 3);
	m88.setCell(2, 2, 4);
	m88.setCell(2, 3, 5);
	m88.setCell(3, 0, 7);
	m88.setCell(3, 1, 6);
	m88.setCell(3, 2, 7);
	m88.setCell(3, 3, 8);

	Matrix m88_inv = m88.getInverse(m88.getDeterminant());
	assert(m88_inv.getNumRows() == 4);
	assert(m88_inv.getNumColumns() == 4);
	assert(deq(m88_inv.getCell(0, 0), -(3.0 / 14.0)));
	assert(deq(m88_inv.getCell(0, 1), 0));
	assert(deq(m88_inv.getCell(0, 2), (2.0 / 7.0)));
	assert(deq(m88_inv.getCell(0, 3), -(1.0 / 14.0)));
	assert(deq(m88_inv.getCell(1, 0), 0));
	assert(deq(m88_inv.getCell(1, 1), (1.0 / 12.0)));
	assert(deq(m88_inv.getCell(1, 2), -(1.0 / 6.0)));
	assert(deq(m88_inv.getCell(1, 3), (1.0 / 12.0)));
	assert(deq(m88_inv.getCell(2, 0), -(13.0 / 14.0)));
	assert(deq(m88_inv.getCell(2, 1), -(1.0 / 6.0)));
	assert(deq(m88_inv.getCell(2, 2), -(23.0 / 21.0)));
	assert(deq(m88_inv.getCell(2, 3), (25.0 / 21.0)));
	assert(deq(m88_inv.getCell(3, 0), 1));
	assert(deq(m88_inv.getCell(3, 1), (1.0 / 12.0)));
	assert(deq(m88_inv.getCell(3, 2), (5.0 / 6.0)));
	assert(deq(m88_inv.getCell(3, 3), -(11.0 / 12.0)));

	Matrix m88_inv_inv = m88_inv.getInverse(m88_inv.getDeterminant());
	assert(m88_inv_inv == m88);

	// Sparse Identity (5x5) (valid)
	Matrix m89 = Matrix::createIdentity(5);
	Matrix m89_inv = m89.getInverse(m89.getDeterminant());
	assert(m89_inv == m89);

	Matrix m89_inv_inv = m89_inv.getInverse(m89_inv.getDeterminant());
	assert(m89_inv_inv == m89);
	assert(m89_inv_inv == m89_inv);

	// ****************************** Solve For ******************************
	// Dense (1 solution)
	Matrix m90 = Matrix::createDense(4, 4, 0);
	m90.setCell(0, 0, 1);
	m90.setCell(0, 1, 2);
	m90.setCell(0, 2, -3);
	m90.setCell(0, 3, 0);
	m90.setCell(1, 0, 1);
	m90.setCell(1, 1, -1);
	m90.setCell(1, 2, 0);
	m90.setCell(1, 3, 1);
	m90.setCell(2, 0, 0);
	m90.setCell(2, 1, 2);
	m90.setCell(2, 2, -2);
	m90.setCell(2, 3, 0);
	m90.setCell(3, 0, 2);
	m90.setCell(3, 1, 1);
	m90.setCell(3, 2, 1);
	m90.setCell(3, 3, 1);

	Matrix m90_aug = Matrix::createDense(4, 1, 0);
	m90_aug.setCell(0, 0, 1);
	m90_aug.setCell(1, 0, -1);
	m90_aug.setCell(2, 0, 3);
	m90_aug.setCell(3, 0, 0);

	std::string m90_solution_str_test = "\nSolution:\n\nx1 = - 2\nx2 = + 1.5\nx3 = + 0\nx4 = + 2.5\n";
	std::string m90_solution_str = m90.solveFor(m90_aug, false, 2);
	assert(streq(m90_solution_str, m90_solution_str_test));

	// Dense (infinite solutions)
	Matrix m91 = Matrix::createDense(3, 4, 0);
	m91.setCell(0, 0, 1);
	m91.setCell(0, 1, 3);
	m91.setCell(0, 2, -1);
	m91.setCell(0, 3, 4);
	m91.setCell(1, 0, 1);
	m91.setCell(1, 1, 1);
	m91.setCell(1, 2, -1);
	m91.setCell(1, 3, -2);
	m91.setCell(2, 0, 1);
	m91.setCell(2, 1, 7);
	m91.setCell(2, 2, -1);
	m91.setCell(2, 3, 16);

	Matrix m91_aug = Matrix::createDense(3, 1, 0);
	m91_aug.setCell(0, 0, 8);
	m91_aug.setCell(1, 0, 2);
	m91_aug.setCell(2, 0, 20);

	std::string m91_solution_str_test = "\nSolution:\n\nx1 = + 1x3 + 5x4 - 1\nx2 = - 3x4 + 3\nx3, x4 are free variables.\n";
	std::string m91_solution_str = m91.solveFor(m91_aug, false, 2);
	assert(streq(m91_solution_str, m91_solution_str_test));

	// Dense (no solution)
	Matrix m92 = Matrix::createDense(4, 4, 0);
	m92.setCell(0, 0, 3);
	m92.setCell(0, 1, -1);
	m92.setCell(0, 2, -1);
	m92.setCell(0, 3, 0);
	m92.setCell(1, 0, 1);
	m92.setCell(1, 1, -1);
	m92.setCell(1, 2, 0);
	m92.setCell(1, 3, 1);
	m92.setCell(2, 0, 0);
	m92.setCell(2, 1, 1);
	m92.setCell(2, 2, -2);
	m92.setCell(2, 3, 2);
	m92.setCell(3, 0, -1);
	m92.setCell(3, 1, 3);
	m92.setCell(3, 2, -1);
	m92.setCell(3, 3, -4);

	Matrix m92_aug = Matrix::createDense(4, 1, 0);
	m92_aug.setCell(0, 0, 2);
	m92_aug.setCell(1, 0, 5);
	m92_aug.setCell(2, 0, 0);
	m92_aug.setCell(3, 0, -1);

	std::string m92_solution_str_test = "\nSolution:\n\nNo solution.\n";
	std::string m92_solution_str = m92.solveFor(m92_aug, false, 2);
	assert(streq(m92_solution_str, m92_solution_str_test));

	// Sparse (1 solution)
	Matrix m93 = Matrix::createSparse(4, 4);
	m93.setCell(0, 0, 1);
	m93.setCell(0, 1, 2);
	m93.setCell(0, 2, -3);
	m93.setCell(0, 3, 0);
	m93.setCell(1, 0, 1);
	m93.setCell(1, 1, -1);
	m93.setCell(1, 2, 0);
	m93.setCell(1, 3, 1);
	m93.setCell(2, 0, 0);
	m93.setCell(2, 1, 2);
	m93.setCell(2, 2, -2);
	m93.setCell(2, 3, 0);
	m93.setCell(3, 0, 2);
	m93.setCell(3, 1, 1);
	m93.setCell(3, 2, 1);
	m93.setCell(3, 3, 1);

	Matrix m93_aug = Matrix::createSparse(4, 1);
	m93_aug.setCell(0, 0, 1);
	m93_aug.setCell(1, 0, -1);
	m93_aug.setCell(2, 0, 3);
	m93_aug.setCell(3, 0, 0);

	std::string m93_solution_str_test = "\nSolution:\n\nx1 = - 2\nx2 = + 1.5\nx3 = + 0\nx4 = + 2.5\n";
	std::string m93_solution_str = m93.solveFor(m93_aug, false, 2);
	assert(streq(m93_solution_str, m93_solution_str_test));

	// Sparse (infinite solutions)
	Matrix m94 = Matrix::createSparse(3, 4);
	m94.setCell(0, 0, 1);
	m94.setCell(0, 1, 3);
	m94.setCell(0, 2, -1);
	m94.setCell(0, 3, 4);
	m94.setCell(1, 0, 1);
	m94.setCell(1, 1, 1);
	m94.setCell(1, 2, -1);
	m94.setCell(1, 3, -2);
	m94.setCell(2, 0, 1);
	m94.setCell(2, 1, 7);
	m94.setCell(2, 2, -1);
	m94.setCell(2, 3, 16);

	Matrix m94_aug = Matrix::createDense(3, 1, 0);
	m94_aug.setCell(0, 0, 8);
	m94_aug.setCell(1, 0, 2);
	m94_aug.setCell(2, 0, 20);

	std::string m94_solution_str_test = "\nSolution:\n\nx1 = + 1x3 + 5x4 - 1\nx2 = - 3x4 + 3\nx3, x4 are free variables.\n";
	std::string m94_solution_str = m94.solveFor(m94_aug, false, 2);
	assert(streq(m94_solution_str, m94_solution_str_test));

	// Sparse (no solution)
	Matrix m95 = Matrix::createSparse(4, 4);
	m95.setCell(0, 0, 3);
	m95.setCell(0, 1, -1);
	m95.setCell(0, 2, -1);
	m95.setCell(0, 3, 0);
	m95.setCell(1, 0, 1);
	m95.setCell(1, 1, -1);
	m95.setCell(1, 2, 0);
	m95.setCell(1, 3, 1);
	m95.setCell(2, 0, 0);
	m95.setCell(2, 1, 1);
	m95.setCell(2, 2, -2);
	m95.setCell(2, 3, 2);
	m95.setCell(3, 0, -1);
	m95.setCell(3, 1, 3);
	m95.setCell(3, 2, -1);
	m95.setCell(3, 3, -4);

	Matrix m95_aug = Matrix::createSparse(4, 1);
	m95_aug.setCell(0, 0, 2);
	m95_aug.setCell(1, 0, 5);
	m95_aug.setCell(2, 0, 0);
	m95_aug.setCell(3, 0, -1);

	std::string m95_solution_str_test = "\nSolution:\n\nNo solution.\n";
	std::string m95_solution_str = m95.solveFor(m95_aug, false, 2);
	assert(streq(m95_solution_str, m95_solution_str_test));

	// ****************************** Rank ******************************
	// ********** Dense **********
	Matrix m96 = Matrix::createDense(5, 5, 0);
	m96.setCell(0, 0, 1);
	m96.setCell(0, 1, 2);
	m96.setCell(0, 2, 2);
	m96.setCell(0, 3, 1);
	m96.setCell(0, 4, 3);
	m96.setCell(1, 0, 2);
	m96.setCell(1, 1, 1);
	m96.setCell(1, 2, 3);
	m96.setCell(1, 3, 1);
	m96.setCell(1, 4, 2);
	m96.setCell(2, 0, 2);
	m96.setCell(2, 1, 2);
	m96.setCell(2, 2, 0);
	m96.setCell(2, 3, 1);
	m96.setCell(2, 4, 3);
	m96.setCell(3, 0, 2);
	m96.setCell(3, 1, 0);
	m96.setCell(3, 2, 1);
	m96.setCell(3, 3, 2);
	m96.setCell(3, 4, 2);
	m96.setCell(4, 0, 1);
	m96.setCell(4, 1, -5);
	m96.setCell(4, 2, 3);
	m96.setCell(4, 3, 2);
	m96.setCell(4, 4, -3);

	assert(m96.getRank() == 4);

	Matrix m97 = Matrix::createDense(4, 4, 0);
	m97.setCell(0, 0, 0);
	m97.setCell(0, 1, 4);
	m97.setCell(0, 2, 10);
	m97.setCell(0, 3, 1);
	m97.setCell(1, 0, 4);
	m97.setCell(1, 1, 8);
	m97.setCell(1, 2, 18);
	m97.setCell(1, 3, 7);
	m97.setCell(2, 0, 10);
	m97.setCell(2, 1, 18);
	m97.setCell(2, 2, 40);
	m97.setCell(2, 3, 17);
	m97.setCell(3, 0, 1);
	m97.setCell(3, 1, 7);
	m97.setCell(3, 2, 17);
	m97.setCell(3, 3, 3);

	assert(m97.getRank() == 2);

	Matrix m98 = Matrix::createDense(3, 3, 0);
	m98.setCell(0, 0, 3);
	m98.setCell(0, 1, 2);
	m98.setCell(0, 2, 4);
	m98.setCell(1, 0, 4);
	m98.setCell(1, 1, 0);
	m98.setCell(1, 2, 2);
	m98.setCell(2, 0, 4);
	m98.setCell(2, 1, 4);
	m98.setCell(2, 2, 2);

	assert(m98.getRank() == 3);

	Matrix m99 = Matrix::createDense(6, 4, 0);
	m99.setCell(0, 0, 2);
	m99.setCell(0, 1, 1);
	m99.setCell(0, 2, 1);
	m99.setCell(0, 3, 1);
	m99.setCell(1, 0, 1);
	m99.setCell(1, 1, 3);
	m99.setCell(1, 2, 1);
	m99.setCell(1, 3, 1);
	m99.setCell(2, 0, 1);
	m99.setCell(2, 1, 1);
	m99.setCell(2, 2, 4);
	m99.setCell(2, 3, 1);
	m99.setCell(3, 0, 1);
	m99.setCell(3, 1, 1);
	m99.setCell(3, 2, 1);
	m99.setCell(3, 3, 5);
	m99.setCell(4, 0, 1);
	m99.setCell(4, 1, 2);
	m99.setCell(4, 2, 3);
	m99.setCell(4, 3, 4);
	m99.setCell(5, 0, 1);
	m99.setCell(5, 1, 1);
	m99.setCell(5, 2, 1);
	m99.setCell(5, 3, 1);

	assert(m99.getRank() == 4);

	Matrix m100 = Matrix::createDense(4, 7, 0);
	m100.setCell(0, 0, 3);
	m100.setCell(0, 1, 1);
	m100.setCell(0, 2, 2);
	m100.setCell(0, 3, 2);
	m100.setCell(0, 4, 2);
	m100.setCell(0, 5, 1);
	m100.setCell(0, 6, 3);
	m100.setCell(1, 0, 1);
	m100.setCell(1, 1, 2);
	m100.setCell(1, 2, 1);
	m100.setCell(1, 3, 1);
	m100.setCell(1, 4, 3);
	m100.setCell(1, 5, 1);
	m100.setCell(1, 6, 2);
	m100.setCell(2, 0, 2);
	m100.setCell(2, 1, 2);
	m100.setCell(2, 2, 0);
	m100.setCell(2, 3, 0);
	m100.setCell(2, 4, 1);
	m100.setCell(2, 5, 3);
	m100.setCell(2, 6, 3);
	m100.setCell(3, 0, 3);
	m100.setCell(3, 1, -1);
	m100.setCell(3, 2, 0);
	m100.setCell(3, 3, 0);
	m100.setCell(3, 4, -3);
	m100.setCell(3, 5, 2);
	m100.setCell(3, 6, 2);

	assert(m100.getRank() == 3);

	// ********** Sparse **********
	Matrix m101 = Matrix::createSparse(5, 5);
	m101.setCell(0, 0, 1);
	m101.setCell(0, 1, 2);
	m101.setCell(0, 2, 2);
	m101.setCell(0, 3, 1);
	m101.setCell(0, 4, 3);
	m101.setCell(1, 0, 2);
	m101.setCell(1, 1, 1);
	m101.setCell(1, 2, 3);
	m101.setCell(1, 3, 1);
	m101.setCell(1, 4, 2);
	m101.setCell(2, 0, 2);
	m101.setCell(2, 1, 2);
	m101.setCell(2, 2, 0);
	m101.setCell(2, 3, 1);
	m101.setCell(2, 4, 3);
	m101.setCell(3, 0, 2);
	m101.setCell(3, 1, 0);
	m101.setCell(3, 2, 1);
	m101.setCell(3, 3, 2);
	m101.setCell(3, 4, 2);
	m101.setCell(4, 0, 1);
	m101.setCell(4, 1, -5);
	m101.setCell(4, 2, 3);
	m101.setCell(4, 3, 2);
	m101.setCell(4, 4, -3);

	assert(m101.getRank() == 4);

	Matrix m102 = Matrix::createSparse(4, 4);
	m102.setCell(0, 0, 0);
	m102.setCell(0, 1, 4);
	m102.setCell(0, 2, 10);
	m102.setCell(0, 3, 1);
	m102.setCell(1, 0, 4);
	m102.setCell(1, 1, 8);
	m102.setCell(1, 2, 18);
	m102.setCell(1, 3, 7);
	m102.setCell(2, 0, 10);
	m102.setCell(2, 1, 18);
	m102.setCell(2, 2, 40);
	m102.setCell(2, 3, 17);
	m102.setCell(3, 0, 1);
	m102.setCell(3, 1, 7);
	m102.setCell(3, 2, 17);
	m102.setCell(3, 3, 3);

	assert(m102.getRank() == 2);

	Matrix m103 = Matrix::createSparse(3, 3);
	m103.setCell(0, 0, 3);
	m103.setCell(0, 1, 2);
	m103.setCell(0, 2, 4);
	m103.setCell(1, 0, 4);
	m103.setCell(1, 1, 0);
	m103.setCell(1, 2, 2);
	m103.setCell(2, 0, 4);
	m103.setCell(2, 1, 4);
	m103.setCell(2, 2, 2);

	assert(m103.getRank() == 3);

	Matrix m104 = Matrix::createSparse(6, 4);
	m104.setCell(0, 0, 2);
	m104.setCell(0, 1, 1);
	m104.setCell(0, 2, 1);
	m104.setCell(0, 3, 1);
	m104.setCell(1, 0, 1);
	m104.setCell(1, 1, 3);
	m104.setCell(1, 2, 1);
	m104.setCell(1, 3, 1);
	m104.setCell(2, 0, 1);
	m104.setCell(2, 1, 1);
	m104.setCell(2, 2, 4);
	m104.setCell(2, 3, 1);
	m104.setCell(3, 0, 1);
	m104.setCell(3, 1, 1);
	m104.setCell(3, 2, 1);
	m104.setCell(3, 3, 5);
	m104.setCell(4, 0, 1);
	m104.setCell(4, 1, 2);
	m104.setCell(4, 2, 3);
	m104.setCell(4, 3, 4);
	m104.setCell(5, 0, 1);
	m104.setCell(5, 1, 1);
	m104.setCell(5, 2, 1);
	m104.setCell(5, 3, 1);

	assert(m104.getRank() == 4);

	Matrix m105 = Matrix::createSparse(4, 7);
	m105.setCell(0, 0, 3);
	m105.setCell(0, 1, 1);
	m105.setCell(0, 2, 2);
	m105.setCell(0, 3, 2);
	m105.setCell(0, 4, 2);
	m105.setCell(0, 5, 1);
	m105.setCell(0, 6, 3);
	m105.setCell(1, 0, 1);
	m105.setCell(1, 1, 2);
	m105.setCell(1, 2, 1);
	m105.setCell(1, 3, 1);
	m105.setCell(1, 4, 3);
	m105.setCell(1, 5, 1);
	m105.setCell(1, 6, 2);
	m105.setCell(2, 0, 2);
	m105.setCell(2, 1, 2);
	m105.setCell(2, 2, 0);
	m105.setCell(2, 3, 0);
	m105.setCell(2, 4, 1);
	m105.setCell(2, 5, 3);
	m105.setCell(2, 6, 3);
	m105.setCell(3, 0, 3);
	m105.setCell(3, 1, -1);
	m105.setCell(3, 2, 0);
	m105.setCell(3, 3, 0);
	m105.setCell(3, 4, -3);
	m105.setCell(3, 5, 2);
	m105.setCell(3, 6, 2);

	assert(m105.getRank() == 3);

	// Print Dense
	Matrix m106 = Matrix::createDense(3, 3, 0);
	m106.setCell(0, 0, 100.92);
	m106.setCell(0, 1, 5);
	m106.setCell(0, 2, 48.02);
	m106.setCell(1, 0, 0);
	m106.setCell(1, 1, 6);
	m106.setCell(1, 2, 7);
	m106.setCell(2, 0, 17.11);
	m106.setCell(2, 1, 55.55);
	m106.setCell(2, 2, 1.02);

	std::string m106_print_str_test = "100.92,  5.00, 48.02\n  0.00,  6.00,  7.00\n 17.11, 55.55,  1.02\n";
	std::string m106_print_str = m106.getPrintStr(2);
	assert(streq(m106_print_str, m106_print_str_test));

	// Print Sparse
	Matrix m107 = Matrix::createSparse(4, 5);
	m107.setCell(0, 0, 1);
	m107.setCell(0, 1, 2);
	m107.setCell(0, 2, -3);
	m107.setCell(0, 3, 0);
	m107.setCell(0, 4, 1);
	m107.setCell(1, 0, 0);
	m107.setCell(1, 1, -3);
	m107.setCell(1, 2, 3);
	m107.setCell(1, 3, 1);
	m107.setCell(1, 4, -2);
	m107.setCell(2, 0, 0);
	m107.setCell(2, 1, 0);
	m107.setCell(2, 2, 0);
	m107.setCell(2, 3, 0.67);
	m107.setCell(2, 4, 1.67);
	m107.setCell(3, 0, 0);
	m107.setCell(3, 1, 0);
	m107.setCell(3, 2, 4);
	m107.setCell(3, 3, 0);
	m107.setCell(3, 4, 0);

	std::string m107_print_str_test = "1.00,  2.00, -3.00, 0.00,  1.00\n0.00, -3.00,  3.00, 1.00, -2.00\n0.00,  0.00,  0.00, 0.67,  1.67\n0.00,  0.00,  4.00, 0.00,  0.00\n";
	std::string m107_print_str = m107.getPrintStr(2);
	assert(streq(m107_print_str, m107_print_str_test));

	return 0;
}