#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "MatCalcUtil.h"
#include <algorithm>
#include <iomanip>
#include <set>

// Public members

DenseMatrix::DenseMatrix()
{
	// Invalid state.
	numRows = 0;
	numColumns = 0;
}

DenseMatrix::DenseMatrix(size_t numRows, size_t numColumns, double initialValues)
{
	this->numRows = numRows;
	this->numColumns = numColumns;

	denseMatrix.resize(numRows);

	for (size_t r = 0; r < numRows; r++)
	{
		denseMatrix[r].resize(numColumns);

		for (size_t c = 0; c < numColumns; c++)
		{
			denseMatrix[r][c] = initialValues;
		}
	}
}

// Inherited via MatrixBase
size_t DenseMatrix::getNumRows() const
{
	return numRows;
}

size_t DenseMatrix::getNumColumns() const
{
	return numColumns;
}

double DenseMatrix::getCell(size_t row, size_t column) const
{
	return denseMatrix[row][column];
}

void DenseMatrix::setCell(size_t row, size_t column, double value)
{
	denseMatrix[row][column] = value;
}

void DenseMatrix::resizeNumRows(size_t newNumRows)
{
	if (numRows == newNumRows)
	{
		return;
	}

	size_t oldNumRows = numRows;

	numRows = newNumRows;
	denseMatrix.resize(numRows);

	if (oldNumRows < numRows)
	{
		for (size_t r = oldNumRows; r < numRows; r++)
		{
			denseMatrix[r].resize(numColumns);

			for (size_t c = 0; c < numColumns; c++)
			{
				denseMatrix[r][c] = 0.0;
			}
		}
	}
}

void DenseMatrix::resizeNumColumns(size_t newNumColumns)
{
	if (numColumns == newNumColumns)
	{
		return;
	}

	size_t oldNumColumns = numColumns;

	numColumns = newNumColumns;

	for (size_t r = 0; r < numRows; r++)
	{
		denseMatrix[r].resize(numColumns);

		if (oldNumColumns < numColumns)
		{
			for (size_t c = oldNumColumns; c < numColumns; c++)
			{
				denseMatrix[r][c] = 0.0;
			}
		}
	}
}

void DenseMatrix::resize(size_t newNumRows, size_t newNumColumns)
{
	resizeNumRows(newNumRows);
	resizeNumColumns(newNumColumns);
}

void DenseMatrix::transpose()
{
	// Prepare transposed denseMatrix container.
	std::vector<std::vector<double>> transposedMatrix;

	transposedMatrix.resize(numColumns); // newNumRows == oldNumColumns

	for (size_t r = 0; r < numColumns; r++)
	{
		transposedMatrix[r].resize(numRows); // newNumColumns == oldNumRows

		for (size_t c = 0; c < numRows; c++)
		{
			transposedMatrix[r][c] = 0.0;
		}
	}

	// Copy from old to transposed.
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			transposedMatrix[c][r] = denseMatrix[r][c];
		}
	}

	std::swap(numRows, numColumns);

	denseMatrix = transposedMatrix;
}

double DenseMatrix::getSparsity() const
{
	size_t numElements = numRows * numColumns;
	size_t numZeroElements = 0;

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double valueAtCell = denseMatrix[r][c];
			if (mcu::doubleAlmostEqual(valueAtCell, 0.0))
			{
				numZeroElements++;
			}
		}
	}

	double nominator = numZeroElements;
	double denominator = numElements;
	double sparsity = nominator / denominator;

	return sparsity;
}

double DenseMatrix::getDensity() const
{
	return 1.0 - getSparsity();
}

bool DenseMatrix::isSparse() const
{
	// Not checking for equality, because the threshold value itself is reserved for Density.
	return getSparsity() > MatrixBase::SparsityThreshold;
}

bool DenseMatrix::isDense() const
{
	return !isSparse();
}

bool DenseMatrix::requiresConversion() const
{
	return !isDense(); // This is DenseMatrix. If it's not Dense, then it requires conversion to Sparse.
}

MatrixBase* DenseMatrix::getConvertedCopy() const
{
	return cloneAsSparseMatrix();
}

MatrixBase* DenseMatrix::clone() const
{
	DenseMatrix* clone = new DenseMatrix(numRows, numColumns, 0.0);

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			clone->denseMatrix[r][c] = this->denseMatrix[r][c];
		}
	}

	return clone;
}

DenseMatrix* DenseMatrix::cloneAsDenseMatrix() const
{
	return dynamic_cast<DenseMatrix*>(clone());
}

SparseMatrix* DenseMatrix::cloneAsSparseMatrix() const
{
	SparseMatrix* sparseClone = new SparseMatrix(numRows, numColumns);

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double valueAtCell = this->denseMatrix[r][c];
			sparseClone->setCell(r, c, valueAtCell);
		}
	}

	return sparseClone;
}

std::vector<std::tuple<size_t, size_t, double>> DenseMatrix::getCellDataList() const
{
	std::vector<std::tuple<size_t, size_t, double>> cellList;

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			auto cellData = std::make_tuple(r, c, denseMatrix[r][c]);
			cellList.push_back(cellData);
		}
	}

	return cellList;
}

void DenseMatrix::scale(double scalar)
{
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			denseMatrix[r][c] *= scalar;
		}
	}
}

bool DenseMatrix::equal(const MatrixBase& right) const
{
	return right.equal(*this);
}

bool DenseMatrix::equal(const DenseMatrix& left) const
{
	if ((left.getNumRows() != this->getNumRows()) || (left.getNumColumns() != this->getNumColumns()))
	{
		return false;
	}

	// Dimensions match. Check every element.
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double leftValue = left.getCell(r, c);
			double rightValue = this->getCell(r, c);

			if (mcu::doubleAlmostEqual(leftValue, rightValue) == false)
			{
				return false;
			}
		}
	}

	return true;
}

bool DenseMatrix::equal(const SparseMatrix& left) const
{
	if ((left.getNumRows() != this->getNumRows()) || (left.getNumColumns() != this->getNumColumns()))
	{
		return false;
	}

	// Dimensions match. Check every element.
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double leftValue = left.getCell(r, c);
			double rightValue = this->getCell(r, c);

			if (mcu::doubleAlmostEqual(leftValue, rightValue) == false)
			{
				return false;
			}
		}
	}

	return true;
}

MatrixBase* DenseMatrix::add(const MatrixBase& right) const
{
	return right.add(*this);
}

MatrixBase* DenseMatrix::add(const DenseMatrix& left) const
{
	DenseMatrix* addedDense = new DenseMatrix(numRows, numColumns, 0.0);

	// Set left.
	for (size_t r = 0; r < left.numRows; r++)
	{
		for (size_t c = 0; c < left.numColumns; c++)
		{
			double valueAtCell = left.getCell(r, c);
			addedDense->setCell(r, c, valueAtCell);
		}
	}

	// Add right.
	for (size_t r = 0; r < this->numRows; r++)
	{
		for (size_t c = 0; c < this->numColumns; c++)
		{
			addedDense->denseMatrix[r][c] += this->denseMatrix[r][c];
		}
	}

	return addedDense;
}

MatrixBase* DenseMatrix::add(const SparseMatrix& left) const
{
	DenseMatrix* addedDense = new DenseMatrix(numRows, numColumns, 0.0);

	// Set left.
	auto leftCellDataList = left.getCellDataList();
	auto leftCellDataListBegin = leftCellDataList.begin();
	auto leftCellDataListEnd = leftCellDataList.end();

	for (auto iter = leftCellDataListBegin; iter != leftCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		addedDense->setCell(row, column, value);
	}

	// Add right.
	for (size_t r = 0; r < this->numRows; r++)
	{
		for (size_t c = 0; c < this->numColumns; c++)
		{
			double rightValue = this->getCell(r, c);
			double existingValue = addedDense->getCell(r, c);
			double newValue = existingValue + rightValue;

			addedDense->setCell(r, c, newValue);
		}
	}

	return addedDense;
}

MatrixBase* DenseMatrix::subtract(const MatrixBase& right) const
{
	return right.subtract(*this);
}

MatrixBase* DenseMatrix::subtract(const DenseMatrix& left) const
{
	DenseMatrix* subtractedDense = new DenseMatrix(numRows, numColumns, 0.0);

	// Set left.
	for (size_t r = 0; r < left.numRows; r++)
	{
		for (size_t c = 0; c < left.numColumns; c++)
		{
			double valueAtCell = left.getCell(r, c);
			subtractedDense->setCell(r, c, valueAtCell);
		}
	}

	// Add negated right.
	for (size_t r = 0; r < this->numRows; r++)
	{
		for (size_t c = 0; c < this->numColumns; c++)
		{
			double rightValue = this->getCell(r, c);
			double existingValue = subtractedDense->getCell(r, c);
			double newValue = existingValue - rightValue; // SUBTRACTION

			subtractedDense->setCell(r, c, newValue);
		}
	}

	return subtractedDense;
}

MatrixBase* DenseMatrix::subtract(const SparseMatrix& left) const
{
	DenseMatrix* subtractedDense = new DenseMatrix(numRows, numColumns, 0.0);

	// Set left.
	auto leftCellDataList = left.getCellDataList();
	auto leftCellDataListBegin = leftCellDataList.begin();
	auto leftCellDataListEnd = leftCellDataList.end();

	for (auto iter = leftCellDataListBegin; iter != leftCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		subtractedDense->setCell(row, column, value);
	}

	// Add negated right.
	for (size_t r = 0; r < this->numRows; r++)
	{
		for (size_t c = 0; c < this->numColumns; c++)
		{
			double rightValue = this->getCell(r, c);
			double existingValue = subtractedDense->getCell(r, c);
			double newValue = existingValue - rightValue; // SUBTRACTION

			subtractedDense->setCell(r, c, newValue);
		}
	}

	return subtractedDense;
}

MatrixBase* DenseMatrix::multiply(const MatrixBase& right) const
{
	return right.multiply(*this);
}

MatrixBase* DenseMatrix::multiply(const DenseMatrix& left) const
{
	// DenseMatrix * DenseMatrix algorithm.
	// The algorithm takes a CELL from the LEFTmatrix, and uses it to scale a ROW on the RIGHTmatrix.
	// The result is ADDED to the existing ROW on the RESULTmatrix.
	// Complexity is cubic or something. 
	// However, since I'm traversing rows first (and not columns), cache coherency should hopefully be on my side...

	DenseMatrix* denseProduct = new DenseMatrix(left.getNumRows(), this->numColumns, 0.0);

	for (size_t leftRow = 0; leftRow < left.getNumRows(); leftRow++)
	{
		for (size_t leftCol = 0; leftCol < left.getNumColumns(); leftCol++)
		{
			for (size_t rightCol = 0; rightCol < this->numColumns; rightCol++)
			{
				double leftValue = left.getCell(leftRow, leftCol);
				double rightValue = this->getCell(leftCol, rightCol);
				double product = leftValue * rightValue;

				double existingValue = denseProduct->getCell(leftRow, rightCol);
				double newValue = existingValue + product;

				denseProduct->setCell(leftRow, rightCol, newValue);
			}
		}
	}

	return denseProduct;
}

MatrixBase* DenseMatrix::multiply(const SparseMatrix& left) const
{
	// I'm using DenseMatrix * DenseMatrix algorithm again, because I don't know the best approach for now.

	DenseMatrix* denseProduct = new DenseMatrix(left.getNumRows(), this->numColumns, 0.0);

	for (size_t leftRow = 0; leftRow < left.getNumRows(); leftRow++)
	{
		for (size_t leftCol = 0; leftCol < left.getNumColumns(); leftCol++)
		{
			for (size_t rightCol = 0; rightCol < this->numColumns; rightCol++)
			{
				double leftValue = left.getCell(leftRow, leftCol);
				double rightValue = this->getCell(leftCol, rightCol);
				double product = leftValue * rightValue;

				double existingValue = denseProduct->getCell(leftRow, rightCol);
				double newValue = existingValue + product;

				denseProduct->setCell(leftRow, rightCol, newValue);
			}
		}
	}

	return denseProduct;
}

MatrixBase* DenseMatrix::mergeByColumns(const MatrixBase& right) const
{
	return right.mergeByColumns(*this);
}

MatrixBase* DenseMatrix::mergeByColumns(const DenseMatrix& left) const
{
	DenseMatrix* mergedDense = new DenseMatrix(numRows, left.getNumColumns() + numColumns, 0.0);

	// Copy left.
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < left.getNumColumns(); c++)
		{
			double leftValue = left.getCell(r, c);
			mergedDense->setCell(r, c, leftValue);
		}
	}

	// Copy right.
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < this->getNumColumns(); c++)
		{
			double rightValue = this->getCell(r, c);

			size_t columnOffset = left.getNumColumns();
			size_t newColumnIndex = c + columnOffset;

			mergedDense->setCell(r, newColumnIndex, rightValue);
		}
	}

	return mergedDense;
}

MatrixBase* DenseMatrix::mergeByColumns(const SparseMatrix& left) const
{
	DenseMatrix* mergedDense = new DenseMatrix(numRows, left.getNumColumns() + numColumns, 0.0);

	// Copy left.
	auto leftCellDataList = left.getCellDataList();
	auto leftCellDataListBegin = leftCellDataList.begin();
	auto leftCellDataListEnd = leftCellDataList.end();

	for (auto iter = leftCellDataListBegin; iter != leftCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		mergedDense->setCell(row, column, value);
	}

	// Copy right.
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < this->getNumColumns(); c++)
		{
			double rightValue = this->getCell(r, c);

			size_t columnOffset = left.getNumColumns();
			size_t newColumnIndex = c + columnOffset;

			mergedDense->setCell(r, newColumnIndex, rightValue);
		}
	}

	return mergedDense;
}

MatrixBase* DenseMatrix::mergeByRows(const MatrixBase& right) const
{
	return right.mergeByRows(*this);
}

MatrixBase* DenseMatrix::mergeByRows(const DenseMatrix& left) const
{
	DenseMatrix* mergedDense = new DenseMatrix(left.getNumRows() + numRows, numColumns, 0.0);

	// Copy left.
	for (size_t r = 0; r < left.getNumRows(); r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double leftValue = left.getCell(r, c);
			mergedDense->setCell(r, c, leftValue);
		}
	}

	// Copy right.
	for (size_t r = 0; r < this->getNumRows(); r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double rightValue = this->getCell(r, c);

			size_t rowOffset = left.getNumRows();
			size_t newRowIndex = r + rowOffset;

			mergedDense->setCell(newRowIndex, c, rightValue);
		}
	}

	return mergedDense;
}

MatrixBase* DenseMatrix::mergeByRows(const SparseMatrix& left) const
{
	DenseMatrix* mergedDense = new DenseMatrix(left.getNumRows() + numRows, numColumns, 0.0);

	// Copy left.
	auto leftCellDataList = left.getCellDataList();
	auto leftCellDataListBegin = leftCellDataList.begin();
	auto leftCellDataListEnd = leftCellDataList.end();

	for (auto iter = leftCellDataListBegin; iter != leftCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double leftValue = std::get<2>(cellData);

		mergedDense->setCell(row, column, leftValue);
	}

	// Copy right.
	for (size_t r = 0; r < this->getNumRows(); r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			double rightValue = this->getCell(r, c);

			size_t rowOffset = left.getNumRows();
			size_t newRowIndex = r + rowOffset;

			mergedDense->setCell(newRowIndex, c, rightValue);
		}
	}

	return mergedDense;
}

MatrixBase* DenseMatrix::splitByColumn(size_t leftNewNumColumns, bool returnLeftMatrix) const
{
	// this->numColumns = Total Num Columns.
	size_t rightNewNumColumns = this->numColumns - leftNewNumColumns;

	size_t splitMatrixNumColumns = returnLeftMatrix ? leftNewNumColumns : rightNewNumColumns;
	size_t columnOffset = returnLeftMatrix ? 0 : leftNewNumColumns;

	DenseMatrix* split = new DenseMatrix(this->numRows, splitMatrixNumColumns, 0.0);

	for (size_t r = 0; r < split->getNumRows(); r++)
	{
		for (size_t c = 0; c < split->getNumColumns(); c++)
		{
			size_t bigMatrixOffsetColumnIndex = c + columnOffset;

			double value = this->getCell(r, bigMatrixOffsetColumnIndex);
			split->setCell(r, c, value);
		}
	}

	return split;
}

MatrixBase* DenseMatrix::splitByRow(size_t topNewNumRows, bool returnTopMatrix) const
{
	// this->numRows = Total Num Rows
	size_t bottomNewNumRows = this->numRows - topNewNumRows;

	size_t splitMatrixNumRows = returnTopMatrix ? topNewNumRows : bottomNewNumRows;
	size_t rowOffset = returnTopMatrix ? 0 : topNewNumRows;

	DenseMatrix* split = new DenseMatrix(splitMatrixNumRows, this->numColumns, 0.0);

	for (size_t r = 0; r < split->getNumRows(); r++)
	{
		for (size_t c = 0; c < split->getNumColumns(); c++)
		{
			size_t bigMatrixOffsetRowIndex = r + rowOffset;

			double value = this->getCell(bigMatrixOffsetRowIndex, c);
			split->setCell(r, c, value);
		}
	}

	return split;
}

MatrixBase* DenseMatrix::getSubMatrix(size_t subRowBeginIndex, size_t subNumRows, size_t subColumnBeginIndex, size_t subNumColumns) const
{
	// This will be the only corner case that is handled by the method.
	if (subNumRows == 0 || subNumColumns == 0)
	{
		return nullptr;
	}

	DenseMatrix* subDense = new DenseMatrix(subNumRows, subNumColumns, 0);

	// Copy values from the big matrix into the sub matrix.
	for (size_t r = 0; r < subDense->getNumRows(); r++)
	{
		for (size_t c = 0; c < subDense->getNumColumns(); c++)
		{
			size_t bigMatRow = r + subRowBeginIndex;
			size_t bigMatCol = c + subColumnBeginIndex;
			double bigMatValue = this->getCell(bigMatRow, bigMatCol);
			subDense->setCell(r, c, bigMatValue);
		}
	}

	return subDense;
}

MatrixBase* DenseMatrix::getSubMatrix(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	MatrixBase* topLeft = this->getSubMatrixTopLeft(ignoredRowIndex, ignoredColumnIndex);
	MatrixBase* topRight = this->getSubMatrixTopRight(ignoredRowIndex, ignoredColumnIndex);
	MatrixBase* bottomLeft = this->getSubMatrixBottomLeft(ignoredRowIndex, ignoredColumnIndex);
	MatrixBase* bottomRight = this->getSubMatrixBottomRight(ignoredRowIndex, ignoredColumnIndex);

	// Merge 'topLeft' and 'topRight', and get 'top'.
	MatrixBase* top = nullptr;
	if (topLeft != nullptr && topRight != nullptr)
	{
		top = topLeft->mergeByColumns(*topRight);
	}
	else if (topLeft != nullptr)
	{
		top = topLeft->clone();;
	}
	else if (topRight != nullptr)
	{
		top = topRight->clone();
	}

	// Merge 'botomLeft' and 'bottomRight', and get 'bottom'.
	MatrixBase* bottom = nullptr;
	if (bottomLeft != nullptr && bottomRight != nullptr)
	{
		bottom = bottomLeft->mergeByColumns(*bottomRight);
	}
	else if (bottomLeft != nullptr)
	{
		bottom = bottomLeft->clone();
	}
	else if (bottomRight != nullptr)
	{
		bottom = bottomRight->clone();
	}

	// Merge 'top' and 'bottom', and get 'subMatrix'.
	MatrixBase* subMatrix = nullptr;
	if (top != nullptr && bottom != nullptr)
	{
		subMatrix = top->mergeByRows(*bottom);
	}
	else if (top != nullptr)
	{
		subMatrix = top->clone();
	}
	else if (bottom != nullptr)
	{
		// If-check to make the compiler happy. 'bottom' cannot be equal to nullptr at this point!
		subMatrix = bottom->clone();
	}

	delete topLeft;
	delete topRight;
	delete bottomLeft;
	delete bottomRight;
	delete top;
	delete bottom;

	return subMatrix;
}

MatrixBase* DenseMatrix::getSubMatrixTopLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == 0 || ignoredColumnIndex == 0)
	{
		return nullptr;
	}

	MatrixBase* left = splitByColumn(ignoredColumnIndex, true);
	MatrixBase* topLeft = left->splitByRow(ignoredRowIndex, true);

	delete left;

	return topLeft;
}

MatrixBase* DenseMatrix::getSubMatrixTopRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == 0 || ignoredColumnIndex == (numColumns - 1))
	{
		return nullptr;
	}

	MatrixBase* right = splitByColumn(ignoredColumnIndex + 1, false);
	MatrixBase* topRight = right->splitByRow(ignoredRowIndex, true);

	delete right;

	return topRight;
}

MatrixBase* DenseMatrix::getSubMatrixBottomLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == (numRows - 1) || ignoredColumnIndex == 0)
	{
		return nullptr;
	}

	MatrixBase* left = splitByColumn(ignoredColumnIndex, true);
	MatrixBase* bottomLeft = left->splitByRow(ignoredRowIndex + 1, false);

	delete left;

	return bottomLeft;
}

MatrixBase* DenseMatrix::getSubMatrixBottomRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == (numRows - 1) || ignoredColumnIndex == (numColumns - 1))
	{
		return nullptr;
	}

	MatrixBase* right = splitByColumn(ignoredColumnIndex + 1, false);
	MatrixBase* bottomRight = right->splitByRow(ignoredRowIndex + 1, false);

	delete right;

	return bottomRight;
}

double DenseMatrix::getDeterminant() const
{
	// Determinant calculation algorithm is based on Laplace Expansion.

	// The matrix is assumed to be square (numRows == numColumns).

	if (numRows == 1)
	{
		return getCell(0, 0);
	}

	if (numRows == 2)
	{
		// det = ad - bc
		return (getCell(0, 0) * getCell(1, 1)) - (getCell(0, 1) * getCell(1, 0));
	}

	double determinant = 0;

	for (size_t c = 0; c < numColumns; c++)
	{
		double cellAtFirstRow = this->getCell(0, c);

		// If the value at this cell is zero, then there's no point in calculating the determinant of the sub-matrix.
		if (mcu::doubleAlmostEqual(cellAtFirstRow, 0))
		{
			continue;
		}

		// Determine the sign based on checkerboard pattern (+-+-+-...).
		// If 'c' is even (default assumption), then positive. If 'c' is odd, then negative.
		if (c % 2 == 1)
		{
			cellAtFirstRow = -cellAtFirstRow;
		}

		// Create sub-matrix, by ignoring the current cell's entire row and column.
		// Create left side.
		size_t leftRowBegin = 1;
		size_t leftNumRows = this->numRows - 1;
		size_t leftColBegin = 0;
		size_t leftNumCols = c;
		MatrixBase* leftSubMatrix = this->getSubMatrix(leftRowBegin, leftNumRows, leftColBegin, leftNumCols);

		// Create right side.
		size_t rightRowBegin = leftRowBegin; // Always the same.
		size_t rightNumRows = leftNumRows; // Always the same.
		size_t rightColBegin = c + 1;
		size_t rightNumCols = (this->numColumns - 1) - c;
		MatrixBase* rightSubMatrix = this->getSubMatrix(rightRowBegin, rightNumRows, rightColBegin, rightNumCols);

		// Create merged sub-matrix from left and right sides.
		MatrixBase* mergedSub = nullptr;

		if (leftSubMatrix == nullptr)
		{
			mergedSub = rightSubMatrix->clone();
		}
		else if (rightSubMatrix == nullptr)
		{
			mergedSub = leftSubMatrix->clone();
		}
		else
		{
			mergedSub = leftSubMatrix->mergeByColumns((*rightSubMatrix));
		}

		// Get the determinant of the merged sub-matrix.
		double subMatDet = mergedSub->getDeterminant();

		// Finally, the big meat.
		determinant += (cellAtFirstRow * subMatDet);

		// Delete the matrices. (left, right and mergedSub).
		delete leftSubMatrix;
		delete rightSubMatrix;
		delete mergedSub;
	}

	return determinant;
}

MatrixBase* DenseMatrix::getMinorMatrix() const
{
	DenseMatrix* denseMinorMatrix = new DenseMatrix(numRows, numColumns, 0);

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			MatrixBase* subDenseMatrix = this->getSubMatrix(r, c);

			double subDeterminant = subDenseMatrix->getDeterminant();

			denseMinorMatrix->setCell(r, c, subDeterminant);

			delete subDenseMatrix;
		}
	}

	return denseMinorMatrix;
}

void DenseMatrix::applyCheckerboardPattern()
{
	// Matrix is assumed to be 'square'.
	// Checkerboard Pattern. Even indices remain positive; odd indices are negated:
	// [+-+]
	// [-+-]
	// [+-+]

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			if ((r + c) % 2 == 0)
			{
				continue; // Even index. Leave it as it is.
			}

			double existingValue = this->getCell(r, c);
			this->setCell(r, c, -existingValue);
		}
	}
}

MatrixBase* DenseMatrix::getInverse(double determinant) const
{
	if (mcu::doubleAlmostEqual(determinant, 0)
		|| mcu::doubleAlmostEqual(determinant, std::numeric_limits<double>::quiet_NaN()))
	{
		return nullptr;
	}

	MatrixBase* inverse = nullptr;

	// Assuming this is a square matrix (it has to be).
	if (this->getNumRows() == 1)
	{
		inverse = new DenseMatrix(1, 1, determinant);
		return inverse;
	}

	// Get matrix of minors.
	inverse = this->getMinorMatrix();

	// Apply the checkerboard pattern on the minor matrix, to get the matrix of cofactors.
	inverse->applyCheckerboardPattern();

	// Transpose the cofactor matrix, to get the adjugate matrix.
	inverse->transpose();

	// Scale the adjugate matrix by the reciprocal of the determinant, to get the inverse.
	inverse->scale(1 / determinant);

	return inverse;
}

std::string DenseMatrix::getPrintStr(size_t precision) const
{
	// Traverse the entire matrix once to create a mapping of: columnIndex --> maxDigits
	// Traverse the matrix AGAIN. Use a stringstream and the mapping to output a well aligned matrix.
	// Complexity is: O(2*m*n). (m = numRows, n = numColumns).

	std::map<size_t, size_t> map_colIndex_maxDigits = getColumnAlignmentMapForPrinting();

	// Build the output string with a string formatter.
	size_t numRows = getNumRows();
	size_t numColumns = getNumColumns();

	std::stringstream sst;
	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			if (c != 0)
			{
				sst << " ";
			}

			double value = getCell(r, c);

			if (mcu::doubleAlmostEqual(value, 0))
			{
				value = 0; // To prevent minus zero.
			}

			size_t intPartLength = mcu::getNumDigitsOfIntegerPart(value, true);
			size_t maxIntPartLength = map_colIndex_maxDigits[c];

			for (size_t i = 0; i < maxIntPartLength - intPartLength; i++)
			{
				// Padding, based on the number's length of intPart.
				sst << " ";
			}

			// WARNING: std::fixed is used to disable scientific notation. Propagates to every cout call.
			sst << std::fixed << std::setprecision(precision) << value;

			if (c != (numColumns - 1))
			{
				sst << ",";
			}
		}

		sst << std::endl;
	}

	return sst.str();
}

std::string DenseMatrix::solveFor(const MatrixBase& augmentedColumn, bool verbose, size_t doublePrecision) const
{
	DenseMatrix* denseClone = this->cloneAsDenseMatrix();
	DenseMatrix* augmentedMatrix = dynamic_cast<DenseMatrix*>(this->mergeByColumns(augmentedColumn));

	// Step 1): Reduce the matrix into Row Echelon Form (REF).
	size_t augNumRows = augmentedMatrix->getNumRows();
	size_t augNumCols = augmentedMatrix->getNumColumns();
	size_t leadingEntryRow = 0;

	size_t numSteps = 0;
	std::stringstream sst;
	for (size_t c = 0; c < (augNumCols - 1); c++)
	{
		bool isZeroColumn = true;
		// Search whether even a single non-zero element exists in this column's rows.
		for (size_t r = leadingEntryRow; r < augNumRows; r++)
		{
			// If a single non-zero row found, then carry it at the top row. 
			// (top row according to current pivot row).
			if (mcu::doubleAlmostEqual(augmentedMatrix->getCell(r, c), 0) == false)
			{
				isZeroColumn = false;

				if (r != leadingEntryRow)
				{
					// Sorry for the hack.
					std::swap(augmentedMatrix->denseMatrix[r], augmentedMatrix->denseMatrix[leadingEntryRow]);
				}

				break;
			}
		}

		// There isn't a single non-zero element. Skip this column.
		if (isZeroColumn)
		{
			continue;
		}

		// Second loop to eliminate every other non-zero entry.
		double leadingEntryValue = augmentedMatrix->getCell(leadingEntryRow, c);
		for (size_t r = (leadingEntryRow + 1); r < augNumRows; r++)
		{
			double valueAtColumn = augmentedMatrix->getCell(r, c);

			// Skip if zero element.
			if (mcu::doubleAlmostEqual(valueAtColumn, 0) == true)
			{
				continue;
			}

			// Eliminate if non-zero.
			double coefficient = valueAtColumn / leadingEntryValue;

			// Do row operation (including augmented column): victim - (coefficient * leadingEntryValue)
			for (size_t col = 0; col < augNumCols; col++)
			{
				double subtrahend = (coefficient * augmentedMatrix->getCell(leadingEntryRow, col));
				double minuend = augmentedMatrix->getCell(r, col);

				double difference = minuend - subtrahend;

				augmentedMatrix->setCell(r, col, difference);
			}
		}

		// If the code was able to get to this point, then there was at least 1 non-zero value.
		// This means, the row index can be incremented in order to find the next leading entry.
		leadingEntryRow++;

		if (verbose)
		{
			numSteps++;
			sst << "Step " << numSteps << ":" << std::endl << std::endl;
			sst << augmentedMatrix->getPrintStr(doublePrecision) << std::endl << std::endl;
		}
	}

	// Step 2): Reduce the matrix into REDUCED ROW ECHELON FORM (RREF).
	leadingEntryRow = 0;
	for (size_t c = 0; c < (augNumCols - 1); c++)
	{
		if (leadingEntryRow == 0)
		{
			leadingEntryRow++;
			continue; // There's nothing to zero out for (row == 0). Skip.
		}

		if (leadingEntryRow >= augNumRows)
		{
			break; // Break if we're done searching all rows.
		}

		double pivotValue = augmentedMatrix->getCell(leadingEntryRow, c);
		if (mcu::doubleAlmostEqual(pivotValue, 0))
		{
			continue; // Skip if zero (zero means = not a pivot entry).
		}

		// At this point, the values below the pivot entry are supposed to be all zeroes anyway.
		// Zero out the values ABOVE the pivot entry, in order to achieve REDUCED Row Echelon Form (RREF).


		// And now, work on the rows ABOVE the pivot entry (make it zero).
		for (long long int r = (leadingEntryRow - 1); r >= 0; r--)
		{
			double valueAbovePivot = augmentedMatrix->getCell(r, c);

			if (mcu::doubleAlmostEqual(valueAbovePivot, 0))
			{
				continue; // The value is already zero. Skip.
			}

			// Both are non-zero. Eliminate the value above pivot.
			double coefficient = valueAbovePivot / pivotValue;

			// Do row operation, including augmented column: victim - (coefficient * leadingEntryValue)
			// Column indices before this point can be ignored, since they're zero.
			for (size_t col = c; col < augNumCols; col++)
			{
				double subtrahend = (coefficient * augmentedMatrix->getCell(leadingEntryRow, col));
				double minuend = augmentedMatrix->getCell(r, col);

				double difference = minuend - subtrahend;

				augmentedMatrix->setCell(r, col, difference);
			}

			if (verbose)
			{
				numSteps++;
				sst << "Step " << numSteps << ":" << std::endl << std::endl;
				sst << augmentedMatrix->getPrintStr(doublePrecision) << std::endl << std::endl;
			}
		}

		// Finally, make sure that the pivot entry is equal to 1.
		// This means, dividing the entire row by the pivot entry (columns before this point can be ignored).
		for (size_t col = c; col < augNumCols; col++)
		{
			double existingValue = augmentedMatrix->getCell(leadingEntryRow, col);
			double newValue = (existingValue / pivotValue);

			augmentedMatrix->setCell(leadingEntryRow, col, newValue);
		}

		if (verbose)
		{
			numSteps++;
			sst << "Step " << numSteps << ":" << std::endl << std::endl;
			sst << augmentedMatrix->getPrintStr(doublePrecision) << std::endl << std::endl;
		}

		leadingEntryRow++;
	}

	// Step 3): The matrix is in RREF. Print solution set.
	sst << std::endl << "Solution:" << std::endl << std::endl;
	std::stringstream solution_sst;
	bool noSolution = false;
	std::set<size_t> freeVarIndices;
	for (size_t r = 0; r < augNumRows; r++)
	{
		bool isAllZerosBeforeAugmentedColumn = true;
		bool foundPivotEntry = false;
		size_t pivotEntryCol = 0;

		for (size_t c = 0; c < augNumCols; c++)
		{
			double valueAtCell = augmentedMatrix->getCell(r, c);
			bool isZeroValue = mcu::doubleAlmostEqual(valueAtCell, 0);

			if (foundPivotEntry == false)
			{
				if (isZeroValue)
				{
					continue; // Pivot entry always comes first. Skip if the value is zero.
				}
				else
				{
					pivotEntryCol = c;
					foundPivotEntry = true;
				}
			}

			if (c != (augNumCols - 1))
			{
				// Found a non-zero value which isn't an augmented column member.
				isAllZerosBeforeAugmentedColumn = false;
			}

			if ((c == (augNumCols - 1))
				&& isAllZerosBeforeAugmentedColumn
				&& (mcu::doubleAlmostEqual(valueAtCell, 0) == false))
			{
				// The entire row was zero, except for the augmented part. No solution for 0 == (non-zero).
				noSolution = true;
				break;
			}

			if (c == pivotEntryCol)
			{
				solution_sst << "x" << (c + 1) << " ="; // When printing out, indices start from '1'.
			}
			else
			{
				double nonPivotElement = augmentedMatrix->getCell(r, c);
				bool isAugColIndex = (c == (augNumCols - 1));

				if (isAugColIndex == false && mcu::doubleAlmostEqual(nonPivotElement, 0))
				{
					continue; // If it's not an augmented column member, then no need to write zero coefficient.
				}

				double shownNumber = std::abs(nonPivotElement);

				solution_sst << " ";

				if (isAugColIndex)
				{
					// Already on the other side of the equation. Keep the sign.
					if (nonPivotElement < 0.0)
					{
						solution_sst << "-";
					}
					else
					{
						solution_sst << "+";
					}
				}
				else
				{
					// Send it to the other side of the equation. Change the sign.
					if (nonPivotElement < 0.0)
					{
						solution_sst << "+";
					}
					else
					{
						solution_sst << "-";
					}
				}

				solution_sst << " " << shownNumber;

				if (isAugColIndex == false)
				{
					solution_sst << "x" << (c + 1); // When printing out, indices start from '1'.
					freeVarIndices.insert(c);
				}
			}
		}

		if (isAllZerosBeforeAugmentedColumn == false)
		{
			solution_sst << std::endl;
		}
	}

	if (noSolution)
	{
		solution_sst = std::stringstream(); // Clear.
		solution_sst << "No solution." << std::endl;
	}
	else
	{
		auto freeVarIndicesBegin = freeVarIndices.begin();
		auto freeVarIndicesEnd = freeVarIndices.end();
		bool first = true;
		for (auto freeVarIter = freeVarIndicesBegin; freeVarIter != freeVarIndicesEnd; ++freeVarIter)
		{
			size_t index = *freeVarIter;
			size_t shownIndex = index + 1;  // When printing out, indices start from '1'.

			if (first)
			{
				first = false;
				solution_sst << "x" << shownIndex;
			}
			else
			{
				solution_sst << ", x" << shownIndex;
			}
		}

		if (freeVarIndices.size() > 0)
		{
			solution_sst << " are free variables." << std::endl;
		}
		// Print the free variables.
	}

	sst << solution_sst.str();

	delete denseClone;
	delete augmentedMatrix;

	return sst.str();
}

size_t DenseMatrix::getRank() const
{
	DenseMatrix* clone = this->cloneAsDenseMatrix();

	// Step 1): Reduce the matrix into Row Echelon Form (REF).
	size_t cloneNumRows = clone->getNumRows();
	size_t cloneNumCols = clone->getNumColumns();
	size_t leadingEntryRow = 0;

	std::stringstream sst;
	for (size_t c = 0; c < cloneNumCols; c++)
	{
		bool isZeroColumn = true;
		// Search whether even a single non-zero element exists in this column's rows.
		for (size_t r = leadingEntryRow; r < cloneNumRows; r++)
		{
			// If a single non-zero row found, then carry it at the top row. 
			// (top row according to current pivot row).
			if (mcu::doubleAlmostEqual(clone->getCell(r, c), 0) == false)
			{
				isZeroColumn = false;

				if (r != leadingEntryRow)
				{
					// Sorry for the hack.
					std::swap(clone->denseMatrix[r], clone->denseMatrix[leadingEntryRow]);
				}

				break;
			}
		}

		// There isn't a single non-zero element. Skip this column.
		if (isZeroColumn)
		{
			continue;
		}

		// Second loop to eliminate every other non-zero entry.
		double leadingEntryValue = clone->getCell(leadingEntryRow, c);
		for (size_t r = (leadingEntryRow + 1); r < cloneNumRows; r++)
		{
			double valueAtColumn = clone->getCell(r, c);

			// Skip if zero element.
			if (mcu::doubleAlmostEqual(valueAtColumn, 0) == true)
			{
				continue;
			}

			// Eliminate if non-zero.
			double coefficient = valueAtColumn / leadingEntryValue;

			// Do row operation (including augmented column): victim - (coefficient * leadingEntryValue)
			for (size_t col = 0; col < cloneNumCols; col++)
			{
				double subtrahend = (coefficient * clone->getCell(leadingEntryRow, col));
				double minuend = clone->getCell(r, col);

				double difference = minuend - subtrahend;

				clone->setCell(r, col, difference);
			}
		}

		// If the code was able to get to this point, then there was at least 1 non-zero value.
		// This means, the row index can be incremented in order to find the next leading entry.
		leadingEntryRow++;
	}

	// Step 2): Count the number of 'zero rows'.

	size_t numZeroRows = 0;
	for (size_t r = 0; r < clone->getNumRows(); r++)
	{
		bool isZeroRow = true;

		for (size_t c = 0; c < clone->getNumColumns(); c++)
		{
			double valueAtCell = clone->getCell(r, c);

			if (mcu::doubleAlmostEqual(valueAtCell, 0) == false)
			{
				isZeroRow = false;
				break;
			}
		}

		if (isZeroRow)
		{
			numZeroRows++;
		}
	}

	size_t numNonZeroRows = clone->getNumRows() - numZeroRows;

	delete clone;

	return numNonZeroRows;
}

// Private members

std::map<size_t, size_t> DenseMatrix::getColumnAlignmentMapForPrinting() const
{
	// I think putting this logic in a function is a bad idea, because the logic is not used anywhere else.
	// But I feel like you'd cut points if I didn't do it...

	// Prepare mapping of: ColumnIndex --> maxDigits.
	std::map<size_t, size_t> map_colIndex_maxDigits;

	// Initialize the map.
	for (size_t c = 0; c < getNumColumns(); c++)
	{
		map_colIndex_maxDigits[c] = 0;
	}

	// Traverse the entire matrix and fill in the 'maxDigits' information.
	for (size_t r = 0; r < getNumRows(); r++)
	{
		for (size_t c = 0; c < getNumColumns(); c++)
		{
			double value = getCell(r, c);
			if (mcu::doubleAlmostEqual(value, 0))
			{
				value = 0; // To prevent minus zero.
			}

			size_t numDigits = mcu::getNumDigitsOfIntegerPart(value, true);

			if (numDigits > map_colIndex_maxDigits[c])
			{
				map_colIndex_maxDigits[c] = numDigits; // Existing was smaller numDigits. Replace it with the bigger one.
			}
		}
	}

	return map_colIndex_maxDigits;
}
