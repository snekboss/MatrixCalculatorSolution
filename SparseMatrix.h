#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "MatrixBase.h"
#include <map>
#include <vector>
#include <functional>

class DenseMatrix;

/**
* The implementation of a matrix when it is Sparse. When a matrix is Sparse, it has more zero elements than non-zero elements. The underlying implementation uses std's map of pair and doubles. The pair is for the matrix cell at row & column coordinates, and the double is for the element at that cell.
*/
class SparseMatrix : public MatrixBase
{
public:
	/**
	* Creates an invalid SparseMatrix with zero rows and zero dimensions. Don't use it. Use the custom consturctor instead.
	*/
	SparseMatrix();
	/**
	* Default Copy Constructor implemented by the compiler. The copying of the underlying std::map is done by std::map.
	* @param other The SparseMatrix to copy construct from.
	*/
	SparseMatrix(const SparseMatrix& other) = default;
	/**
	* Default Move Constructor. The underlying data structure is an std::map. Move operation is handled by it.
	* @param other The SparseMatrix to move construct from.
	*/
	SparseMatrix(SparseMatrix&& other) noexcept = default;
	/**
	* Default Copy Assignment Operator. The assignment of the underlying std::map is done by std::map.
	* @param other The SparseMatrix to move construct from.
	*/
	SparseMatrix& operator=(const SparseMatrix& other) = default;
	/**
	* Default Move Assignment Operator. The underlying data structure is an std::map. Move operation is handled by it.
	* @param other The SparseMatrix to move assign from.
	*/
	SparseMatrix& operator=(SparseMatrix&& other) noexcept = default;
	/**
	* Default Destructor. The deallocating of resources is handled by std::map.
	*/
	~SparseMatrix() = default;
	/**
	* Custom Constructor for SparseMatrix. Initially, every cell of a SparseMatrix is set to zero. If any of the dimensions is less than 1, the SparseMatrix is in invalid state, but no exception is thrown. Use at your own risk.
	* @param numRows The number of rows for the SparseMatrix.
	* @param numColumns The number of columns for the SparseMatrix.
	*/
	SparseMatrix(size_t numRows, size_t numColumns);
	/**
	* Returns the number of rows of this matrix.
	*/
	virtual size_t getNumRows() const override;
	/**
	* Returns the number of columns of this matrix.
	*/
	virtual size_t getNumColumns() const override;
	/**
	* Returns the double value at a given cell. Indices start from zero. No exception is thrown by std::map (probaby) when the indices are out of range, but it is undefined behavior. Don't do it.
	* @param row Row index of the matrix.
	* @param column Column index of the matrix.
	* @return The double value which resides in the cell.
	*/
	virtual double getCell(size_t row, size_t column) const override;
	/**
	* Sets a cell to the given value. Indices start from zero. No exception is thrown by std::map (probaby) when the indices are out of range, but it is undefined behavior. Don't do it.
	* @param row Row index of the matrix.
	* @param column Column index of the matrix.
	* @param value The value to be set in the cell.
	*/
	virtual void setCell(size_t row, size_t column, double value) override;
	/**
	* Resizes the number of rows of the matrix. No exception is thrown by std::map (probaby) if the argument is bad, but don't do it anyway.
	* @param newNumRows If the argument is less than current num rows, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	*/
	virtual void resizeNumRows(size_t newNumRows) override;
	/**
	* Resizes the number of columns of the matrix. No exception is thrown by std::map (probaby) if the argument is bad, but don't do it anyway.
	* @param newNumColumns If the argument is less than current num columns, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	*/
	virtual void resizeNumColumns(size_t newNumColumns) override;
	/**
	* Resizes the number of rows and columns of the matrix in a single function. No exceptions are thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param newNumRows If the argument is less than current num rows, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	* @param newNumColumns If the argument is less than current num columns, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	*/
	virtual void resize(size_t newNumRows, size_t newNumColumns) override;
	/**
	* Transposes the matrix. Pretty safe function. Shouldn't throw any exceptions unless the matrix was invalid in the first place.
	*/
	virtual void transpose() override;
	/**
	* Gets the Sparsity value of the matrix. Sparsity is the ratio of numZeroElements/numTotalElements.
	* @see MatrixBase::SparsityThreshold
	* @return Sparsity floating point value between 0 and 1. The SparsityThreshold value itself is NOT considered Sparse. It is reserved for Density.
	*/
	virtual double getSparsity() const override;
	/**
	* Gets the Density value of the matrix. Density is the ratio of numNonZeroElements/numTotalElements.
	* @see MatrixBase::DensityThreshold
	* @return Density floating point value between 0 and 1. The DensityThreshold value itself IS considered Density.
	*/
	virtual double getDensity() const override;
	/**
	* Checks if the matrix's Sparsity is greater than SparsityThreshold.
	* @see MatrixBase::SparsityThreshold
	* @see getSparsity()
	* @return True if Sparse, false if Dense.
	*/
	virtual bool isSparse() const override;
	/**
	* Checks if the matrix's Density is greater than or equal to DensityThreshold.
	* @see MatrixBase::DensityThreshold
	* @see getDensity()
	* @return True if Dense, false if Sparse.
	*/
	virtual bool isDense() const override;
	/**
	* This class is SparseMatrix. It is meant for containing sparse matrices. If its Sparsity is less than or equal to SparsityThreshold, then it requires conversion to DenseeMatrix. This method answers that question.
	* @see MatrixBase::DensityThreshold
	* @see getDensity()
	* @see isDense()
	* @return True if it requires conversion, false if not.
	*/
	virtual bool requiresConversion() const override;
	/**
	* This is SparseMatrix. If it requires any conversion, then it should return a DenseMatrix. This method does just that.
	* @see requiresConversion()
	* @return A raw pointer to a MatrixBase instance. In this case, it will be a DenseMatrix instance.
	*/
	virtual MatrixBase* getConvertedCopy() const override;
	/**
	* Clones (deep copy) this SparseMatrix instance.
	* @return A raw pointer to a MatrixBase instance. In this case, it will be a SparseMatrix instance.
	*/
	virtual MatrixBase* clone() const override;
	/**
	* Creates a DenseMatrix version of this matrix, regardless of the Density value. Doesn't make sense to use this method if the matrix is sparse.
	* @see isDense()
	* @return A raw pointer to DenseMatrix instance.
	*/
	virtual DenseMatrix* cloneAsDenseMatrix() const override;
	/**
	* This is SparseMatrix. Cloning as SparseMatrix is the same as just cloning.
	* @see clone()
	* @return A raw pointer to MatrixBase instance. In this case, it will be a SparseMatrix instance.
	*/
	virtual SparseMatrix* cloneAsSparseMatrix() const override;
	/**
	* Creates a vector of 3-tuples. The first two size_t are the row & column data; the double is the value at the row & column coordinate. It makes it easier to use SparseMatrix. Acts as a short hack instead of writing a custom iterator. Recommended to use this instead of iterating over an std::map, which can reduce your keyboard's life span with all the insufferably::verbose::iterator::syntax..
	* @return A vector of 3-tuples, which contains row & column coordinates; and their corresponding double value.
	*/
	virtual std::vector<std::tuple<size_t, size_t, double>> getCellDataList() const override;
	/**
	* Scales every cell of this matrix by the given scalar value.
	* @param scalar Scalar value to scale each cell.
	*/
	virtual void scale(double scalar) override;
	/**
	* Checks if this matrix is equal to the argument matrix. Method implements Double Dispatch. This particular method just calls the equal method on the argument to activate polymorphism.
	* @param right The other MatrixBase.
	* @return True if equal, false if not equal.
	*/
	virtual bool equal(const MatrixBase& right) const override;
	/**
	* Checks if this matrix is equal to the argument matrix. Method implements Double Dispatch. This particular checks if this SparseMatrix is equal to the other DenseMatrix.
	* @param left The other DenseMatrix. This is meant to be called by the more generic equal method.
	* @return True if equal, false if not equal.
	*/
	virtual bool equal(const DenseMatrix& left) const override;
	/**
	* Checks if this matrix is equal to the argument matrix. Method implements Double Dispatch. This particular checks if this SparseMatrix is equal to the other SparseMatrix.
	* @param left The other SparseMatrix. This is meant to be called by the more generic equal method.
	* @return True if equal, false if not equal.
	*/
	virtual bool equal(const SparseMatrix& left) const override;
	/**
	* Performs matrix addition with the argument and returns the result. Method implements Double Dispatch. This particular method just calls the add method on the argument to activate polymorphism. std::vector may throw an exception (if at least one of the matrices is DenseMatrix) if the dimensions don't match.
	* @param right The other MatrixBase.
	* @return A raw pointer to MatrixBase instance. The result is SparseMatrix if and only if both of the matrices are of type SparseMatrix.
	*/
	virtual MatrixBase* add(const MatrixBase& right) const override;
	/**
	* Performs matrix addition with the argument and returns the result. Method implements Double Dispatch. This particular method adds this SparseMatrix to the other DenseMatrix and returns the result. std::vector may throw an exception if the dimensions don't match.
	* @param left The other DenseMatrix. This is meant to be called by the more generic add method.
	* @return A raw pointer to MatrixBase instance. At least one of the matrices is of type DenseMatrix, so the result will be a DenseMatrix.
	*/
	virtual MatrixBase* add(const DenseMatrix& left) const override;
	/**
	* Performs matrix addition with the argument and returns the result. Method implements Double Dispatch. This particular method adds this SparseMatrix to the other SparseMatrix and returns the result. No exception is thrown by std::map (probaby) if the dimensions don't match, but don't do it anyway.
	* @param left The other SparseMatrix. This is meant to be called by the more generic add method.
	* @return A raw pointer to MatrixBase instance. Both of the matrices are SparseMatrix, so it will return a SparseMatrix.
	*/
	virtual MatrixBase* add(const SparseMatrix& left) const override;
	/**
	* Performs matrix subtraction with the argument and returns the result. Method implements Double Dispatch. This particular method just calls the subtract method on the argument to activate polymorphism. std::vector may throw an exception (if at least one of the matrices is DenseMatrix) if the dimensions don't match.
	* @param right The other MatrixBase.
	* @return A raw pointer to MatrixBase instance. The result is SparseMatrix if and only if both of the matrices are of type SparseMatrix.
	*/
	virtual MatrixBase* subtract(const MatrixBase& right) const override;
	/**
	* Performs matrix subtraction with the argument and returns the result. Method implements Double Dispatch. This particular method subtracts this SparseMatrix from the argument DenseMatrix. std::vector may throw an exception if the dimensions don't match.
	* @param left The other DenseMatrix. This is meant to be called by the more generic subtract method.
	* @return A raw pointer to MatrixBase instance. At least one of the matrices is of type DenseMatrix, so the result will be a DenseMatrix.
	*/
	virtual MatrixBase* subtract(const DenseMatrix& left) const override;
	/**
	* Performs matrix subtraction with the argument and returns the result. Method implements Double Dispatch. This particular method subtracts this SparseMatrix from the argument SparseMatrix. No exception is thrown by std::map (probaby) if the dimensions don't match, but don't do it anyway.
	* @param left The other SparseMatrix. This is meant to be called by the more generic subtract method.
	* @return A raw pointer to MatrixBase instance. Both of the matrices are SparseMatrix, so it will return a SparseMatrix.
	*/
	virtual MatrixBase* subtract(const SparseMatrix& left) const override;
	/**
	* Performs matrix multiplication with the argument and returns the result. Method implements Double Dispatch. This particular method just calls the multiply method on the argument to activate polymorphism. std::vector may throw an exception (if at least one of the matrices is DenseMatrix) if the dimensions don't match. The method uses the same algorithm as DenseMatrix::multiply.
	* @param right The other MatrixBase.
	* @return A raw pointer to MatrixBase instance. The result is SparseMatrix if and only if both of the matrices are of type SparseMatrix.
	*/
	virtual MatrixBase* multiply(const MatrixBase& right) const override;
	/**
	* Performs matrix multiplication with the argument and returns the result. Method implements Double Dispatch. This particular method multiples the argument DenseMatrix by this SparseMatrix. std::vector may throw an exception if the dimensions don't match. The method uses the same algorithm as DenseMatrix::multiply.
	* @param left The other DenseMatrix. This is meant to be called by the more generic multiply method.
	* @return A raw pointer to MatrixBase instance. At least one of the matrices is of type DenseMatrix, so the result will be a DenseMatrix.
	*/
	virtual MatrixBase* multiply(const DenseMatrix& left) const override;
	/**
	* Performs matrix multiplication with the argument and returns the result. Method implements Double Dispatch. This particular method multiples the argument SparseMatrix by this SparseMatrix. No exception is thrown by std::map (probaby) if the dimensions don't match, but don't do it anyway. The method uses the same algorithm as DenseMatrix::multiply.
	* @param left The other SparseMatrix. This is meant to be called by the more generic multiply method.
	* @return A raw pointer to MatrixBase instance. Both of the matrices are SparseMatrix, so it will return a SparseMatrix.
	*/
	virtual MatrixBase* multiply(const SparseMatrix& left) const override;
	/**
	* Merges this and the argument matrix by columns and returns the result. Method implements Double Disptch. This particular method just calls the mergeByColumns method on the argument to activate polymorphism. std::vector may throw an exception (if at least one of the matrices is DenseMatrix) if the dimensions don't match.
	* @param right The other MatrixBase.
	* @return A raw pointer to MatrixBase instance. The result is SparseMatrix if and only if both of the matrices are of type SparseMatrix.
	*/
	virtual MatrixBase* mergeByColumns(const MatrixBase& right) const override;
	/**
	* Merges this and the argument matrix by columns and returns the result. Method implements Double Disptch. This particular method merges the argument matrix with this and returns the result. std::vector may throw an exception if the dimensions don't match.
	* @param left The other DenseMatrix. This is meant to be called by the more generic mergeByColumns method.
	* @return A raw pointer to MatrixBase instance. At least one of the matrices is of type DenseMatrix, so the result will be a DenseMatrix.
	*/
	virtual MatrixBase* mergeByColumns(const DenseMatrix& left) const override;
	/**
	* Merges this and the argument matrix by columns and returns the result. Method implements Double Disptch. This particular method merges the argument matrix with this and returns the result. No exception is thrown by std::map (probaby) if the dimensions don't match, but don't do it anyway.
	* @param left The other SparseMatrix. This is meant to be called by the more generic mergeByColumns method.
	* @return A raw pointer to MatrixBase instance. Both of the matrices are SparseMatrix, so it will return a SparseMatrix.
	*/
	virtual MatrixBase* mergeByColumns(const SparseMatrix& left) const override;
	/**
	* Merges this and the argument matrix by rows and returns the result. Method implements Double Disptch. This particular method just calls the mergeByRows method on the argument to activate polymorphism. std::vector may throw an exception (if at least one of the matrices is DenseMatrix) if the dimensions don't match.
	* @param right The other MatrixBase.
	* @return A raw pointer to MatrixBase instance. The result is SparseMatrix if and only if both of the matrices are of type SparseMatrix.
	*/
	virtual MatrixBase* mergeByRows(const MatrixBase& right) const override; 
	/**
	* Merges this and the argument matrix by rows and returns the result. Method implements Double Disptch. This particular method merges the argument matrix with this and returns the result. std::vector may throw an exception if the dimensions don't match.
	* @param left The other DenseMatrix. This is meant to be called by the more generic mergeByRows method.
	* @return A raw pointer to MatrixBase instance. At least one of the matrices is of type DenseMatrix, so the result will be a DenseMatrix.
	*/
	virtual MatrixBase* mergeByRows(const DenseMatrix& left) const override; 
	/**
	* Merges this and the argument matrix by rows and returns the result. Method implements Double Disptch. This particular method merges the argument matrix with this and returns the result. No exception is thrown by std::map (probaby) if the dimensions don't match, but don't do it anyway.
	* @param left The other SparseMatrix. This is meant to be called by the more generic mergeByRows method.
	* @return A raw pointer to MatrixBase instance. Both of the matrices are SparseMatrix, so it will return a SparseMatrix.
	*/
	virtual MatrixBase* mergeByRows(const SparseMatrix& left) const override;
	/**
	* Splits this matrix by columns. The user will always specify the number of columns of the left matrix. No exception is thrown by std::map if the arguments are bad, but don't do it anyway.
	* @param leftNewNumColumns The new number of columns for the left side of the split matrix. Must be greater than 0 and less than this matrix's numColumns.
	* @param returnLeftMatrix If you want the left part of the matrix to be returned, set to true. If you want the right side, then set to false.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* splitByColumn(size_t leftNewNumColumns, bool returnLeftMatrix) const override;
	/**
	* Splits this matrix by rows. The user will always specify the number of rows of the top matrix. No exception is thrown by std::map if the arguments are bad, but don't do it anyway.
	* @param topNewNumRows The new number of rows for the top side of the split matrix. Must be greater than 0 and less than this matrix's numRows.
	* @param returnTopMatrix If you want the top part of the matrix to be returned, set to true. If you want the bottom side, then set to false.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* splitByRow(size_t topNewNumRows, bool returnTopMatrix) const override;
	/**
	* Returns the submatrix of this matrix. The submatrix will be specified as a "square" area within the bigger matrix. The parameters are the start INDEX and the SIZE of the dimensions. No exception is thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param subRowBeginIndex The beginning INDEX of the row of the submatrix.
	* @param subNumRows The number of rows of the submatrix. This is not an index, it's a SIZE.
	* @param subColumnBeginIndex The beginning INDEX of the row of the submatrix.
	* @param subNumColumns The number of columns of the submatrix. This is not an index, it's a SIZE.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getSubMatrix(size_t subRowBeginIndex, size_t subNumRows, size_t subColumnBeginIndex, size_t subNumColumns) const override;
	/**
	* Returns a submatrix by ignoring the current matrix's row and column as specified by the arguments. No exception is thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param ignoredRowIndex The row index to be ignored in the bigger matrix.
	* @param ignoredColumnIndex The column index to be ignored in the bigger matrix.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getSubMatrix(size_t ignoredRowIndex, size_t ignoredColumnIndex) const override;
	/**
	* Returns the top left part of this matrix, as specified by the row & column coordinates of the method's arguments. No exception is thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getSubMatrixTopLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const override;
	/**
	* Returns the top right part of this matrix, as specified by the row & column coordinates of the method's arguments. No exception is thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getSubMatrixTopRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const override;
	/**
	* Returns the bottom left part of this matrix, as specified by the row & column coordinates of the method's arguments. No exception is thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getSubMatrixBottomLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const override;
	/**
	* Returns the bottom right part of this matrix, as specified by the row & column coordinates of the method's arguments. No exception is thrown by std::map (probaby) if the arguments are bad, but don't do it anyway.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return A raw pointer to MatrixBase instance. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getSubMatrixBottomRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const override;
	/**
	* Calculates the determinant of the matrix using Laplace Expansion. The matrix is assumed to be square. No exception is thrown by std::map (probably) if the arguments are bad, but don't do it anyway.
	* @return A double floating point value containing the determinant of this matrix.
	*/
	virtual double getDeterminant() const override;
	/**
	* Returns the matrix of minors of this matrix. Used while calculating the inverse of a matrix. Doesn't make sense to use it alone. The matrix is assumed to be square. No exception is thrown by std::map (probably) if the arguments are bad, but don't do it anyway.
	* @return A raw pointer to MatrixBase instance, containing the matrix of minors. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getMinorMatrix() const override;
	/**
	* Applies the so called checkerboard pattern on this matrix. It's used to calculate the Cofactor Matrix from Minor Matrix while calculating the inverse of a matrix. Doesn't make sense to use it alone. The matrix is assumed to be square. No exception is thrown by std::map (probably) if the arguments are bad, but don't do it anyway.
	*/
	virtual void applyCheckerboardPattern() override;
	/**
	* Returns the inverse of this matrix. If the inverse doesn't exist, it will return nullptr. The matrix is assumed to be square. No exception is thrown by std::map (probably) if the arguments are bad, but don't do it anyway.
	* @param determinant The determinant of this matrix. getInverse method will return nullptr if the argument determinant is equal to zero.
	* @see getDeterminant()
	* @return A raw pointer to MatrixBase instance, containing the inverse of this matrix. This is SparseMatrix, so the result will also be SparseMatrix.
	*/
	virtual MatrixBase* getInverse(double determinant) const override;
	/**
	* Prepares a neat looking output string for this matrix. The cells are nicely aligned with respect to their columns. The output floating point values are fixed (std::fixed).
	* @param precision The number of digits after the floating point.
	* @return The output string which contains the neatly aligned cell values.
	*/
	virtual std::string getPrintStr(size_t precision) const override;
	/**
	* Treats this and the argument matrices as a Systems of Linear Equations, and performs Gaussian Eliminations to find the solution set, and return it as a string. The augmentedColumn matrix must have 1 column. Uses a DenseMatrix copy to perform the operation.
	* @see DenseMatrix::solveFor()
	* @param augmentedColumn A column matrix with 1 column. It contains the numbers which the equations are equal to.
	* @param verbose True if the output string should contain the steps of Gaussian Elimination, false if not.
	* @param doublePrecision The number of digits after the floating point when outputting the cells of the matrix.
	* @see getPrintStr()
	* @return A string which contains the solution set (with steps if verbose was true) of the Systems of Linear Equations.
	*/
	virtual std::string solveFor(const MatrixBase& augmentedColumn, bool verbose, size_t doublePrecision) const override;
	/**
	* Performs Gaussian Elimination and finds the rank from the Reduced Echelon Form of this matrix. Uses a DenseMatrix copy to perform the operation.
	* @see DenseMatrix::getRank()
	* @return The rank of this matrix.
	*/
	virtual size_t getRank() const;
private:
	/**
	* The underlying implementation of SparseMatrix. An std::map of std::pair for row & column coordinates of the cells; and a double as value of the cell.
	*/
	std::map<std::pair<size_t, size_t>, double> sparseMatrix;
	/**
	* The number of rows of this matrix.
	*/
	size_t numRows;
	/**
	* The number of columns of this matrix.
	*/
	size_t numColumns;

	/**
	* Returns a map of alignment for each column in order to achieve a neatly aligned output stirng. The method calculates the maximum digit size after the floating point each column has.
	* @return An "alignment map". The first size_t is the index of the column; the second size_t is the maximum digit size for each column. The negative sign adds 1 to the "digit count" as well.
	*/
	std::map<size_t, size_t> getColumnAlignmentMapForPrinting() const;
};

#endif // SPARSE_MATRIX_H
