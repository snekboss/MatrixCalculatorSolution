#ifndef MATRIX_H
#define MATRIX_H

#include "MatrixBase.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"

/**
* Wrapper class for MatrixBase instances. Manages the the raw pointer resource. If the resource is nullptr, then the Matrix is considered to be in an invalid state; and is called invalid matrix.
*/
class Matrix
{
public:
	/**
	* Default constructor. Puts the matrix in an invalid state, because it doesn't make to construct a matrix without row and column information. Use the provided static methods instead.
	*/
	Matrix();
	/**
	* Copy Constructor. The current matrix instance is initialized via a deep copy operation on the argument Matrix's resource.
	* @param other The other Matrix to copy from.
	*/
	Matrix(const Matrix& other);

	/**
	* Checks whether or not this Matrix and the argument Matrix are equal. Returns false if either of the matrices is in an invalid state.
	* @param right The other Matrix.
	* @return True if equal, false if not equal. Also returns false if either of the matrices is in an invalid state.
	*/
	bool operator==(const Matrix& right);
	/**
	* Checks whether or not this Matrix and the argument Matrix are NOT equal. Returns false if either of the matrices is in an invalid state.
	* @param right The other Matrix.
	* @return True of NOT equal, false if equal. Also returns false if either of the matrices is in an invalid state.
	*/
	bool operator!=(const Matrix& right);
	/**
	* Performs matrix addition. Returns an invalid matrix if either of the arguments were invalid.
	* @param right The other Matrix.
	* @return The result of the addition.
	*/
	Matrix operator+(const Matrix& right);
	/**
	* Performs matrix subtraction. Returns an invalid matrix if either of the arguments were invalid.
	* @param right The other Matrix.
	* @return The result of the subtraction.
	*/
	Matrix operator-(const Matrix& right);
	/**
	* Performs matrix multiplication. Returns an invalid matrix if either of the arguments were invalid.
	* @param right The other Matrix.
	* @return The result of the multiplication.
	*/
	Matrix operator*(const Matrix& right);
	/**
	* Returns a scaled copy of the matrix. Returns an invalid matrix if the current matrix was invalid.
	* @param scalar Double precision floating point value by which to scale the matrix.
	* @return The result of the scaling operation.
	*/
	Matrix operator*(double scalar);
	/**
	* Returns a scaled copy of the matrix. Returns an invalid matrix if the current matrix was invalid. This particular method is for when the scalar is on the right hand side.
	* @param scalar Double precision floating point value by which to scale the matrix.
	* @param right The other Matrix.
	* @return The result of the scaling operation.
	*/
	friend Matrix operator*(double scalar, const Matrix& right);
	/**
	* Copy Assignment Operator. Performs a deep copy on the argument Matrix's resource and deletes the old resource.
	*/
	Matrix& operator=(const Matrix& other);
	/**
	* Destructor. Calls destroyResource method. Deallocates the resource if it's not nullptr.
	* @see destroyResource()
	*/
	~Matrix();

	/**
	* Returns a neatly aligned output string of the underlying matrix resource. Returns nullptr if the matrix is invalid.
	* @param precision The number of deigits after the floating point.
	* @return The print output of the matrix.
	*/
	std::string getPrintStr(size_t precision) const;

	/**
	* Returns the number of rows. Returns zero if the matrix is invalid.
	*/
	size_t getNumRows() const;
	/**
	* Returns the number of columns. Returns zero if the matrix is invalid.
	*/
	size_t getNumColumns() const;
	/**
	* Returns the double value at a given cell. Indices start from zero. Returns quiet NaN (Not a Number) if the matrix is invalid.
	* @param row Row index of the matrix.
	* @param column Column index of the matrix.
	* @return The double value which resides in the cell.
	*/
	double getCell(size_t row, size_t column) const;
	/**
	* Sets a cell to the given value. Indices start from zero. The method is a "no-op" if the matrix is invalid.
	* @param row Row index of the matrix.
	* @param column Column index of the matrix.
	* @param value The value to be set in the cell.
	*/
	void setCell(size_t row, size_t column, double value);
	/**
	* Resizes the number of rows of the matrix. The method is a "no-op" if the matrix is invalid.
	* @param newNumRows If the argument is less than current num rows, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	*/
	void resizeNumRows(size_t newNumRows);
	/**
	* Resizes the number of columns of the matrix. The method is a "no-op" if the matrix is invalid.
	* @param newNumColumns If the argument is less than current num columns, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	*/
	void resizeNumColumns(size_t newNumColumns);
	/**
	* Resizes the number of rows and columns of the matrix in a single function. The method is a "no-op" if the matrix is invalid.
	* @param newNumRows If the argument is less than current num rows, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	* @param newNumColumns If the argument is less than current num columns, the 'extra part' is truncated. If the argument is greater, then the new cells are initialized to zero.
	*/
	void resize(size_t newNumRows, size_t newNumColumns);
	/**
	* Transposes the matrix. The method is a "no-op" if the matrix is invalid.
	*/
	void transpose();
	/**
	* Gets the Sparsity value of the matrix. Sparsity is the ratio of numZeroElements/numTotalElements. Returns quiet NaN (Not a Number) if the matrix is invalid.
	* @see MatrixBase::SparsityThreshold
	* @return Sparsity floating point value between 0 and 1. The SparsityThreshold value itself is NOT considered Sparse. It is reserved for Density.
	*/
	double getSparsity() const;
	/**
	* Gets the Density value of the matrix. Density is the ratio of numNonZeroElements/numTotalElements. Returns quiet NaN (Not a Number) if the matrix is invalid.
	* @see MatrixBase::DensityThreshold
	* @return Density floating point value between 0 and 1. The DensityThreshold value itself IS considered Density.
	*/
	double getDensity() const;
	/**
	* Checks if the matrix's Sparsity is greater than SparsityThreshold. Also returns false if the matrix is invalid.
	* @see MatrixBase::SparsityThreshold
	* @see getSparsity()
	* @return True if Sparse, false if Dense. Also returns false if the matrix is invalid.
	*/
	bool isSparse() const;
	/**
	* Checks if the matrix's Density is greater than or equal to DensityThreshold. Also returns false if the matrix is invalid.
	* @see MatrixBase::DensityThreshold
	* @see getDensity()
	* @return True if Dense, false if Sparse. Also returns false if the matrix is invalid.
	*/
	bool isDense() const;
	/**
	* Converts the underlying resource to DenseMatrix, regardless of the Density value. The method is a "no-op" if the matrix is invalid.
	*/
	void toDense();
	/**
	* Converts the underlying resource to SparseMatrix, regardless of the Sparsity value. The method is a "no-op" if the matrix is invalid.
	*/
	void toSparse();
	/**
	* Checks whether or not the MatrixBase instance requires conversion to another MatrixBase instance. Also returns false if the matrix is invalid.
	* @see MatrixBase::DensityThreshold
	* @see getDensity()
	* @see isDense()
	* @return True if it requires conversion, false if not. Also returns false if the matrix is invalid.
	*/
	bool requiresConversion() const;
	/**
	* Converts the current matrix to the appropriate matrix type.
	* @see requiresConversion()
	*/
	void convertToAppropriateMatrixType();
	/**
	* Merges this and the argument matrix by columns and returns the result. Returns an invalid matrix if either of the arguments were invalid.
	* @param right The other MatrixBase.
	* @return The result of the merge by columns operation
	*/
	Matrix mergeByColumns(const Matrix& right);
	/**
	* Merges this and the argument matrix by rows and returns the result. Returns an invalid matrix if either of the arguments were invalid.
	* @param right The other MatrixBase.
	* @return The result of the merge by rows operation
	*/
	Matrix mergeByRows(const Matrix& right);
	/**
	* Splits this matrix by columns. The user will always specify the number of columns of the left matrix. Returns an invalid matrix if either of the arguments were invalid.
	* @param leftNewNumColumns The new number of columns for the left side of the split matrix. Must be greater than 0 and less than this matrix's numColumns.
	* @param returnLeftMatrix If you want the left part of the matrix to be returned, set to true. If you want the right side, then set to false.
	* @return The desired part of the result of the split by columns operation.
	*/
	Matrix splitByColumn(size_t leftNewNumColumns, bool returnLeftMatrix) const;
	/**
	* Splits this matrix by rows. The user will always specify the number of rows of the top matrix. Returns an invalid matrix if either of the arguments were invalid.
	* @param topNewNumRows The new number of rows for the top side of the split matrix. Must be greater than 0 and less than this matrix's numRows.
	* @param returnTopMatrix If you want the top part of the matrix to be returned, set to true. If you want the bottom side, then set to false.
	* @return The desired part of the result of the split by rows operation.
	*/
	Matrix splitByRow(size_t topNewNumRows, bool returnTopMatrix) const;
	/**
	* Returns the submatrix of this matrix. The submatrix will be specified as a "square" area within the bigger matrix. The parameters are the start INDEX and the SIZE of the dimensions. Returns an invalid matrix if either of the arguments were invalid.
	* @param subRowBeginIndex The beginning INDEX of the row of the submatrix.
	* @param subNumRows The number of rows of the submatrix. This is not an index, it's a SIZE.
	* @param subColumnBeginIndex The beginning INDEX of the row of the submatrix.
	* @param subNumColumns The number of columns of the submatrix. This is not an index, it's a SIZE.
	* @return The result of the operation.
	*/
	Matrix getSubMatrix(size_t subRowBeginIndex, size_t subNumRows, size_t subColumnBeginIndex, size_t subNumColumns) const;
	/**
	* Returns a submatrix by ignoring the current matrix's row and column as specified by the arguments. Returns an invalid matrix if either of the arguments were invalid.
	* @param ignoredRowIndex The row index to be ignored in the bigger matrix.
	* @param ignoredColumnIndex The column index to be ignored in the bigger matrix.
	* @return The result of the operation.
	*/
	Matrix getSubMatrix(size_t ignoredRowIndex, size_t ignoredColumnIndex) const;
	/**
	* Returns the top left part of this matrix, as specified by the row & column coordinates of the method's arguments. Returns an invalid matrix if the matrix was invalid.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return The top left part of the matrix.
	*/
	Matrix getSubMatrixTopLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const;
	/**
	* Returns the top right part of this matrix, as specified by the row & column coordinates of the method's arguments. Returns an invalid matrix if the matrix was invalid.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return The top right part of the matrix.
	*/
	Matrix getSubMatrixTopRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const;
	/**
	* Returns the bottom left part of this matrix, as specified by the row & column coordinates of the method's arguments. Returns an invalid matrix if the matrix was invalid.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return The bottom left part of the matrix.
	*/
	Matrix getSubMatrixBottomLeft(size_t ignoredRowIndex, size_t ignoredColumnIndex) const;
	/**
	* Returns the bottom right part of this matrix, as specified by the row & column coordinates of the method's arguments. Returns an invalid matrix if the matrix was invalid.
	* @param ignoredRowIndex The row index to be ignored.
	* @param ignoredColumnIndex The column index to be ignored.
	* @return The bottom right part of the matrix.
	*/
	Matrix getSubMatrixBottomRight(size_t ignoredRowIndex, size_t ignoredColumnIndex) const;
	/**
	* Calculates the determinant of the matrix using Laplace Expansion. The matrix is assumed to be square. Returns quiet NaN (Not a Number) if the matrix is invalid.
	* @return A double precision floating point value containing the determinant of this matrix.
	*/
	double getDeterminant() const;
	/**
	* Returns the matrix of minors of this matrix. Used while calculating the inverse of a matrix. Doesn't make sense to use it alone. The matrix is assumed to be square. Returns an invalid matrix if the matrix was invalid.
	* @return The minor matrix.
	*/
	Matrix getMinorMatrix() const;
	/**
	* Applies the so called checkerboard pattern on this matrix. It's used to calculate the Cofactor Matrix from Minor Matrix while calculating the inverse of a matrix. Doesn't make sense to use it alone. The matrix is assumed to be square. The method is a "no-op" if the matrix is invalid.
	*/
	void applyCheckerboardPattern();
	/**
	* Returns the inverse of this matrix. If the inverse doesn't exist, it will return an invalid matrix. The matrix is assumed to be square.
	* @param determinant The determinant of this matrix. getInverse method will return nullptr if the argument determinant is equal to zero.
	* @see getDeterminant()
	* @return The inverse matrix.
	*/
	Matrix getInverse(double determinant) const;
	/**
	* Treats this and the argument matrices as a Systems of Linear Equations, and performs Gaussian Eliminations to find the solution set, and return it as a string. The augmentedColumn matrix must have 1 column. Returns nullptr if either of the matrices were invalid.
	* @param augmentedColumn A column matrix with 1 column. It contains the numbers which the equations are equal to.
	* @param verbose True if the output string should contain the steps of Gaussian Elimination, false if not.
	* @param doublePrecision The number of digits after the floating point when outputting the cells of the matrix.
	* @see getPrintStr()
	* @return A string which contains the solution set (with steps if verbose was true) of the Systems of Linear Equations.
	*/
	std::string solveFor(const Matrix& augmentedColumn, bool verbose, size_t doublePrecision) const;
	/**
	* Performs Gaussian Elimination and finds the rank from the Reduced Echelon Form of this matrix. Uses a DenseMatrix copy to perform the operation. Returns zero if the matrix was invalid.
	* @see DenseMatrix::getRank()
	* @return The rank of this matrix.
	*/
	size_t getRank() const;

	/**
	* A static method to create a DenseMatrix. If any of the dimensions is less than 1, the DenseMatrix is in invalid state, but no exception is thrown. Use at your own risk.
	* @param numRows The number of rows for the DenseMatrix.
	* @param numColumns The number of columns for the DenseMatrix.
	* @param initialValues Every cell of the DenseMatrix will be set to this value.
	* @return A DenseMatrix, as requsted.
	*/
	static Matrix createDense(size_t numRows, size_t numColumns, double initialValues);
	/**
	* A static method to create a SparseMatrix. Initially, every cell of a SparseMatrix is set to zero. If any of the dimensions is less than 1, the SparseMatrix is in invalid state, but no exception is thrown. Use at your own risk.
	* @param numRows The number of rows for the SparseMatrix.
	* @param numColumns The number of columns for the SparseMatrix.
	* @return A SparseMatrix, as requsted.
	*/
	static Matrix createSparse(size_t numRows, size_t numColumns);
	/**
	* A static method to create a zero matrix. A zero matrix is inherently Sparse, so it will return a Sparse Matrix. If any of the dimensions is less than 1, the zero matrix is in invalid state, but no exception is thrown. Use at your own risk.
	* @param numRows The number of rows for the zero matrix.
	* @param numColumns The number of columns for the zero matrix.
	* @return A zero matrix, as requsted.
	*/
	static Matrix createZero(size_t numRows, size_t numColumns);
	/**
	* A static method to create an identity matrix. An identity matrix is inherently Sparse, so it will return a Sparse Matrix. An identity matrix is square, therefore there's only a single parameter to specify the dimensions. If the dimension is less than 1, the matrix is in an invalid state, but no exception is thrown. Use at your own risk.
	* @param numDimensions The number of rows and columns for the identity matrix.
	* @return An identity matrix, as requsted.
	*/
	static Matrix createIdentity(size_t numDimensions);

private:
	/**
	* The resource MatrixBase wrapped by this class Matrix.
	*/
	MatrixBase* matrixPtr;

	/**
	* Deallocates the resource if it's not nullptr. Mainly used in the Destructor and the Copy Assignment Operator.
	*/
	void destroyResource();
};

#endif // MATRIX_H
