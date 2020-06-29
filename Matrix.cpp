#include "Matrix.h"
#include "MatCalcUtil.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits> // Required by g++ (std::numeric_limits<double>)
#include <functional>

// Public members

Matrix::Matrix()
{
	// Invalid state.
	matrixPtr = nullptr;
}

Matrix::Matrix(const Matrix& other)
{
	if (&other == this)
	{
		return;
	}

	if (other.matrixPtr != nullptr) // Else Invalid state.
	{
		matrixPtr = other.matrixPtr->clone();
	}
}

bool Matrix::operator==(const Matrix& right)
{
	if (this->matrixPtr == nullptr || right.matrixPtr == nullptr)
	{
		return false; // Invalid state.
	}

	return this->matrixPtr->equal((*right.matrixPtr));
}

bool Matrix::operator!=(const Matrix& right)
{
	return !((*this) == right);
}

Matrix Matrix::operator+(const Matrix& right)
{
	Matrix result;

	if (this->matrixPtr == nullptr || right.matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->add(*(right.matrixPtr));

	return result;
}

Matrix Matrix::operator-(const Matrix& right)
{
	Matrix result;

	if (this->matrixPtr == nullptr || right.matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->subtract(*(right.matrixPtr));

	return result;
}

Matrix Matrix::operator*(const Matrix& right)
{
	Matrix result;

	if (this->matrixPtr == nullptr || right.matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->multiply(*(right.matrixPtr));

	return result;
}

Matrix Matrix::operator*(double scalar)
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result = (*this);

	result.matrixPtr->scale(scalar);

	return result;
}

Matrix operator*(double scalar, const Matrix& right)
{
	Matrix result;

	if (right.matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result = right;

	result.matrixPtr->scale(scalar);

	return result;
}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (&other == this)
	{
		return *this;
	}

	destroyResource();

	if (other.matrixPtr != nullptr)
	{
		matrixPtr = other.matrixPtr->clone();
	}
	//else Invalid State.

	return *this;
}

Matrix::~Matrix()
{
	destroyResource();
}

std::string Matrix::getPrintStr(size_t precision) const
{
	std::string ret = "";
	if (matrixPtr == nullptr)
	{
		return ret; // Invalid state.
	}

	ret = matrixPtr->getPrintStr(precision);

	return ret;
}

size_t Matrix::getNumRows() const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->getNumRows();
	}

	return 0; // Invalid state.
}

size_t Matrix::getNumColumns() const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->getNumColumns();
	}

	return 0; // Invalid state.
}

double Matrix::getCell(size_t row, size_t column) const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->getCell(row, column);
	}

	return std::numeric_limits<double>::quiet_NaN(); // Invalid state.
}

void Matrix::setCell(size_t row, size_t column, double value)
{
	if (matrixPtr != nullptr)
	{
		matrixPtr->setCell(row, column, value);
	}

	// else invalid state.
}

void Matrix::resizeNumRows(size_t newNumRows)
{
	if (matrixPtr != nullptr)
	{
		matrixPtr->resizeNumRows(newNumRows);
	}

	// else invalid state.
}

void Matrix::resizeNumColumns(size_t newNumColumns)
{
	if (matrixPtr != nullptr)
	{
		matrixPtr->resizeNumColumns(newNumColumns);
	}

	// else invalid state.
}

void Matrix::resize(size_t newNumRows, size_t newNumColumns)
{
	resizeNumRows(newNumRows);
	resizeNumColumns(newNumColumns);
}

void Matrix::transpose()
{
	if (matrixPtr != nullptr)
	{
		matrixPtr->transpose();
	}

	// else invalid state.
}

double Matrix::getSparsity() const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->getSparsity();
	}

	return std::numeric_limits<double>::quiet_NaN(); // Invalid state.
}

double Matrix::getDensity() const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->getDensity();
	}

	return std::numeric_limits<double>::quiet_NaN(); // Invalid state.
}

bool Matrix::isSparse() const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->isSparse();
	}

	return false; // Invalid state.
}

bool Matrix::isDense() const
{
	if (matrixPtr != nullptr)
	{
		return matrixPtr->isDense();
	}

	return false; // Invalid state.
}

void Matrix::toDense()
{
	if (matrixPtr == nullptr)
	{
		return; // Invalid state.
	}

	MatrixBase* oldPtr = matrixPtr;
	matrixPtr = oldPtr->cloneAsDenseMatrix();
	delete oldPtr;
}

void Matrix::toSparse()
{
	if (matrixPtr == nullptr)
	{
		return; // Invalid state.
	}

	MatrixBase* oldPtr = matrixPtr;
	matrixPtr = oldPtr->cloneAsSparseMatrix();
	delete oldPtr;
}

bool Matrix::requiresConversion() const
{
	if (matrixPtr == nullptr)
	{
		return false; // Invalid state.
	}

	return matrixPtr->requiresConversion();
}

void Matrix::convertToAppropriateMatrixType()
{
	if (matrixPtr == nullptr)
	{
		return; // Invalid state.
	}

	MatrixBase* oldPtr = matrixPtr;
	matrixPtr = matrixPtr->getConvertedCopy();
	
	delete oldPtr;
}

Matrix Matrix::mergeByColumns(const Matrix& right)
{
	Matrix result;

	if (this->matrixPtr == nullptr || right.matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->mergeByColumns(*(right.matrixPtr));

	return result;
}

Matrix Matrix::mergeByRows(const Matrix& right)
{
	Matrix result;

	if (this->matrixPtr == nullptr || right.matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->mergeByRows(*(right.matrixPtr));

	return result;
}

Matrix Matrix::splitByColumn(size_t leftNewNumColumns, bool returnLeftMatrix) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->splitByColumn(leftNewNumColumns, returnLeftMatrix);

	return result;
}

Matrix Matrix::splitByRow(size_t topNewNumRows, bool returnTopMatrix) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->splitByRow(topNewNumRows, returnTopMatrix);

	return result;
}

Matrix Matrix::getSubMatrix(size_t subRowBeginIndex, size_t subNumRows, size_t subColumnBeginIndex, size_t subNumColumns) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getSubMatrix(subRowBeginIndex, subNumRows, subColumnBeginIndex, subNumColumns);

	return result; // Possible invalid state (getSubMatrix could have returned nullptr).
}

Matrix Matrix::getSubMatrix(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getSubMatrix(ignoredRowIndex, ignoredColumnIndex);

	return result;
}

Matrix Matrix::getSubMatrixTopLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getSubMatrixTopLeft(ignoredRowIndex, ignoredColumnIndex);

	return result; // Possible invalid state (getSubMatrixTopLeft could have returned nullptr).
}

Matrix Matrix::getSubMatrixTopRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getSubMatrixTopRight(ignoredRowIndex, ignoredColumnIndex);

	return result; // Possible invalid state (getSubMatrixTopRight could have returned nullptr).
}

Matrix Matrix::getSubMatrixBottomLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getSubMatrixBottomLeft(ignoredRowIndex, ignoredColumnIndex);

	return result; // Possible invalid state (getSubMatrixBottomLeft could have returned nullptr).
}

Matrix Matrix::getSubMatrixBottomRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getSubMatrixBottomRight(ignoredRowIndex, ignoredColumnIndex);

	return result; // Possible invalid state (getSubMatrixBottomRight could have returned nullptr).
}

Matrix Matrix::getMinorMatrix() const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getMinorMatrix();

	return result;
}

void Matrix::applyCheckerboardPattern()
{
	if (this->matrixPtr != nullptr)
	{
		matrixPtr->applyCheckerboardPattern();
	}

	// Invalid state.
}

double Matrix::getDeterminant() const
{
	double determinant = std::numeric_limits<double>::quiet_NaN(); // Invalid state.

	if (matrixPtr != nullptr)
	{
		determinant = matrixPtr->getDeterminant();
	}

	return determinant;
}

Matrix Matrix::getInverse(double determinant) const
{
	Matrix result;

	if (this->matrixPtr == nullptr)
	{
		return result; // Invalid state.
	}

	result.matrixPtr = this->matrixPtr->getInverse(determinant);

	return result; // Possible invalid state (getInverse could have returned nullptr).
}

std::string Matrix::solveFor(const Matrix& augmentedColumn, bool verbose, size_t doublePrecision) const
{
	std::string ret = "";
	if (matrixPtr == nullptr || augmentedColumn.matrixPtr == nullptr)
	{
		return ret; // Invalid state.
	}

	ret = matrixPtr->solveFor(*(augmentedColumn.matrixPtr), verbose, doublePrecision);

	return ret;
}

size_t Matrix::getRank() const
{
	if (matrixPtr == nullptr)
	{
		return 0; // Invalid state.
	}

	return matrixPtr->getRank();
}

// Public static members

Matrix Matrix::createDense(size_t numRows, size_t numColumns, double initialValues)
{
	Matrix dense;
	dense.matrixPtr = new DenseMatrix(numRows, numColumns, initialValues);
	return dense;
}

Matrix Matrix::createSparse(size_t numRows, size_t numColumns)
{
	Matrix sparse;
	sparse.matrixPtr = new SparseMatrix(numRows, numColumns);
	return sparse;
}

Matrix Matrix::createZero(size_t numRows, size_t numColumns)
{
	return createSparse(numRows, numColumns);
}

Matrix Matrix::createIdentity(size_t numDimensions)
{
	Matrix identity = createZero(numDimensions, numDimensions);

	for (size_t d = 0; d < numDimensions; d++)
	{
		identity.setCell(d, d, 1.0);
	}

	return identity;
}

// Private members

void Matrix::destroyResource()
{
	if (matrixPtr != nullptr)
	{
		delete matrixPtr;
	}
}
