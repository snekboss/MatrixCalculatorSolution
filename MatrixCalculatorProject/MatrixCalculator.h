#ifndef MATRIX_CALCULATOR_H
#define MATRIX_CALCULATOR_H

#include "Matrix.h"
#include <iostream>
#include <map>

/**
* The user interface of the Matrix Calculator program. It uses a simple switch(case) structure to execute commands which are similar to assembly. The commands are given via the console.
*/
class MatrixCalculator
{
public:
	/**
	* Default Constructor. Initializes some const fields. Also sets the std::fixed flag on std::cout for floating point values. We don't want no scientific notation here!
	* @see MatrixCalculator::MatrixIODirectoryName
	* @see MatrixCalculator::CurrentPathStr
	* @see MatrixCalculator::MatrixIOPathStr
	*/
	MatrixCalculator();
	/**
	* Default Copy Constructor. The copying of the underlying members are done automatically by the data structures of the standard library.
	*/
	MatrixCalculator(const MatrixCalculator& other) = default;
	/**
	* Default Move Constructor. The moving of the underlying members are done automatically by the data structures of the standard library.
	*/
	MatrixCalculator(MatrixCalculator&& other) noexcept = default;
	/**
	* Default Copy Assignment Operator. The copying of the underlying members are done automatically by the data structures of the standard library.
	*/
	MatrixCalculator& operator=(const MatrixCalculator& other) = default;
	/**
	* Default Move Assignment Operator. The moving of the underlying members are done automatically by the data structures of the standard library.
	*/
	MatrixCalculator& operator=(MatrixCalculator&& other) noexcept = default;
	/**
	* Default Destructor. The destruction of the underlying members are done automatically by the data structures of the standard library.
	*/
	~MatrixCalculator() = default;

	/**
	* A single function to use the entire command line user interface. It works on a single thread, so it'll block the caller thread.
	*/
	void run();

private:

	/**
	* Name of the directory for MatrixIO operations.
	*/
	const std::string MatrixIODirectoryName = "examples";
	/**
	* Path of the executable.
	*/
	const std::string CurrentPathStr;
	/**
	* Path of MatrixIO directory. It is "$(CurrentPath)/$(MatrixIODirectoryName)".
	*/
	const std::string MatrixIOPathStr;

	/**
	* An enum class for the commands of the user interface program. Must be in sync with initializeCommands method, because C++ doesn't convert an enum class to string.
	* @see initializeCommands()
	*/
	enum class Command
	{
		exit,					/**< Exits the program.*/
		help,					/**< Shows the list of all commands.*/
		vars,					/**< Shows the list of all variable names of matrices in the program.*/
		dirinfo,				/**< Shows information regarding directories and paths.*/
		cls,					/**< Clears the console screen.*/
		dims,					/**< Shows the dimensions of a matrix.*/
		erase,					/**< Erases a variable inside the program's memory.*/
		eraseallvars,			/**< Erases all variables inside the program's memory.*/
		copy,					/**< Creates a copy of a matrix into another. */
		rename,					/**< Renames a variable name. */
		zero,					/**< Creates a zero matrix. */
		identity,				/**< Creates an identity matrix. */
		print,					/**< Prints a matrix into the console or a file.*/
		getprintprecision,		/**< Gets the current precision for floating point numbers.*/
		setprintprecision,		/**< Sets the current precision for floating point numbers. */
		read,					/**< Reads a matrix from the console or a file. */
		resize,					/**< Resizes a matrix by rows or columns. */
		add,					/**< Performs a matrix addition operation with two matrices and stores the result into a variable. */
		sub,					/**< Performs a matrix subtraction operation with two matrices and stores the result into a variable. */
		mul,					/**< Performs a matrix multiplication operation with two matrices and stores the result into a variable. */
		scale,					/**< Scales a matrix by a a scalar.*/
		transpose,				/**< Transposes a matrix. */
		split,					/**< Splits a matrix by rows or columns. */
		merge,					/**< Merges a matrix by rows or columns. */
		invert,					/**< Inverts a matrix. */
		det,					/**< Calculates and prints the determinant of a matrix. */
		rank,					/**< Calculates and prints the rank of a matrix. */
		solvefor,				/**< Solves Systems of Linear Equations. */
		getcell,				/**< Gets a cell of a matrix. */
		setcell,				/**< Sets the cell of a matrix by a value. */
		density,				/**< Gets the density value of a matrix. */
		sparsity				/**< Gets the sparisty value of a matrix. */
	};

	/**
	* Acts as a global variable for the list of all input "words" extracted out of a single line from the console. These inputs are meant to be used for the commands, later on.
	* @see getInputList()
	*/
	std::vector<std::string> inputList;
	/**
	* The list of all possible commands which the program can execute. Must be in sync with enum class Commands.
	* @see MatrixCalculator::Commands
	*/
	std::map<std::string, Command> commands;
	/**
	* The list of the names of all matrices in the program's memory throughout the life of the program.
	*/
	std::map<std::string, Matrix> varName_matrix_map;
	/**
	* The current set floating point precision value for printing matrices for output purposes.
	*/
	size_t doublePrintPrecision = 2;

	/**
	* Initializes (or defines) all possible commands the program supports. It simply adds the appropriate string values to varName_matrix_map.
	* @see varName_matrix_map
	*/
	void initializeCommands();
	/**
	* Gets an input line from the console and separates the "words" by whitespaces. These inputs are meant to be used for the commands, later on.
	* @see inputList
	*/
	std::vector<std::string> getInputList();
	/**
	* A matrix is conceptually a two dimensional object. However, the coordinates of the cells can be expressed by a single value, since the elements of a matrix can be written as a single line. This method finds out the two-dimensional coordinates out of a single line.
	* @param cellNumber The single number which acts as a coordinate of a cell.
	* @param totalNumColumns The number of columns of the matrix in question.
	* @return Row and column coordinates, stored in an std::pair.
	*/
	std::pair<size_t, size_t> getMatrixCoordinates(size_t cellNumber, size_t totalNumColumns);
	/**
	* A shortcut for me to let the user know that a variable name does not exist. This message is used in several places, so having it as a method saves me from making typos.
	*/
	void doPrint_varNameDoesNotExist(std::string varName);
	/**
	* A shortcut for me to let the user know that the input was invalid. This message is used in several places, so having it as a method saves me from making typos.
	*/
	void doPrint_invalidInput();
	/**
	* A shortcut for me to let the user know that a variable name was overwritten by a new matrix object. This message is used in several places, so having it as a method saves me from making typos.
	*/
	void doPrint_overwrittenExistingVariable(std::string varName);
	/**
	* Outputs a string to a text file. If MatrixIODirectoryName does not exist, it will be created by this method automatically.
	* @see MatrixCalculator::MatrixIODirectoryName
	* @see MatrixCalculator::MatrixIOPathStr
	* @param outputString The string which is meant to be written into a text file.
	* @param fileName The name of the file to which the outputString is meant to be written.
	* @return Returns true if the write operation was successful, false if not successful.
	*/
	bool outputStringToTextFile(std::string outputString, std::string fileName);
	/**
	* Checks if a variable name of a matrix already exists.
	* @param varName The name of the variable to query.
	* @return True if exists, false if not exists.
	*/
	bool variableNameExists(std::string varName);
	/**
	* Safely converts a string to an unsigned 32-bit integer. Exceptions are handled with a simple, default C++ try-catch block.
	* @param str The string from which the unsigned 32-bit integer is meant to be read.
	* @param out_uint A pointer to the unsigned 32-bit integer to which the result is meant to be stored. This is an output variable, the user must declare the necessary variable before calling this method.
	* @return Returns true if the conversion was successful, false if not successful.
	*/
	bool readStringToUInt(std::string str, size_t* out_uint);
	/**
	* Safely converts a string to a double precision floating point number. Exceptions are handled with a simple, default C++ try-catch block.
	* @param str The string from which the unsigned 32-bit integer is meant to be read.
	* @param out_double A pointer to the double to which the result is meant to be stored. This is an output variable, the user must declare the necessary variable before calling this method.
	* @return Returns true if the conversion was successful, false if not successful.
	*/
	bool readStringToDouble(std::string str, double* out_double);
	/**
	* Safely converts a string to a C char. Exceptions are handled with a simple, default C++ try-catch block.
	* @param str The string from which the unsigned 32-bit integer is meant to be read.
	* @param out_char A pointer to the C char to which the result is meant to be stored. This is an output variable, the user must declare the necessary variable before calling this method.
	* @return Returns true if the conversion was successful, false if not successful.
	*/
	bool readStringToLowerChar(std::string str, char* out_char);
	/**
	* Safely converts several double precision floating point numbers, and stores them into a std::vector. Exceptions are handled with a simple, default C++ try-catch block.
	* @param str The string from which the doubles are meant to be read.
	* @param out_doubles A pointer to the std::vector to which the dobules are meant to be stored. This is an output variable, the user must declare the necessary variable before calling this method.
	* @return Returns true if the conversion was successful, false if not successful.
	*/
	bool readDoublesFromString(std::string str, std::vector<double>* out_doubles);

	/**
	* A method which contains a simple switch(case) for all possible commands which this program supports.
	* @param cmd The command to be executed. Does nothing if the argument is not a known Command.
	* @see MatrixCalculator::Command
	*/
	void handleCommand(Command cmd);
	/**
	* Prints the list of all commands supported by this program, with their usages and some examples.
	*/
	void handleCommand_help();
	/**
	* Prints the names of all variables of all matrices in the program's memory.
	*/
	void handleCommand_vars();
	/**
	* Prints information about the input & output directory, and their paths.
	*/
	void handleCommand_dirinfo();
	/**
	* Clears the console screen.
	*/
	void handleCommand_cls();
	/**
	* Shows the dimensions of a matrix on the console.
	*/
	void handleCommand_dims();
	/**
	* Erases a matrix variable from the program's memory.
	*/
	void handleCommand_erase();
	/**
	* Erases all matrix variables from the program's memory. Does not ask for confirmation.
	*/
	void handleCommand_eraseallvars();
	/**
	* Creates a copy of an existing matrix, and stores it into a variable.
	*/
	void handleCommand_copy();
	/**
	* Renames the variable string of a matrix.
	*/
	void handleCommand_rename();
	/**
	* Creates a zero matrix.
	*/
	void handleCommand_zero();
	/**
	* Creates an identity matrix.
	*/
	void handleCommand_identity();
	/**
	* Prints a matrix to the console screen or a text file.
	*/
	void handleCommand_print();
	/**
	* Shows the precision of the floating point numbers when printing matrices.
	*/
	void handleCommand_getprintprecision();
	/**
	* Sets the precision of the floating point numbers when printing matrices.
	*/
	void handleCommand_setprintprecision();
	/**
	* Reads a matrix from the console or a text file.
	*/
	void handleCommand_read();
	/**
	* Resizes a matrix by rows or columns.
	*/
	void handleCommand_resize();
	/**
	* Performs a matrix addition with two matrices and stores the result into a variable.
	*/
	void handleCommand_add();
	/**
	* Performs a matrix subtraction with two matrices and stores the result into a variable.
	*/
	void handleCommand_sub();
	/**
	* Performs a matrix multiplication with two matrices and stores the result into a variable.
	*/
	void handleCommand_mul();
	/**
	* Scales a matrix by a scalar.
	*/
	void handleCommand_scale();
	/**
	* Transposes a matrix.
	*/
	void handleCommand_transpose();
	/**
	* Splits a matrix by rows or columns. The user can choose to return the top, bottom; left or right part of the bigger matrix.
	*/
	void handleCommand_split();
	/**
	* Merges two matrices by rows or by columns.
	*/
	void handleCommand_merge();
	/**
	* Inverts a matrix.
	*/
	void handleCommand_invert();
	/**
	* Calculates and outputs the determinant of a matrix.
	*/
	void handleCommand_det();
	/**
	* Calculates and outputs the rank of a matrix.
	*/
	void handleCommand_rank();
	/**
	* By treating a matrix of coefficients and an augmented column as a System of Linear Equations, the solvefor command outputs the solution set to the console or a text file.
	*/
	void handleCommand_solvefor();
	/**
	* Shows the value of a cell of a matrix at a row and column coordinate. The coordinates are zero based.
	*/
	void handleCommand_getcell();
	/**
	* Sets the value of a cell of a matrix at a row and a column coordinate. The coordinates are zero based.
	*/
	void handleCommand_setcell();
	/**
	* Shows the density value of a matirx.
	*/
	void handleCommand_density();
	/**
	* Shows the sparsity value of a matrix.
	*/
	void handleCommand_sparsity();
};

#endif // MATRIX_CALCULATOR_H
