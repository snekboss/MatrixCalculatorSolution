#include "SparseMatrix.h"
#include "DenseMatrix.h"
#include "MatCalcUtil.h"
#include <iomanip>
#include <limits> // Required by g++ (std::numeric_limits<double>)

// Public members

SparseMatrix::SparseMatrix()
{
	// Invalid state.
	numRows = 0;
	numColumns = 0;
}

SparseMatrix::SparseMatrix(size_t numRows, size_t numColumns)
{
	this->numRows = numRows;
	this->numColumns = numColumns;
}

// Inherited via MatrixBase
size_t SparseMatrix::getNumRows() const
{
	return numRows;
}

size_t SparseMatrix::getNumColumns() const
{
	return numColumns;
}

double SparseMatrix::getCell(size_t row, size_t column) const
{
	double ret = 0.0;
	std::pair<size_t, size_t> rowColumn = std::make_pair(row, column);

	auto iter = sparseMatrix.find(rowColumn);

	if (iter != sparseMatrix.end())
	{
		ret = (*iter).second;
	}

	return ret;
}

void SparseMatrix::setCell(size_t row, size_t column, double value)
{
	std::pair<size_t, size_t> rowColumn = std::make_pair(row, column);

	sparseMatrix[rowColumn] = value;

	if (mcu::doubleAlmostEqual(sparseMatrix[rowColumn], 0.0))
	{
		sparseMatrix.erase(rowColumn);
	}
}

void SparseMatrix::resizeNumRows(size_t newNumRows)
{
	if (numRows == newNumRows)
	{
		return;
	}

	size_t oldNumRows = numRows;

	numRows = newNumRows;

	if (oldNumRows > numRows)
	{
		std::vector < std::pair<size_t, size_t>> pairsToBeErased;

		auto mapBegin = sparseMatrix.begin();
		auto mapEnd = sparseMatrix.end();

		for (auto iter = mapBegin; iter != mapEnd; ++iter)
		{
			std::pair<size_t, size_t> rowColumn = (*iter).first;
			size_t row = rowColumn.first;

			// If the element's row coordinate is greaterOrEqual our newNumRows, mark it to erase later.
			if (row >= numRows)
			{
				pairsToBeErased.push_back(rowColumn);
			}
		}

		auto pairsToBeErasedBegin = pairsToBeErased.begin();
		auto pairsToBeErasedEnd = pairsToBeErased.end();

		for (auto iter = pairsToBeErasedBegin; iter != pairsToBeErasedEnd; ++iter)
		{
			sparseMatrix.erase((*iter));
		}
	}
}

void SparseMatrix::resizeNumColumns(size_t newNumColumns)
{
	if (numColumns == newNumColumns)
	{
		return;
	}

	size_t oldNumColumns = numColumns;

	numColumns = newNumColumns;

	if (oldNumColumns > numColumns)
	{
		std::vector < std::pair<size_t, size_t>> pairsToBeErased;

		auto mapBegin = sparseMatrix.begin();
		auto mapEnd = sparseMatrix.end();

		for (auto iter = mapBegin; iter != mapEnd; ++iter)
		{
			std::pair<size_t, size_t> rowColumn = (*iter).first;
			size_t column = rowColumn.second;

			// If the element's column coordinate is greaterOrEqual our newNumColumns, erase it.
			if (column >= numColumns)
			{
				pairsToBeErased.push_back(rowColumn);
			}
		}

		auto pairsToBeErasedBegin = pairsToBeErased.begin();
		auto pairsToBeErasedEnd = pairsToBeErased.end();

		for (auto iter = pairsToBeErasedBegin; iter != pairsToBeErasedEnd; ++iter)
		{
			sparseMatrix.erase((*iter));
		}
	}
}

void SparseMatrix::resize(size_t newNumRows, size_t newNumColumns)
{
	resizeNumRows(newNumRows);
	resizeNumColumns(newNumColumns);
}

void SparseMatrix::transpose()
{
	std::map<std::pair<size_t, size_t>, double> map_newSparseMatrix;

	auto oldMapBegin = sparseMatrix.begin();
	auto oldMapEnd = sparseMatrix.end();

	// Initialize a new std::map, with swapped row and column coordinates of the old std::map
	for (auto oldMapIter = oldMapBegin; oldMapIter != oldMapEnd; ++oldMapIter)
	{
		auto rowColumn_value_KVP = *(oldMapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double value = rowColumn_value_KVP.second;

		std::pair<size_t, size_t> pair_columnRow = std::make_pair(column, row);

		map_newSparseMatrix[pair_columnRow] = value;
	}

	sparseMatrix = map_newSparseMatrix;

	std::swap(numRows, numColumns);
}

double SparseMatrix::getSparsity() const
{
	size_t numElements = numRows * numColumns;
	size_t numZeroElements = numElements - sparseMatrix.size();

	double nominator = numZeroElements;
	double denominator = numElements;
	double sparsity = nominator / denominator;

	return sparsity;
}

double SparseMatrix::getDensity() const
{
	return 1.0 - getSparsity();
}

bool SparseMatrix::isSparse() const
{
	// Not checking for equality, because the threshold value itself is reserved for Density.
	return getSparsity() > MatrixBase::SparsityThreshold;
}

bool SparseMatrix::isDense() const
{
	return !isSparse();
}

bool SparseMatrix::requiresConversion() const
{
	return !isSparse(); // This is SparseMatrix. If it's not Sparse, then it requires conversion to Dense.
}

MatrixBase* SparseMatrix::getConvertedCopy() const
{
	return cloneAsDenseMatrix();
}

MatrixBase* SparseMatrix::clone() const
{
	SparseMatrix* clone = new SparseMatrix(numRows, numColumns);

	auto mapBegin = sparseMatrix.begin();
	auto mapEnd = sparseMatrix.end();

	for (auto mapIter = mapBegin; mapIter != mapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> rowColumn = rowColumn_value_KVP.first;
		double valueAtCell = rowColumn_value_KVP.second;

		size_t row = rowColumn.first;
		size_t column = rowColumn.second;

		clone->setCell(row, column, valueAtCell);
	}

	return clone;
}

DenseMatrix* SparseMatrix::cloneAsDenseMatrix() const
{
	DenseMatrix* denseClone = new DenseMatrix(numRows, numColumns, 0.0);

	auto mapBegin = sparseMatrix.begin();
	auto mapEnd = sparseMatrix.end();

	for (auto mapIter = mapBegin; mapIter != mapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> rowColumn = rowColumn_value_KVP.first;
		double valueAtCell = rowColumn_value_KVP.second;

		size_t row = rowColumn.first;
		size_t column = rowColumn.second;

		denseClone->setCell(row, column, valueAtCell);
	}

	return denseClone;
}

SparseMatrix* SparseMatrix::cloneAsSparseMatrix() const
{
	return dynamic_cast<SparseMatrix*>(clone());
}

std::vector<std::tuple<size_t, size_t, double>> SparseMatrix::getCellDataList() const
{
	std::vector<std::tuple<size_t, size_t, double>> cellList;

	auto mapBegin = sparseMatrix.begin();
	auto mapEnd = sparseMatrix.end();

	for (auto mapIter = mapBegin; mapIter != mapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> rowColumn = rowColumn_value_KVP.first;
		double valueAtCell = rowColumn_value_KVP.second;

		size_t row = rowColumn.first;
		size_t column = rowColumn.second;

		auto cellData = std::make_tuple(row, column, valueAtCell);

		cellList.push_back(cellData);
	}

	return cellList;
}

void SparseMatrix::scale(double scalar)
{
	auto cellDataList = getCellDataList();
	auto cellDataListBegin = cellDataList.begin();
	auto cellDataListEnd = cellDataList.end();

	for (auto iter = cellDataListBegin; iter != cellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		double newValue = value * scalar;

		setCell(row, column, newValue);
	}
}

bool SparseMatrix::equal(const MatrixBase& right) const
{
	return right.equal(*this);
}

bool SparseMatrix::equal(const DenseMatrix& left) const
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

bool SparseMatrix::equal(const SparseMatrix& left) const
{
	if ((left.getNumRows() != this->getNumRows()) || (left.getNumColumns() != this->getNumColumns()))
	{
		return false;
	}

	// Dimensions match. Check existing elements.

	auto leftCellDataList = left.getCellDataList();
	auto rightCellDataList = this->getCellDataList();

	// Check if they have the same number of elements.
	if (leftCellDataList.size() != rightCellDataList.size())
	{
		return false;
	}

	// Check if every single element is equal. Just iterate over one of them.
	auto leftCellDataListBegin = leftCellDataList.begin();
	auto leftCellDataListEnd = leftCellDataList.end();

	for (auto leftIter = leftCellDataListBegin; leftIter != leftCellDataListEnd; ++leftIter)
	{
		auto cellData = (*leftIter);
		size_t leftRow = std::get<0>(cellData);
		size_t leftColumn = std::get<1>(cellData);
		double leftValue = std::get<2>(cellData);

		double rightValue = this->getCell(leftRow, leftColumn);

		if (mcu::doubleAlmostEqual(leftValue, rightValue) == false)
		{
			return false;
		}
	}

	return true;
}

MatrixBase* SparseMatrix::add(const MatrixBase& right) const
{
	return right.add(*this);
}

MatrixBase* SparseMatrix::add(const DenseMatrix& left) const
{
	DenseMatrix* addedDense = new DenseMatrix(numRows, numColumns, 0.0);

	// Set left.
	for (size_t r = 0; r < left.getNumRows(); r++)
	{
		for (size_t c = 0; c < left.getNumColumns(); c++)
		{
			double valueAtCell = left.getCell(r, c);
			addedDense->setCell(r, c, valueAtCell);
		}
	}

	// Add right.
	auto rightCellDataList = this->getCellDataList();
	auto rightCellDataListBegin = rightCellDataList.begin();
	auto rightCellDataListEnd = rightCellDataList.end();

	for (auto iter = rightCellDataListBegin; iter != rightCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		double existingValue = addedDense->getCell(row, column);
		double newValue = existingValue + value;
		addedDense->setCell(row, column, newValue);
	}

	return addedDense;
}

MatrixBase* SparseMatrix::add(const SparseMatrix& left) const
{
	SparseMatrix* addedSparse = new SparseMatrix(numRows, numColumns);

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

		addedSparse->setCell(row, column, value);
	}

	// Add right.
	auto rightCellDataList = this->getCellDataList();
	auto rightCellDataListBegin = rightCellDataList.begin();
	auto rightCellDataListEnd = rightCellDataList.end();

	for (auto iter = rightCellDataListBegin; iter != rightCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		double existingValue = addedSparse->getCell(row, column);
		double newValue = existingValue + value;
		addedSparse->setCell(row, column, newValue);
	}

	return addedSparse;
}

MatrixBase* SparseMatrix::subtract(const MatrixBase& right) const
{
	return right.subtract(*this);
}

MatrixBase* SparseMatrix::subtract(const DenseMatrix& left) const
{
	DenseMatrix* subtractedDense = new DenseMatrix(numRows, numColumns, 0.0);

	// Set left.
	for (size_t r = 0; r < left.getNumRows(); r++)
	{
		for (size_t c = 0; c < left.getNumColumns(); c++)
		{
			double valueAtCell = left.getCell(r, c);
			subtractedDense->setCell(r, c, valueAtCell);
		}
	}

	// Add negated right.
	auto rightCellDataList = this->getCellDataList();
	auto rightCellDataListBegin = rightCellDataList.begin();
	auto rightCellDataListEnd = rightCellDataList.end();

	for (auto iter = rightCellDataListBegin; iter != rightCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		double existingValue = subtractedDense->getCell(row, column);
		double newValue = existingValue - value; // SUBTRACTION

		subtractedDense->setCell(row, column, newValue);
	}

	return subtractedDense;
}

MatrixBase* SparseMatrix::subtract(const SparseMatrix& left) const
{
	SparseMatrix* subtractedSparse = new SparseMatrix(numRows, numColumns);

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

		subtractedSparse->setCell(row, column, value);
	}

	// Add negated right.
	auto rightCellDataList = this->getCellDataList();
	auto rightCellDataListBegin = rightCellDataList.begin();
	auto rightCellDataListEnd = rightCellDataList.end();

	for (auto iter = rightCellDataListBegin; iter != rightCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double value = std::get<2>(cellData);

		double existingValue = subtractedSparse->getCell(row, column);
		double newValue = existingValue - value; // SUBTRACTION

		subtractedSparse->setCell(row, column, newValue);
	}

	return subtractedSparse;
}

MatrixBase* SparseMatrix::multiply(const MatrixBase& right) const
{
	return right.multiply(*this);
}

MatrixBase* SparseMatrix::multiply(const DenseMatrix& left) const
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

MatrixBase* SparseMatrix::multiply(const SparseMatrix& left) const
{
	// I'm using DenseMatrix * DenseMatrix algorithm again, because I don't know the best approach for now.

	SparseMatrix* sparseProduct = new SparseMatrix(left.getNumRows(), this->numColumns);

	for (size_t leftRow = 0; leftRow < left.getNumRows(); leftRow++)
	{
		for (size_t leftCol = 0; leftCol < left.getNumColumns(); leftCol++)
		{
			for (size_t rightCol = 0; rightCol < this->numColumns; rightCol++)
			{
				double leftValue = left.getCell(leftRow, leftCol);
				double rightValue = this->getCell(leftCol, rightCol);
				double product = leftValue * rightValue;

				double existingValue = sparseProduct->getCell(leftRow, rightCol);
				double newValue = existingValue + product;

				sparseProduct->setCell(leftRow, rightCol, newValue);
			}
		}
	}

	return sparseProduct;
}

MatrixBase* SparseMatrix::mergeByColumns(const MatrixBase& right) const
{
	return right.mergeByColumns(*this);
}

MatrixBase* SparseMatrix::mergeByColumns(const DenseMatrix& left) const
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
	auto rightCellDataList = this->getCellDataList();
	auto rightCellDataListBegin = rightCellDataList.begin();
	auto rightCellDataListEnd = rightCellDataList.end();

	for (auto iter = rightCellDataListBegin; iter != rightCellDataListEnd; ++iter)
	{
		auto cellData = (*iter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double rightValue = std::get<2>(cellData);

		size_t columnOffset = left.getNumColumns();
		size_t newColumnIndex = column + columnOffset;

		mergedDense->setCell(row, newColumnIndex, rightValue);
	}

	return mergedDense;
}

MatrixBase* SparseMatrix::mergeByColumns(const SparseMatrix& left) const
{
	SparseMatrix* mergedSparse = new SparseMatrix(numRows, left.getNumColumns() + numColumns);

	// Copy left.
	auto leftMapBegin = left.sparseMatrix.begin();
	auto leftMapEnd = left.sparseMatrix.end();

	for (auto mapIter = leftMapBegin; mapIter != leftMapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double leftValue = rowColumn_value_KVP.second;

		mergedSparse->setCell(row, column, leftValue);
	}

	// Copy right.
	auto rightMapBegin = this->sparseMatrix.begin();
	auto rightMapEnd = this->sparseMatrix.end();

	for (auto mapIter = rightMapBegin; mapIter != rightMapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double rightValue = rowColumn_value_KVP.second;

		size_t columnOffset = left.getNumColumns();
		size_t newColumnIndex = column + columnOffset;

		mergedSparse->setCell(row, newColumnIndex, rightValue);
	}

	return mergedSparse;
}

MatrixBase* SparseMatrix::mergeByRows(const MatrixBase& right) const
{
	return right.mergeByRows(*this);
}

MatrixBase* SparseMatrix::mergeByRows(const DenseMatrix& left) const
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
	auto rightCellDataList = this->getCellDataList();
	auto rightCellDataListBegin = rightCellDataList.begin();
	auto rightCellDataListEnd = rightCellDataList.end();

	for (auto cellListIter = rightCellDataListBegin; cellListIter != rightCellDataListEnd; ++cellListIter)
	{
		auto cellData = (*cellListIter);
		size_t row = std::get<0>(cellData);
		size_t column = std::get<1>(cellData);
		double rightValue = std::get<2>(cellData);

		size_t rowOffset = left.getNumRows();
		size_t newRowIndex = row + rowOffset;

		mergedDense->setCell(newRowIndex, column, rightValue);
	}

	return mergedDense;
}

MatrixBase* SparseMatrix::mergeByRows(const SparseMatrix& left) const
{
	SparseMatrix* mergedSparse = new SparseMatrix(left.getNumRows() + numRows, numColumns);

	// Copy left.
	auto leftMapBegin = left.sparseMatrix.begin();
	auto leftMapEnd = left.sparseMatrix.end();

	for (auto mapIter = leftMapBegin; mapIter != leftMapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double leftValue = rowColumn_value_KVP.second;

		mergedSparse->setCell(row, column, leftValue);
	}

	// Copy right.
	auto rightMapBegin = this->sparseMatrix.begin();
	auto rightMapEnd = this->sparseMatrix.end();

	for (auto mapIter = rightMapBegin; mapIter != rightMapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double rightValue = rowColumn_value_KVP.second;

		size_t rowOffset = left.getNumRows();
		size_t newRowIndex = row + rowOffset;

		mergedSparse->setCell(newRowIndex, column, rightValue);
	}

	return mergedSparse;
}

MatrixBase* SparseMatrix::splitByColumn(size_t leftNewNumColumns, bool returnLeftMatrix) const
{
	size_t totalNumColumns = this->numColumns;
	size_t rightNewNumColumns = totalNumColumns - leftNewNumColumns;

	size_t splitMatrixNumColumns = returnLeftMatrix ? leftNewNumColumns : rightNewNumColumns;
	size_t columnOffset = returnLeftMatrix ? 0 : leftNewNumColumns;

	size_t splitMatrixColumnBeginIndex = returnLeftMatrix ? 0 : leftNewNumColumns;
	size_t splitMatrixColumnEndIndex = returnLeftMatrix ? (leftNewNumColumns - 1) : (totalNumColumns - 1);

	SparseMatrix* split = new SparseMatrix(this->numRows, splitMatrixNumColumns);

	auto mapBegin = sparseMatrix.begin();
	auto mapEnd = sparseMatrix.end();

	for (auto mapIter = mapBegin; mapIter != mapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = *(mapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double value = rowColumn_value_KVP.second;

		// colBeginIndex <= column <= colEndIndex
		if ((splitMatrixColumnBeginIndex <= column) && (column <= splitMatrixColumnEndIndex))
		{
			size_t newColumnIndex = returnLeftMatrix ? (column + columnOffset) : (column - columnOffset);

			split->setCell(row, newColumnIndex, value);
		}
	}

	return split;
}

MatrixBase* SparseMatrix::splitByRow(size_t topNewNumRows, bool returnTopMatrix) const
{
	size_t totalNumRows = this->numRows;
	size_t bottomNewNumRows = totalNumRows - topNewNumRows;

	size_t splitMatrixNumRows = returnTopMatrix ? topNewNumRows : bottomNewNumRows;
	size_t rowOffset = returnTopMatrix ? 0 : topNewNumRows;

	size_t splitMatrixRowBeginIndex = returnTopMatrix ? 0 : topNewNumRows;
	size_t splitMatrixRowEndIndex = returnTopMatrix ? (topNewNumRows - 1) : (totalNumRows - 1);

	SparseMatrix* split = new SparseMatrix(splitMatrixNumRows, this->numColumns);

	auto mapBegin = sparseMatrix.begin();
	auto mapEnd = sparseMatrix.end();

	for (auto mapIter = mapBegin; mapIter != mapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = *(mapIter);
		std::pair<size_t, size_t> pair_rowColumn = rowColumn_value_KVP.first;

		size_t row = pair_rowColumn.first;
		size_t column = pair_rowColumn.second;
		double value = rowColumn_value_KVP.second;

		// beginIndex <= row <= endIndex
		if ((splitMatrixRowBeginIndex <= row) && (row <= splitMatrixRowEndIndex))
		{
			size_t newRowIndex = returnTopMatrix ? (row + rowOffset) : (row - rowOffset);

			split->setCell(newRowIndex, column, value);
		}
	}

	return split;
}

MatrixBase* SparseMatrix::getSubMatrix(size_t subRowBeginIndex, size_t subNumRows, size_t subColumnBeginIndex, size_t subNumColumns) const
{
	// This will be the only corner case that is handled by the method.
	if (subNumRows == 0 || subNumColumns == 0)
	{
		return nullptr;
	}

	SparseMatrix* subSparse = new SparseMatrix(subNumRows, subNumColumns);

	auto cellDataList = this->getCellDataList();
	auto cellDataListBegin = cellDataList.begin();
	auto cellDataListEnd = cellDataList.end();

	for (auto cellDataIter = cellDataListBegin; cellDataIter != cellDataListEnd; ++cellDataIter)
	{
		auto cellData = (*cellDataIter);
		size_t bigMatRow = std::get<0>(cellData);
		size_t bigMatCol = std::get<1>(cellData);
		double bigMatValue = std::get<2>(cellData);

		// Boundary check: (rowBegin <= row < (rowBegin + numRows)) && (colBegin <= col < (colBegin + numCols))
		if (((subRowBeginIndex <= bigMatRow) && (bigMatRow < (subRowBeginIndex + subNumRows)))
			&& ((subColumnBeginIndex <= bigMatCol) && (bigMatCol < (subColumnBeginIndex + subNumColumns))))
		{
			size_t subMatRow = bigMatRow - subRowBeginIndex;
			size_t subMatCol = bigMatCol - subColumnBeginIndex;
			subSparse->setCell(subMatRow, subMatCol, bigMatValue);
		}
	}

	return subSparse;
}

MatrixBase* SparseMatrix::getSubMatrix(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
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

MatrixBase* SparseMatrix::getSubMatrixTopLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == 0 || ignoredColumnIndex == 0)
	{
		return nullptr;
	}

	size_t subNumRows = ignoredRowIndex;
	size_t subNumColumns = ignoredColumnIndex;
	SparseMatrix* topLeft = new SparseMatrix(subNumRows, subNumColumns);

	auto cellDataList = this->getCellDataList();
	auto cellDataListBegin = cellDataList.begin();
	auto cellDataListEnd = cellDataList.end();

	for (auto cellDataIter = cellDataListBegin; cellDataIter != cellDataListEnd; ++cellDataIter)
	{
		auto cellData = (*cellDataIter);
		size_t bigRow = std::get<0>(cellData);
		size_t bigColumn = std::get<1>(cellData);
		double bigValue = std::get<2>(cellData);

		// Pick bigMatrix cells that are within the following boundaries:
		// 0 <= bigRow < ignoredRowIndex
		// 0 <= bigColumn < ignoredColumnIndex

		if (bigRow < ignoredRowIndex && bigColumn < ignoredColumnIndex)
		{
			topLeft->setCell(bigRow, bigColumn, bigValue);
		}
	}

	return topLeft;
}

MatrixBase* SparseMatrix::getSubMatrixTopRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == 0 || ignoredColumnIndex == (numColumns - 1))
	{
		return nullptr;
	}

	size_t subNumRows = ignoredRowIndex;
	size_t subNumColumns = (this->numColumns - 1) - ignoredColumnIndex;
	SparseMatrix* topRight = new SparseMatrix(subNumRows, subNumColumns);

	auto cellDataList = this->getCellDataList();
	auto cellDataListBegin = cellDataList.begin();
	auto cellDataListEnd = cellDataList.end();

	for (auto cellDataIter = cellDataListBegin; cellDataIter != cellDataListEnd; ++cellDataIter)
	{
		auto cellData = (*cellDataIter);
		size_t bigRow = std::get<0>(cellData);
		size_t bigColumn = std::get<1>(cellData);
		double bigValue = std::get<2>(cellData);

		// Pick bigMatrix cells that are within the following boundaries:
		// 0 <= bigRow < ignoredRowIndex
		// ignoredColumnIndex < bigColumn < this->numColumns

		if ((bigRow < ignoredRowIndex)
			&& ((ignoredColumnIndex < bigColumn) && (bigColumn < this->numColumns)))
		{
			size_t subColumn = bigColumn - (ignoredColumnIndex + 1); // If-check passed. Safe to do subtraction now.
			topRight->setCell(bigRow, subColumn, bigValue);
		}
	}

	return topRight;
}

MatrixBase* SparseMatrix::getSubMatrixBottomLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == (numRows - 1) || ignoredColumnIndex == 0)
	{
		return nullptr;
	}

	size_t subNumRows = (this->numRows - 1) - ignoredRowIndex;
	size_t subNumColumns = ignoredColumnIndex;
	SparseMatrix* bottomLeft = new SparseMatrix(subNumRows, subNumColumns);

	auto cellDataList = this->getCellDataList();
	auto cellDataListBegin = cellDataList.begin();
	auto cellDataListEnd = cellDataList.end();

	for (auto cellDataIter = cellDataListBegin; cellDataIter != cellDataListEnd; ++cellDataIter)
	{
		auto cellData = (*cellDataIter);
		size_t bigRow = std::get<0>(cellData);
		size_t bigColumn = std::get<1>(cellData);
		double bigValue = std::get<2>(cellData);

		// Pick bigMatrix cells that are within the following boundaries:
		// ignoredRowIndex < bigRow < this->numRows
		// 0 <= bigColumn < ignoredColumnIndex

		if (((ignoredRowIndex < bigRow) && (bigRow < this->numRows))
			&& (bigColumn < ignoredColumnIndex))
		{
			size_t subRow = bigRow - (ignoredRowIndex + 1); // If-check passed. Safe to do subtraction now.
			bottomLeft->setCell(subRow, bigColumn, bigValue);
		}
	}

	return bottomLeft;
}

MatrixBase* SparseMatrix::getSubMatrixBottomRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const
{
	if (ignoredRowIndex == (numRows - 1) || ignoredColumnIndex == (numColumns - 1))
	{
		return nullptr;
	}

	size_t subNumRows = (this->numRows - 1) - ignoredRowIndex;
	size_t subNumColumns = (this->numColumns - 1) - ignoredColumnIndex;
	SparseMatrix* bottomRight = new SparseMatrix(subNumRows, subNumColumns);

	auto cellDataList = this->getCellDataList();
	auto cellDataListBegin = cellDataList.begin();
	auto cellDataListEnd = cellDataList.end();

	for (auto cellDataIter = cellDataListBegin; cellDataIter != cellDataListEnd; ++cellDataIter)
	{
		auto cellData = (*cellDataIter);
		size_t bigRow = std::get<0>(cellData);
		size_t bigColumn = std::get<1>(cellData);
		double bigValue = std::get<2>(cellData);

		// Pick bigMatrix cells that are within the following boundaries:
		// ignoredRowIndex < bigRow < this->numRows
		// ignoredColumnIndex < bigColumn < this-numColumns

		if (((ignoredRowIndex < bigRow) && (bigRow < this->numRows))
			&& ((ignoredColumnIndex < bigColumn) && (bigColumn < this->numColumns)))
		{
			// If-check passed. Safe to do subtractions now.
			size_t subRow = bigRow - (ignoredRowIndex + 1);
			size_t subColumn = bigColumn - (ignoredColumnIndex + 1);
			bottomRight->setCell(subRow, subColumn, bigValue);
		}
	}

	return bottomRight;
}

double SparseMatrix::getDeterminant() const
{
	// I'm using DenseMatrix's algorithm for the determinant again, because I don't know the best approach for now.
	// However, at the very least, 'getSubMatrix' returns a SparseMatrix, so that makes it somewhat different.

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

MatrixBase* SparseMatrix::getMinorMatrix() const
{
	// I'm using DenseMatrix's algorithm here, because a minor matrix is full of determinants of SUB-MATRICES.
	SparseMatrix* sparseMinorMatrix = new SparseMatrix(numRows, numColumns);

	for (size_t r = 0; r < numRows; r++)
	{
		for (size_t c = 0; c < numColumns; c++)
		{
			MatrixBase* subSparseMatrix = this->getSubMatrix(r, c);

			double subDeterminant = subSparseMatrix->getDeterminant();

			sparseMinorMatrix->setCell(r, c, subDeterminant);

			delete subSparseMatrix;
		}
	}

	return sparseMinorMatrix;
}

void SparseMatrix::applyCheckerboardPattern()
{
	// Matrix is assumed to be 'square'.
	// Checkerboard Pattern. Even indices remain positive; odd indices are negated:
	// [+-+]
	// [-+-]
	// [+-+]

	auto mapBegin = sparseMatrix.begin();
	auto mapEnd = sparseMatrix.end();

	for (auto mapIter = mapBegin; mapIter != mapEnd; ++mapIter)
	{
		auto rowColumn_value_KVP = (*mapIter);
		std::pair<size_t, size_t> rowColumn = rowColumn_value_KVP.first;
		double valueAtCell = rowColumn_value_KVP.second;

		size_t row = rowColumn.first;
		size_t column = rowColumn.second;

		if ((row + column) % 2 == 0)
		{
			continue; // Even index. Leave it as it is.
		}

		setCell(row, column, -valueAtCell);
	}
}

MatrixBase* SparseMatrix::getInverse(double determinant) const
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
		inverse = new SparseMatrix(1, 1);
		inverse->setCell(0, 0, determinant);
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

std::string SparseMatrix::getPrintStr(size_t precision) const
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

std::string SparseMatrix::solveFor(const MatrixBase& augmentedColumn, bool verbose, size_t doublePrecision) const
{
	// Yes, I'm using DenseMatrix's algorithm. Either I do this, or fail the semester project completely...
	DenseMatrix* denseClone = this->cloneAsDenseMatrix();

	std::string ret = denseClone->solveFor(augmentedColumn, verbose, doublePrecision);

	delete denseClone;

	return ret;
}

size_t SparseMatrix::getRank() const
{
	// Yes, sorry for the cheat. I don't think I can finish the project in time otherwise...
	DenseMatrix* denseClone = this->cloneAsDenseMatrix();
	size_t rank = denseClone->getRank();

	delete denseClone;

	return rank;
}

// Private members

std::map<size_t, size_t> SparseMatrix::getColumnAlignmentMapForPrinting() const
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
