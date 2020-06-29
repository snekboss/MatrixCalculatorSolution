#include "MatrixCalculator.h"
#include "MatCalcUtil.h"
#include <fstream>
#include <iomanip>
#include <cmath> // g++ requires it for isnan.

#ifdef _WIN32
#include <filesystem> // MSVC requires it.
namespace fs = std::filesystem;
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
#include <experimental/filesystem> // g++ requires it.
namespace fs = std::experimental::filesystem;
#endif

// Public members

MatrixCalculator::MatrixCalculator()
	: CurrentPathStr(fs::current_path().string()),
	MatrixIOPathStr(CurrentPathStr + "/" + MatrixIODirectoryName)
{
	// Paths with mixed \ and / slashes actually work (at least on Windows).
	initializeCommands();

	std::cout << std::fixed; // Propagates to every cout call.
}

void MatrixCalculator::run()
{
	std::cout << "Welcome to Matrix Calculator. Enter 'help' to see the list of commands." << std::endl << std::endl;

	while (true)
	{
		inputList = getInputList();

		if (inputList.size() < 1)
		{
			continue; // Empty line. Skip.
		}

		// Make the command string all lower case.
		std::transform(inputList[0].begin(), inputList[0].end(), inputList[0].begin(), [](unsigned char c) { return std::tolower(c); });

		auto command_KVP = commands.find(inputList[0]);

		if (command_KVP == commands.end())
		{
			std::cout << "Unrecognized command." << std::endl;
			inputList.clear();
			continue;
		}

		Command cmd = (*command_KVP).second;

		if (cmd == Command::exit)
		{
			break;
		}

		handleCommand(cmd);
	}

	std::cout << "Exiting..." << std::endl;
}

// Private members

void MatrixCalculator::initializeCommands()
{
	// Must be in sync with enum class Command until C++ stops being trash.
	commands["exit"] = Command::exit;
	commands["help"] = Command::help;
	commands["vars"] = Command::vars;
	commands["dirinfo"] = Command::dirinfo;
	commands["cls"] = Command::cls; // (hehehe windows 'cls' hEhEhE)
	commands["dims"] = Command::dims;
	commands["erase"] = Command::erase;
	commands["eraseallvars"] = Command::eraseallvars;
	commands["copy"] = Command::copy;
	commands["rename"] = Command::rename;
	commands["zero"] = Command::zero;
	commands["identity"] = Command::identity;
	commands["print"] = Command::print;
	commands["getprintprecision"] = Command::getprintprecision;
	commands["setprintprecision"] = Command::setprintprecision;
	commands["read"] = Command::read;
	commands["resize"] = Command::resize;
	commands["add"] = Command::add;
	commands["sub"] = Command::sub;
	commands["mul"] = Command::mul;
	commands["scale"] = Command::scale;
	commands["transpose"] = Command::transpose;
	commands["split"] = Command::split;
	commands["merge"] = Command::merge;
	commands["invert"] = Command::invert;
	commands["det"] = Command::det;
	commands["rank"] = Command::rank;
	commands["solvefor"] = Command::solvefor;
	commands["getcell"] = Command::getcell;
	commands["setcell"] = Command::setcell;
	commands["density"] = Command::density;
	commands["sparsity"] = Command::sparsity;
}

std::vector<std::string> MatrixCalculator::getInputList()
{
	std::vector<std::string> list;

	char c;
	std::stringstream sst;
	while (std::cin >> std::noskipws >> c)
	{
		if (c == '\n' || std::feof(stdin))
		{
			if (sst.tellp() != std::streampos(0))
			{
				// stringstream is not empty. Create string and add to the list.
				list.push_back(sst.str());
				sst = std::stringstream(); // Clear.
			}

			break;
		}

		if (isspace(c))
		{
			if (sst.tellp() != std::streampos(0))
			{
				// stringstream is not empty. Create string and add to the list.
				list.push_back(sst.str());
				sst = std::stringstream(); // Clear.
			}

			continue; // Continue regardless.
		}

		sst << c;
	}

	return list;
}

std::pair<size_t, size_t> MatrixCalculator::getMatrixCoordinates(size_t cellNumber, size_t totalNumColumns)
{
	std::pair<size_t, size_t> coords;

	size_t division = cellNumber / totalNumColumns;

	coords.first = division;
	coords.second = (cellNumber % totalNumColumns);

	return coords;
}

void MatrixCalculator::doPrint_varNameDoesNotExist(std::string varName)
{
	std::cout << "Variable '" << varName << "' does not exist." << std::endl;
}

void MatrixCalculator::doPrint_invalidInput()
{
	std::cout << "Invalid input." << std::endl;
}

void MatrixCalculator::doPrint_overwrittenExistingVariable(std::string varName)
{
	std::cout << "Overwritten existing variable '" << varName << "'." << std::endl;
}

bool MatrixCalculator::outputStringToTextFile(std::string outputString, std::string fileName)
{
	fs::path matrixioPath = MatrixIOPathStr;

	if (fs::exists(matrixioPath) == false)
	{
		std::cout << "Directory '" << MatrixIODirectoryName << "' does not exist. Creating it under path: \"" << CurrentPathStr << "\"..." << std::endl << std::endl;

		try
		{
			fs::create_directory(matrixioPath);
		}
		catch (const std::exception&)
		{
			std::cout << "Error while creating directory. Cancelling..." << std::endl;
			return false;
		}
	}

	std::cout << "Outputting to file '" << fileName << "'..." << std::endl;

	std::ofstream outfile;
	std::string outPathStr = MatrixIOPathStr + "/" + fileName; // Backward or forward slash... Apparently, doesn't matter.

	bool overwriteExistingTextFile = false;
	if (fs::exists(fs::path(outPathStr)))
	{
		overwriteExistingTextFile = true;
	}

	try
	{
		outfile.open(outPathStr);
		outfile << outputString;
	}
	catch (const std::exception&)
	{
		std::cout << "Error while writing to file." << std::endl;
		outfile.close();
		return false;
	}

	if (overwriteExistingTextFile)
	{
		std::cout << "Overwritten existing file '" << fileName << "'..." << std::endl;
	}

	outfile.close();

	return true;
}

bool MatrixCalculator::variableNameExists(std::string varName)
{
	return varName_matrix_map.find(varName) != varName_matrix_map.end();
}

bool MatrixCalculator::readStringToUInt(std::string str, size_t* out_uint)
{
	if (out_uint == nullptr)
	{
		return false;
	}

	long long int signedInt = 0;
	try
	{
		signedInt = std::stoi(str);
	}
	catch (const std::exception&)
	{
		return false;
	}

	if (signedInt < 0 || signedInt >= UINT32_MAX)
	{
		return false;
	}

	*out_uint = (size_t)signedInt;
	return true;
}

bool MatrixCalculator::readStringToDouble(std::string str, double* out_double)
{
	if (out_double == nullptr)
	{
		return false;
	}

	try
	{
		*out_double = std::stod(str);
	}
	catch (const std::exception&)
	{
		return false;
	}

	if (std::isnan(*out_double))
	{
		return false;
	}

	return true;
}

bool MatrixCalculator::readStringToLowerChar(std::string str, char* out_char)
{
	if (out_char == nullptr)
	{
		return false;
	}

	if ((str.size() != 1) || (!std::isalpha(str[0])))
	{
		return false;
	}

	*out_char = std::tolower(str[0]);
	return true;
}

bool MatrixCalculator::readDoublesFromString(std::string str, std::vector<double>* out_doubles)
{
	size_t i = 0;
	size_t strSize = str.size();
	while (i < strSize)
	{
		if (str[i] == EOF)
		{
			break;
		}

		if (isspace(str[i]) || str[i] == '\n' || str[i] == ',')
		{
			i++;
			continue;
		}

		std::stringstream doubleSST;
		while (true)
		{
			if (std::isdigit(str[i]) || str[i] == '.' || str[i] == '-')
			{
				doubleSST << str[i];
				i++;
				continue;
			}

			break;
		}

		double d;
		if (!readStringToDouble(doubleSST.str(), &d))
		{
			return false;
		}

		out_doubles->push_back(d);
	}

	return true;
}

void MatrixCalculator::handleCommand(Command cmd)
{
	switch (cmd)
	{
	case Command::exit:
		// Command::exit is handled inside 'run' method.
		break;
	case Command::help:
		handleCommand_help();
		break;
	case Command::vars:
		handleCommand_vars();
		break;
	case Command::dirinfo:
		handleCommand_dirinfo();
		break;
	case Command::cls:
		handleCommand_cls();
		break;
	case Command::dims:
		handleCommand_dims();
		break;
	case Command::erase:
		handleCommand_erase();
		break;
	case Command::eraseallvars:
		handleCommand_eraseallvars();
		break;
	case Command::copy:
		handleCommand_copy();
		break;
	case Command::rename:
		handleCommand_rename();
		break;
	case Command::zero:
		handleCommand_zero();
		break;
	case Command::identity:
		handleCommand_identity();
		break;
	case Command::print:
		handleCommand_print();
		break;
	case Command::getprintprecision:
		handleCommand_getprintprecision();
		break;
	case Command::setprintprecision:
		handleCommand_setprintprecision();
		break;
	case Command::read:
		handleCommand_read();
		break;
	case Command::resize:
		handleCommand_resize();
		break;
	case Command::add:
		handleCommand_add();
		break;
	case Command::sub:
		handleCommand_sub();
		break;
	case Command::mul:
		handleCommand_mul();
		break;
	case Command::scale:
		handleCommand_scale();
		break;
	case Command::transpose:
		handleCommand_transpose();
		break;
	case Command::split:
		handleCommand_split();
		break;
	case Command::merge:
		handleCommand_merge();
		break;
	case Command::invert:
		handleCommand_invert();
		break;
	case Command::det:
		handleCommand_det();
		break;
	case Command::rank:
		handleCommand_rank();
		break;
	case Command::solvefor:
		handleCommand_solvefor();
		break;
	case Command::getcell:
		handleCommand_getcell();
		break;
	case Command::setcell:
		handleCommand_setcell();
		break;
	case Command::density:
		handleCommand_density();
		break;
	case Command::sparsity:
		handleCommand_sparsity();
		break;
	default:
		// Do nothing.
		break;
	}
}

void MatrixCalculator::handleCommand_help()
{
	std::cout << std::endl << "--------------------------------------------------" << std::endl << std::endl;
	std::cout << "List of commands:" << std::endl;
	std::cout << "> exit\n\tExists the program." << std::endl;
	std::cout << "> help\n\tLists all commands." << std::endl;
	std::cout << "> vars\n\tLists the names of all variables." << std::endl;
	std::cout << "> dirinfo\n\tShows information about Input & Output directores & paths." << std::endl;
	std::cout << "> cls\n\tClears the screen." << std::endl;
	std::cout << "> dims <matrix>\n\tShows the dimensions of the matrix on the console.\n\texample: dims mat1" << std::endl;
	std::cout << "> erase <matrix>\n\texample: erase mat1" << std::endl;
	std::cout << "> eraseallvars\n\tErases all variables. DOES NOT ASK FOR CONFIRMATION." << std::endl;
	std::cout << "> copy <target> <source>\n\texample: copy mat2 mat1" << std::endl;
	std::cout << "> rename <oldName> <newName>\n\tRenames a matrix.\n\texample: rename mat1 mat2" << std::endl;
	std::cout << "> zero <matrix> <numRows> <numCols>\n\tCreates a zero (Sparse) matrix.\n\texample: zero mat1 3 5" << std::endl;
	std::cout << "> identity <matrix> <dimension>\n\tIdentiy matrices are square. Its number of rows and columns will be the same.\n\texample: identity mat1 5" << std::endl;
	std::cout << "> print <matrix> <option1>\n\toption1: File name. File name cannot have white spaces. The '.txt' extension will be appended automatically.\n\tIf option1 is unspecified, the program will output to the console by default.\n\texample1: print mat1\n\texample2: print mat1 mat1_output" << std::endl;
	std::cout << "> getprintprecision\n\tGets the precision value after the floating point when printing matrices." << std::endl;
	std::cout << "> setprintprecision <positiveInteger>\n\tSets the precision value after the floating point when printing matrices.\n\texample: setprecision 5" << std::endl;
	std::cout << "> read <matrix> <numRows> <numColumns>\n\texample: read mat1 4 8" << std::endl;
	std::cout << "> readf <matrix> <numRows> <numColumns> <file_name>\n\tfile_name: File name cannot have white spaces. The '.txt' extension will be appended automatically.\n\tFile must contain (numRows * numColumns) numbers.\n\texample: readf mat1 3 7 mat1_in_a_file" << std::endl;
	std::cout << "> resize <matrix> <arg1> <arg2>\n\targ1: R to resize rows; C to resize columns\n\targ2: New num rows or columns\n\texample1: resize mat1 R 5\n\texample2: resize mat1 C 3" << std::endl;
	std::cout << "> add <result> <operand1> <operand2>\n\texample: add mat3 mat1 mat2" << std::endl;
	std::cout << "> sub <result> <operand1> <operand2>\n\tSubtracts operand2 from operand1 and stores the result in result.\n\texample: sub mat3 mat1 mat2" << std::endl;
	std::cout << "> mul <result> <operand1> <operand2>\n\texample: mul mat3 mat1 mat2" << std::endl;
	std::cout << "> scale <operand> <scalar>\n\texample: scale mat1 -3.1415" << std::endl;
	std::cout << "> transpose <operand>\n\texample: transpose mat1" << std::endl;
	std::cout << "> split <result> <operand> <arg1> <arg2>\n\targ1: T for top; B for bottom; L for left; R for right.\n\targ2: If arg1 is T or B, then arg2 is 'topNumRows'. If arg1 is L or R, then arg2 is 'leftNumColumns'.\n\texample1: split mat1Top mat1 T 3\n\texample2: split mat1Bot mat B 3\n\texample3: split mat1Left mat1 L 5\n\texample4: split mat1Right mat1 R 5" << std::endl;
	std::cout << "> merge <result> <operand1> <operand2> <arg1>\n\targ1: R to merge by rows; C to merge by columns.\n\texample1: merge mat1and2 mat1 mat2 R\n\texample2: merge mat1and2 mat1 mat2 C" << std::endl;
	std::cout << "> invert <matrix>\n\texample: invert mat1" << std::endl;
	std::cout << "> det <matrix>\n\texample: det mat1" << std::endl;
	std::cout << "> rank <matrix>\n\texample: rank mat1" << std::endl;
	std::cout << "> solvefor <matrix> <augmentedColumn> <arg1> <option1>\n\taugmentedColumn: Number of columns must be 1.\n\targ1: V for verbose; C for concise.\n\toption1: File name. File name cannot have white spaces. The '.txt' extension will be appended automatically.\n\tIf option1 is unspecified, the program will output to the console by default.\n\texample1: solvefor mat1 augCol1 V\n\texample2: solvefor mat1 augCol1 C\n\texample3: solvefor mat1 augCol1 V solution_set\n\texample4: solvefor mat1 augCol1 C solution_set" << std::endl;
	std::cout << "> getcell <matrix> <row> <column>\n\tRow and column indices are zero based.\n\texample: getcell mat1 2 3" << std::endl;
	std::cout << "> setcell <matrix> <row> <column> <value>\n\tRow and column indices are zero based.\n\texample: setcell mat1 2 3 -3.1415" << std::endl;
	std::cout << "> density <matrix>\n\tOutputs a value between 0 and 1 which represents the density of the matrix.\n\texample: density mat1" << std::endl;
	std::cout << "> sparsity <matrix>\n\tOutputs a value between 0 and 1 which represents the sparsity of the matrix.\n\texample: sparsity mat1" << std::endl;
	std::cout << std::endl << "--------------------------------------------------" << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_vars()
{
	std::cout << std::endl << "--------------------------------------------------" << std::endl << std::endl;
	std::cout << "List of variables:" << std::endl;
	auto mapBegin = varName_matrix_map.begin();
	auto mapEnd = varName_matrix_map.end();
	for (auto it = mapBegin; it != mapEnd; it++)
	{
		auto KVP = (*it);
		std::cout << KVP.first << " (" << KVP.second.getNumRows() << "x" << KVP.second.getNumColumns() << ")" << std::endl;
	}

	std::cout << std::endl << "--------------------------------------------------" << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_dirinfo()
{
	std::cout << std::endl << "--------------------------------------------------" << std::endl << std::endl;
	std::cout << "Directory name for Matrix Input & Output operations: " << MatrixIODirectoryName << std::endl << std::endl;
	std::cout << "Full path to executable directory (Current Path): \"" << CurrentPathStr << "\"." << std::endl << std::endl;
	std::cout << "Full path to " << MatrixIODirectoryName << ": \"" << MatrixIOPathStr << "\"." << std::endl << std::endl;
	bool matrixIOExists = fs::exists(fs::path(MatrixIOPathStr));
	std::string answer = (matrixIOExists ? "Yes" : "No");
	std::cout << "Does directory '" << MatrixIODirectoryName << "' exist under Current Path?: " << answer << "." << std::endl << std::endl;

	std::cout << std::endl << "--------------------------------------------------" << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_cls()
{
	// Source: https://stackoverflow.com/a/52895729
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
	//std::cout << std::string(99, '\n') << std::endl;
}

void MatrixCalculator::handleCommand_dims()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	size_t numRows = varName_matrix_map[varName].getNumRows();
	size_t numCols = varName_matrix_map[varName].getNumColumns();

	std::cout << "Matrix '" << varName << "' is " << numRows << " by " << numCols << " (Has " << numRows << " rows and " << numCols << " columns.)." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_erase()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	varName_matrix_map.erase(varName);

	std::cout << "Erased '" << varName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_eraseallvars()
{
	size_t numVars = varName_matrix_map.size();
	varName_matrix_map.clear();
	std::cout << "Erased " << numVars << " variable(s)." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_copy()
{
	if (inputList.size() != 3)
	{
		doPrint_invalidInput();
		return;
	}

	std::string targetName = inputList[1];
	std::string sourceName = inputList[2];

	if (targetName == sourceName)
	{
		std::cout << "Target and source variables are identical." << std::endl;
		return;
	}

	if (!variableNameExists(sourceName))
	{
		doPrint_varNameDoesNotExist(sourceName);
		return;
	}

	bool overwriteExistingVariable = variableNameExists(targetName);

	varName_matrix_map[targetName] = varName_matrix_map[sourceName];

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(targetName);
	}

	std::cout << "Copy operation successful." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_rename()
{
	if (inputList.size() != 3)
	{
		doPrint_invalidInput();
		return;
	}

	std::string oldName = inputList[1];
	std::string newName = inputList[2];

	if (!variableNameExists(oldName))
	{
		doPrint_varNameDoesNotExist(oldName);
		return;
	}

	if (variableNameExists(newName))
	{
		std::cout << "Invalid input: Variable name '" << newName << "' already exists." << std::endl;
		return;
	}

	varName_matrix_map[newName] = varName_matrix_map[oldName];

	varName_matrix_map.erase(oldName);

	std::cout << "Successfully renamed '" << oldName << "' to '" << newName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_zero()
{
	if (inputList.size() != 4)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	bool overwriteExistingVariable = variableNameExists(varName);

	// Read dimensions
	size_t numRows = 0;
	size_t numCols = 0;
	if (!readStringToUInt(inputList[2], &numRows)
		|| !readStringToUInt(inputList[3], &numCols))
	{
		doPrint_invalidInput();
		return;
	}

	if (numRows < 1 || numCols < 1)
	{
		std::cout << "Invalid input: Dimensions cannot be smaller than 1." << std::endl;
		return;
	}

	varName_matrix_map[varName] = Matrix::createSparse(numRows, numCols);

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(varName);
	}

	std::cout << "Successfully created a zero (sparse) matrix with dimensions " << numRows << " by " << numCols << "." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_identity()
{
	if (inputList.size() != 3)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	bool overwriteExistingVariable = variableNameExists(varName);

	// Read dimension
	size_t dimension = 0;
	if (!readStringToUInt(inputList[2], &dimension))
	{
		doPrint_invalidInput();
		return;
	}

	if (dimension < 1)
	{
		std::cout << "Invalid input: Dimension cannot be smaller than 1." << std::endl;
		return;
	}

	varName_matrix_map[varName] = Matrix::createIdentity(dimension);

	if (varName_matrix_map[varName].requiresConversion())
	{
		varName_matrix_map[varName].convertToAppropriateMatrixType();
	}

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(varName);
	}

	std::cout << "Successfully created an identity matrix with dimension " << dimension << "." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_print()
{
	if (inputList.size() != 2 && inputList.size() != 3)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	std::string fileName = "";
	bool outputToFile = false;
	if (inputList.size() == 3)
	{
		fileName = inputList[2];
		fileName.append(".txt");
		outputToFile = true;
	}

	if (outputToFile)
	{
		if (outputStringToTextFile(varName_matrix_map[varName].getPrintStr(doublePrintPrecision), fileName) == true)
		{
			std::cout << "Output into '" << fileName << "' was successful." << std::endl << std::endl;
		}
		else
		{
			std::cout << "Error." << std::endl << std::endl;
		}
	}
	else
	{
		std::cout << std::endl << varName_matrix_map[varName].getPrintStr(doublePrintPrecision) << std::endl;
	}
}

void MatrixCalculator::handleCommand_getprintprecision()
{
	std::cout << "Floating point print precision is " << doublePrintPrecision << "." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_setprintprecision()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	size_t precision;
	if (!readStringToUInt(inputList[1], &precision))
	{
		doPrint_invalidInput();
		return;
	}

	if (precision < 1)
	{
		std::cout << "Invalid input: Precision cannot be smaller than 1." << std::endl;
		return;
	}

	size_t oldPrecision = doublePrintPrecision;
	doublePrintPrecision = precision;

	std::cout << "Floating point print precision set from " << oldPrecision << " to " << doublePrintPrecision << "." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_read()
{
	if (inputList.size() != 4 && inputList.size() != 5)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	bool overwriteExistingVariable = variableNameExists(varName);

	// Read dimensions
	size_t numRows = 0;
	size_t numCols = 0;
	if (!readStringToUInt(inputList[2], &numRows)
		|| !readStringToUInt(inputList[3], &numCols))
	{
		doPrint_invalidInput();
		return;
	}

	if (numRows < 1 || numCols < 1)
	{
		std::cout << "Invalid input: Dimensions cannot be smaller than 1." << std::endl;
		return;
	}

	size_t numTotalElements = numRows * numCols;
	size_t numEnteredElements = 0;

	Matrix mat = Matrix::createDense(numRows, numCols, 0);

	bool readFromConsole = true;
	std::string fileName = "";
	if (inputList.size() == 5)
	{
		fileName = inputList[4];
		fileName.append(".txt");
		readFromConsole = false;
	}

	if (readFromConsole)
	{
		bool isCancelled = false;
		while (true)
		{
			if (isCancelled)
			{
				std::cout << "Cancelling..." << std::endl;
				return;
			}

			size_t remaining = numTotalElements - numEnteredElements;

			if (remaining == 0)
			{
				break;
			}

			std::cout << "Enter " << remaining << " numbers (enter 'c' to cancel):" << std::endl;

			auto inputs = getInputList();

			if (inputs.size() == 0)
			{
				continue;
			}

			for (size_t i = 0; i < inputs.size(); i++)
			{
				if (remaining == 0)
				{
					std::cout << "All cells have been filled. Extra inputs were ignored." << std::endl;
					break;
				}

				std::string str = inputs[i];

				char ch;
				if (readStringToLowerChar(str, &ch) && (ch == 'c'))
				{
					isCancelled = true;
					break;
				}

				double d = 0;
				if (!readStringToDouble(str, &d))
				{
					doPrint_invalidInput();
					return;
				}

				auto coords = getMatrixCoordinates(numEnteredElements, numCols);
				size_t r = coords.first;
				size_t c = coords.second;

				mat.setCell(r, c, d);

				numEnteredElements++;
				remaining = numTotalElements - numEnteredElements;
			}
		}
	}
	else
	{
		fs::path matrixIOPath = MatrixIOPathStr;
		if (fs::exists(matrixIOPath) == false)
		{
			std::cout << "Directory '" << MatrixIODirectoryName << "' does not exist." << std::endl;
			std::cout << "Please create it under \"" << CurrentPathStr << "\"" << std::endl;
			std::cout << "Cancelling..." << std::endl << std::endl;
			return;
		}

		std::ifstream inputfile;
		std::string inputPathStr = MatrixIOPathStr + "/" + fileName; // Backward or forward slash... Apparently, doesn't matter.

		if (fs::exists(fs::path(inputPathStr)) == false)
		{
			std::cout << "Text file named '" << fileName << "' does not exist. Cancelling..." << std::endl << std::endl;
			return;
		}

		// Get contents from text file.
		std::stringstream contentsSST;
		try
		{
			inputfile.open(inputPathStr);
			contentsSST << inputfile.rdbuf();
			inputfile.close();
		}
		catch (const std::exception&)
		{
			std::cout << "Error while reading from file. Cancelling..." << std::endl << std::endl;
			inputfile.close();
			return;
		}

		// Read contents.
		std::vector<double> doubles;
		if (!readDoublesFromString(contentsSST.str(), &doubles))
		{
			std::cout << "Invalid text file. Cancelling..." << std::endl << std::endl;
			return;
		}

		// Check the number of doubles that were read.
		if (doubles.size() != (numRows * numCols))
		{
			std::cout << "Invalid text file: File must contain exactly " << (numRows * numCols) << " numbers (it contained " << doubles.size() << ")." << std::endl << std::endl;
			return;
		}

		// Create matrix.
		size_t i = 0;
		for (size_t r = 0; r < numRows; r++)
		{
			for (size_t c = 0; c < numCols; c++)
			{
				mat.setCell(r, c, doubles[i]);
				i++;
			}
		}

		std::cout << "Contents of '" << fileName << "' were read and stored into variable '" << varName << "'." << std::endl;
	}

	if (mat.requiresConversion())
	{
		mat.convertToAppropriateMatrixType();
	}

	varName_matrix_map[varName] = mat;

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(varName);
	}

	std::cout << "Read successful." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_resize()
{
	if (inputList.size() != 4)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	char arg1;
	if ((!readStringToLowerChar(inputList[2], &arg1)) || (std::tolower(arg1) != 'r' && std::tolower(arg1) != 'c'))
	{
		std::cout << "Invalid input: Enter 'R' for rows or 'C' for columns." << std::endl;
		return;
	}

	bool resizingRows = true;
	if (arg1 == 'c')
	{
		resizingRows = false;
	}

	// Read dimensions
	size_t arg2 = 0;
	if (!readStringToUInt(inputList[3], &arg2))
	{
		doPrint_invalidInput();
		return;
	}

	if (arg2 < 1)
	{
		std::cout << "Invalid input: Dimension cannot be smaller than 1." << std::endl;
		return;
	}

	size_t oldDimension = 0;
	std::string operationStr = "";

	if (resizingRows)
	{
		operationStr = "rows";
		oldDimension = varName_matrix_map[varName].getNumRows();
		varName_matrix_map[varName].resizeNumRows(arg2);
	}
	else
	{
		operationStr = "columns";
		oldDimension = varName_matrix_map[varName].getNumColumns();
		varName_matrix_map[varName].resizeNumColumns(arg2);
	}

	if (varName_matrix_map[varName].requiresConversion())
	{
		varName_matrix_map[varName].convertToAppropriateMatrixType();
	}

	std::cout << "Successfully resized " << operationStr << " from " << oldDimension << " to " << arg2 << "." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_add()
{
	if (inputList.size() != 4)
	{
		doPrint_invalidInput();
		return;
	}

	std::string resultName = inputList[1];
	std::string operand1Name = inputList[2];
	std::string operand2Name = inputList[3];

	if (!variableNameExists(operand1Name))
	{
		doPrint_varNameDoesNotExist(operand1Name);
		return;
	}

	if (!variableNameExists(operand2Name))
	{
		doPrint_varNameDoesNotExist(operand2Name);
		return;
	}

	bool overwriteExistingVariable = variableNameExists(resultName);

	size_t op1numRows = varName_matrix_map[operand1Name].getNumRows();
	size_t op1numCols = varName_matrix_map[operand1Name].getNumColumns();

	size_t op2numRows = varName_matrix_map[operand2Name].getNumRows();
	size_t op2numCols = varName_matrix_map[operand2Name].getNumColumns();

	if ((op1numRows != op2numRows) || (op1numCols != op2numCols))
	{
		std::cout << "Invalid input: Matrix dimensions do not match." << std::endl;
		return;
	}

	varName_matrix_map[resultName] = varName_matrix_map[operand1Name] + varName_matrix_map[operand2Name];

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(resultName);
	}

	if (varName_matrix_map[resultName].requiresConversion())
	{
		varName_matrix_map[resultName].convertToAppropriateMatrixType();
	}

	std::cout << "Added '" << operand1Name << "' to '" << operand2Name << "' and the result was stored into '" << resultName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_sub()
{
	if (inputList.size() != 4)
	{
		doPrint_invalidInput();
		return;
	}

	std::string resultName = inputList[1];
	std::string operand1Name = inputList[2];
	std::string operand2Name = inputList[3];

	if (!variableNameExists(operand1Name))
	{
		doPrint_varNameDoesNotExist(operand1Name);
		return;
	}

	if (!variableNameExists(operand2Name))
	{
		doPrint_varNameDoesNotExist(operand2Name);
		return;
	}

	bool overwriteExistingVariable = variableNameExists(resultName);

	size_t op1numRows = varName_matrix_map[operand1Name].getNumRows();
	size_t op1numCols = varName_matrix_map[operand1Name].getNumColumns();

	size_t op2numRows = varName_matrix_map[operand2Name].getNumRows();
	size_t op2numCols = varName_matrix_map[operand2Name].getNumColumns();

	if ((op1numRows != op2numRows) || (op1numCols != op2numCols))
	{
		std::cout << "Invalid input: Matrix dimensions do not match." << std::endl;
		return;
	}

	varName_matrix_map[resultName] = varName_matrix_map[operand1Name] - varName_matrix_map[operand2Name];

	if (varName_matrix_map[resultName].requiresConversion())
	{
		varName_matrix_map[resultName].convertToAppropriateMatrixType();
	}

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(resultName);
	}

	std::cout << "Subtracted '" << operand1Name << "' from '" << operand2Name << "' and the result was stored into '" << resultName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_mul()
{
	if (inputList.size() != 4)
	{
		doPrint_invalidInput();
		return;
	}

	std::string resultName = inputList[1];
	std::string operand1Name = inputList[2];
	std::string operand2Name = inputList[3];

	if (!variableNameExists(operand1Name))
	{
		doPrint_varNameDoesNotExist(operand1Name);
		return;
	}

	if (!variableNameExists(operand2Name))
	{
		doPrint_varNameDoesNotExist(operand2Name);
		return;
	}

	bool overwriteExistingVariable = variableNameExists(resultName);

	size_t op1numCols = varName_matrix_map[operand1Name].getNumColumns();

	size_t op2numRows = varName_matrix_map[operand2Name].getNumRows();

	if (op1numCols != op2numRows)
	{
		std::cout << "Invalid input: Matrix dimensions do not match." << std::endl;
		return;
	}

	varName_matrix_map[resultName] = varName_matrix_map[operand1Name] * varName_matrix_map[operand2Name];

	if (varName_matrix_map[resultName].requiresConversion())
	{
		varName_matrix_map[resultName].convertToAppropriateMatrixType();
	}

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(resultName);
	}

	std::cout << "Multiplied '" << operand1Name << "' by '" << operand2Name << "' and the result was stored into '" << resultName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_scale()
{
	if (inputList.size() != 3)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	double scalar;
	if (!readStringToDouble(inputList[2], &scalar))
	{
		doPrint_invalidInput();
		return;
	}

	varName_matrix_map[varName] = varName_matrix_map[varName] * scalar;

	if (varName_matrix_map[varName].requiresConversion())
	{
		varName_matrix_map[varName].convertToAppropriateMatrixType();
	}

	std::cout << "Successfully scaled '" << varName << "' by " << std::fixed << std::setprecision(doublePrintPrecision) << scalar << "." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_transpose()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	varName_matrix_map[varName].transpose();
	std::cout << "Successfully transposed '" << varName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_split()
{
	if (inputList.size() != 5)
	{
		doPrint_invalidInput();
		return;
	}

	std::string resultName = inputList[1];
	std::string operandName = inputList[2];

	if (!variableNameExists(operandName))
	{
		doPrint_varNameDoesNotExist(operandName);
		return;
	}

	bool overwriteExistingVariable = variableNameExists(resultName);

	// arg1 sanitization.
	char arg1;
	if ((!readStringToLowerChar(inputList[3], &arg1))
		|| (arg1 != 't' && arg1 != 'b' && arg1 != 'l' && arg1 != 'r'))
	{
		std::cout << "Invalid input: When splitting a matrix, use T for top; R for right; L for left; B for bottom matrix." << std::endl;
		return;
	}

	// arg2 sanitization
	size_t arg2;
	if (!readStringToUInt(inputList[4], &arg2))
	{
		doPrint_invalidInput();
		return;
	}

	size_t opNumRows = varName_matrix_map[operandName].getNumRows();
	size_t opNumCols = varName_matrix_map[operandName].getNumColumns();

	// Do checks for rows and columns based on which part of the matrix is requested.
	if (arg1 == 't')
	{
		if (arg2 > opNumRows)
		{
			std::cout << "Invalid input: topNumRows cannot be greater than operand's numRows." << std::endl;
			return;
		}
		else if (arg2 == 0)
		{
			std::cout << "Invalid input: topNumRows cannot be 0 when requesting the top matrix." << std::endl;
			return;
		}

		varName_matrix_map[resultName] = varName_matrix_map[operandName].splitByRow(arg2, true);
	}
	else if (arg1 == 'b')
	{
		if (arg2 > opNumRows)
		{
			std::cout << "Invalid input: topNumRows cannot be greater than operand's numRows." << std::endl;
			return;
		}
		else if (arg2 == opNumRows)
		{
			std::cout << "Invalid input: topNumRows cannot be equal to the operand's numRows when requesting the bottom matrix." << std::endl;
			return;
		}

		varName_matrix_map[resultName] = varName_matrix_map[operandName].splitByRow(arg2, false);
	}
	else if (arg1 == 'l')
	{
		if (arg2 > opNumCols)
		{
			std::cout << "Invalid input: leftNumColumns cannot be greater than operand's numColumns." << std::endl;
			return;
		}
		else if (arg2 == 0)
		{
			std::cout << "Invalid input: leftNumColumns cannot be 0 when requesting left matrix." << std::endl;
			return;
		}

		varName_matrix_map[resultName] = varName_matrix_map[operandName].splitByColumn(arg2, true);
	}
	else /*if (arg1 == 'r') */
	{
		if (arg2 > opNumCols)
		{
			std::cout << "Invalid input: leftNumColumns cannot be greater than operand's numColumns." << std::endl;
			return;
		}
		else if (arg2 == opNumCols)
		{
			std::cout << "Invalid input: leftNumColumns cannot be equal to the operand's numColumns when requesting the right matrix." << std::endl;
			return;
		}

		varName_matrix_map[resultName] = varName_matrix_map[operandName].splitByColumn(arg2, false);
	}

	if (varName_matrix_map[resultName].requiresConversion())
	{
		varName_matrix_map[resultName].convertToAppropriateMatrixType();
	}

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(resultName);
	}

	std::string opStr = "row";
	if (arg1 == 'l' || arg1 == 'r')
	{
		opStr = "column";
	}

	std::string locationStr = "";
	if (arg1 == 't')
	{
		locationStr = "top";
	}
	else if (arg1 == 'b')
	{
		locationStr = "bottom";
	}
	else if (arg1 == 'l')
	{
		locationStr = "left";
	}
	else /*if (arg1 == 'r') */
	{
		locationStr = "right";
	}

	std::cout << "Successfully split '" << operandName << "' by " << opStr << " and stored the " << locationStr << " part into matrix '" << resultName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_merge()
{
	if (inputList.size() != 5)
	{
		doPrint_invalidInput();
		return;
	}

	std::string resultName = inputList[1];
	std::string operand1Name = inputList[2];
	std::string operand2Name = inputList[3];

	if (!variableNameExists(operand1Name))
	{
		doPrint_varNameDoesNotExist(operand1Name);
		return;
	}

	if (!variableNameExists(operand2Name))
	{
		doPrint_varNameDoesNotExist(operand2Name);
		return;
	}

	bool overwriteExistingVariable = variableNameExists(resultName);

	// arg1 sanitization.
	char arg1;
	if ((!readStringToLowerChar(inputList[4], &arg1))
		|| (arg1 != 'r' && arg1 != 'c'))
	{
		std::cout << "Invalid input: Use R to merge by rows; C to merge by columns." << std::endl;
		return;
	}

	bool mergeByRows = true;
	if (arg1 == 'c')
	{
		mergeByRows = false;
	}

	size_t op1numRows = varName_matrix_map[operand1Name].getNumRows();
	size_t op1numCols = varName_matrix_map[operand1Name].getNumColumns();

	size_t op2numRows = varName_matrix_map[operand2Name].getNumRows();
	size_t op2numCols = varName_matrix_map[operand2Name].getNumColumns();

	if (mergeByRows)
	{
		// Num columns must match.
		if (op1numCols != op2numCols)
		{
			std::cout << "Invalid input: Number of columns must match." << std::endl;
			return;
		}
	}
	else
	{
		// Merging by columns. Num rows must match.
		if (op1numRows != op2numRows)
		{
			std::cout << "Invalid input: Number of rows must match." << std::endl;
			return;
		}
	}

	if (mergeByRows)
	{
		varName_matrix_map[resultName] = varName_matrix_map[operand1Name].mergeByRows(varName_matrix_map[operand2Name]);
	}
	else
	{
		varName_matrix_map[resultName] = varName_matrix_map[operand1Name].mergeByColumns(varName_matrix_map[operand2Name]);
	}

	if (varName_matrix_map[resultName].requiresConversion())
	{
		varName_matrix_map[resultName].convertToAppropriateMatrixType();
	}

	if (overwriteExistingVariable)
	{
		doPrint_overwrittenExistingVariable(resultName);
	}

	std::string opStr = "by rows";
	if (mergeByRows == false)
	{
		opStr = "by columns";
	}

	std::cout << "Successfully merged '" << operand1Name << "' with '" << operand2Name << "' " << opStr << " and stored the result in '" << resultName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_invert()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	size_t numRows = varName_matrix_map[varName].getNumRows();
	size_t numCols = varName_matrix_map[varName].getNumColumns();

	if (numRows != numCols)
	{
		std::cout << "Inversion failed: Matrix is not square." << std::endl;
		return;
	}

	double determinant = varName_matrix_map[varName].getDeterminant();

	if (mcu::doubleAlmostEqual(determinant, 0))
	{
		std::cout << "Inversion failed: Matrix '" << varName << "' is not invertible (determinant == 0)." << std::endl;
		return;
	}

	varName_matrix_map[varName] = varName_matrix_map[varName].getInverse(determinant);

	if (varName_matrix_map[varName].requiresConversion())
	{
		varName_matrix_map[varName].convertToAppropriateMatrixType();
	}

	std::cout << "Successfully inverted '" << varName << "'." << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_det()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	std::cout << "det(" << varName << ") = " << std::setprecision(doublePrintPrecision) << varName_matrix_map[varName].getDeterminant() << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_rank()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	std::cout << "rk(" << varName << ") = " << varName_matrix_map[varName].getRank() << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_solvefor()
{
	if (inputList.size() != 4 && inputList.size() != 5)
	{
		doPrint_invalidInput();
		return;
	}

	std::string matName = inputList[1];
	std::string augColName = inputList[2];

	if (!variableNameExists(matName))
	{
		doPrint_varNameDoesNotExist(matName);
		return;
	}

	if (!variableNameExists(augColName))
	{
		doPrint_varNameDoesNotExist(augColName);
		return;
	}

	size_t matNumRows = varName_matrix_map[matName].getNumRows();

	size_t augColNumRows = varName_matrix_map[augColName].getNumRows();
	size_t augColNumCols = varName_matrix_map[augColName].getNumColumns();

	if (augColNumCols != 1)
	{
		std::cout << "Invalid input: Augmented column cannot have more than 1 number of columns." << std::endl;
		return;
	}

	if (matNumRows != augColNumRows)
	{
		std::cout << "Invalid input: The matrix and the augmented column have mismatching number of rows." << std::endl;
		return;
	}

	// Verbose or concise
	char arg1;
	if ((!readStringToLowerChar(inputList[3], &arg1)) || (arg1 != 'v' && arg1 != 'c'))
	{
		std::cout << "Invalid input: For arg1, use V for verbose or C for concise." << std::endl;
		return;
	}

	bool verbose = false;
	if (arg1 == 'v')
	{
		verbose = true;
	}

	std::string solutionStr = varName_matrix_map[matName].solveFor(varName_matrix_map[augColName], verbose, doublePrintPrecision);

	// Output
	std::stringstream outputSST;
	outputSST << "Solving systems of linear equations with matrix '" << matName << "' and augmented column '" << augColName << "'..." << std::endl << std::endl;
	outputSST << solutionStr << std::endl;

	// Check if output to file.
	std::string fileName = "";
	bool outputToFile = false;
	if (inputList.size() == 5)
	{
		fileName = inputList[4];
		fileName.append(".txt");
		outputToFile = true;
	}

	if (outputToFile)
	{
		if (outputStringToTextFile(outputSST.str(), fileName) == true)
		{
			std::cout << "Output into '" << fileName << "' was successful." << std::endl << std::endl;
		}
		else
		{
			std::cout << "Error." << std::endl;
		}
	}
	else
	{
		std::cout << outputSST.str();
		std::cout << "Done!" << std::endl << std::endl;
	}
}

void MatrixCalculator::handleCommand_getcell()
{
	if (inputList.size() != 4)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	size_t row;
	size_t col;
	if (!readStringToUInt(inputList[2], &row) || !readStringToUInt(inputList[3], &col))
	{
		doPrint_invalidInput();
		return;
	}

	if (row >= varName_matrix_map[varName].getNumRows()
		|| col >= varName_matrix_map[varName].getNumColumns())
	{
		std::cout << "Invalid input: Index cannot be greater or equal to the dimensions." << std::endl;
		return;
	}

	double value = varName_matrix_map[varName].getCell(row, col);
	std::cout << varName << "[" << row << "][" << col << "] = " << std::fixed << std::setprecision(doublePrintPrecision) << value << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_setcell()
{
	if (inputList.size() != 5)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];
	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	size_t row;
	size_t col;
	if (!readStringToUInt(inputList[2], &row) || !readStringToUInt(inputList[3], &col))
	{
		doPrint_invalidInput();
		return;
	}

	if (row >= varName_matrix_map[varName].getNumRows()
		|| col >= varName_matrix_map[varName].getNumColumns())
	{
		std::cout << "Invalid input: Index cannot be greater or equal to the dimensions." << std::endl;
		return;
	}

	double value;
	if (!readStringToDouble(inputList[4], &value))
	{
		doPrint_invalidInput();
		return;
	}

	varName_matrix_map[varName].setCell(row, col, value);

	if (varName_matrix_map[varName].requiresConversion())
	{
		varName_matrix_map[varName].convertToAppropriateMatrixType();
	}

	std::cout << varName << "[" << row << "][" << col << "] = " << std::fixed << std::setprecision(2) << value << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_density()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	std::cout << "Density = " << std::fixed << std::setprecision(2) << varName_matrix_map[varName].getDensity() << std::endl << std::endl;
}

void MatrixCalculator::handleCommand_sparsity()
{
	if (inputList.size() != 2)
	{
		doPrint_invalidInput();
		return;
	}

	std::string varName = inputList[1];

	if (!variableNameExists(varName))
	{
		doPrint_varNameDoesNotExist(varName);
		return;
	}

	std::cout << "Sparsity = " << std::fixed << std::setprecision(2) << varName_matrix_map[varName].getSparsity() << std::endl << std::endl;
}
