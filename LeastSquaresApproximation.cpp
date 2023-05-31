#include "LeastSquaresApproximation.h"

//--------------------- Private Functions -----------------------//

/**
 * Will set up the Matrices x, y, xT, xTx,xTy
 *
 * @param times provides list of the times
 * @param temps provides list of the temps of the target core
 */
void LeastSquaresApproximation::Setup(const std::vector<int>& times, const std::vector<double>& temps) {
	//Initialize x
	for (int i = 0; i < times.size(); i++) {
		std::vector<double> rowStorage;
		rowStorage.push_back(1);
		rowStorage.push_back(times[i]);
		x.push_back(rowStorage);
	}
	//Initialize y
	for (int i = 0; i < temps.size(); i++) {
		std::vector<double> rowStorage;
		rowStorage.push_back(temps[i]);
		y.push_back(rowStorage);
	}
	//Initialize xT
	xT = Transpose(x);
	//Initialize xTx and xTy
	xTx = MatrixDotProduct(xT, x);
	xTy = MatrixDotProduct(xT, y);
}

/**
 * Will transpose a provide Matrix
 *
 * @param toTranspose is the Matrix we wish to transpose
 *
 * @return the transpose Matrix
 *
 * @pre Matrix is implemented as vector<vector<double>>
 */
Matrix LeastSquaresApproximation::Transpose(const Matrix& toTranspose) {
	Matrix retVal;
	//Column Num -> Reminder we decided Matrix is row outside column inside
	for (int j = 0; j < toTranspose[j].size(); j++) {
		std::vector<double> columnStore;
		//Row Num
		for (int i = 0; i < toTranspose.size(); i++) {
			columnStore.push_back(toTranspose[i][j]);
		}
		//Push the column as a row
		retVal.push_back(columnStore);
	}
	return retVal;
}

/**
 * Will multiply two Matrices together. The dot product follows the rules of:
 *
 * m x n * n x p = m x p
 *
 * Where m x n and n x p are the row x column dimensions of the matrix.
 *
 * @param lhs is the Matrix of the left side to be multiplied
 * @param rhs is the Matrix of the right side to be multiplied
 *
 * @return Matrix that is the m x p ; the dot product of the provided Matrices
 *
 * @pre Matrix is implemented as vector<vector<double>>
 * @pre lhs column # == rhs row #
 */
Matrix LeastSquaresApproximation::MatrixDotProduct(const Matrix& lhs, const Matrix& rhs) {
	Matrix retVal;
	//Row of lhs moves down last (m)
	for (int lhsRowNum = 0; lhsRowNum < lhs.size(); lhsRowNum++) {
		std::vector<double> rowStore;
		//Column of rhs moves before row of lhs, but after calcs (p)
		for (int rhsColumnNum = 0; rhsColumnNum < rhs[rhsColumnNum].size(); rhsColumnNum++) {
			double val = 0.0;
			//Column of lhs and row of rhs move in sync (n)
			for (int n = 0; n < rhs.size(); n++) {
				val += lhs[lhsRowNum][n] * rhs[n][rhsColumnNum];
			}
			rowStore.push_back(val);
		}
		retVal.push_back(rowStore);
	}
	return retVal;
}

/**
 * Takes a matrix on the left and an augmented vector (aka verticle vector)
 * and solves it before performing row operations until the left hand matrix is
 * an identity matrix and the augmented vector is changed by those operations.
 *
 * @param lhsMatrix takes the matrix to perform row operations on (nxn)
 * @param augVector takes the augmented vector to peform row operations on (nx1)
 *
 * @return a augmented vector with the updated values
 */
Matrix LeastSquaresApproximation::SolveMatrix(const Matrix& lhsMatrix, const Matrix& augVector) {
	//Store for editting
	Matrix retVector = augVector;
	Matrix solvingMatrix = lhsMatrix;
	
	for (int i = 0; i < lhsMatrix.size(); i++) {
		Pivot(solvingMatrix, retVector, i, i);
		Scale(solvingMatrix, retVector, i, i);
		Eliminate(solvingMatrix, retVector, i, i);
	}
	BackEliminate(solvingMatrix, retVector);
	return retVector;
}

/**
 * Searches through all the rows from the startRow to the end of the Matrix
 * looking for the largest number in the indicated column index. If a row has
 * the largest number it will swap it with the startRow.
 *
 * @param lhsMatrix takes the matrix to perform row operations on
 * @param augVector takes the augmented vector to peform row operations on
 * @param startRow indicates which row to start at and move on from
 * @param columnIndex indicates which column we are using for reference
 */
void LeastSquaresApproximation::Pivot(Matrix& lhsMatrix, Matrix& augVector, int startRow, int columnIndex) {
	//Find largest
	double max = lhsMatrix[startRow][columnIndex];
	int maxRow = startRow;
	for (int i = startRow; i < lhsMatrix.size(); i++) {
		double check = lhsMatrix[i][columnIndex];
		if (max < check) {
			max = check;
			maxRow = i;
		}
	}
	//Swap if needed
	if (maxRow != startRow) {
		//Swap left matrix first
		std::vector<double> tempRow;
		tempRow = lhsMatrix[startRow];
		lhsMatrix[startRow] = lhsMatrix[maxRow];
		lhsMatrix[maxRow] = tempRow;

		//Swap the aug vector to match
		tempRow = augVector[startRow];
		augVector[startRow] = augVector[maxRow];
		augVector[maxRow] = tempRow;
	}
}

/**
 * Scales a row within the Matrix and augVector based on the inverse of the number
 * indicated within the column and row. (The number at [rowToScale][columnIndex] = 1
 * after this)
 *
 * @param lhsMatrix takes the matrix to perform row operations on
 * @param augVector takes the augmented vector to peform row operations on
 * @param rowToScale indicates which row scale
 * @param columnIndex indicates which column we are using for for the base number
 */
void LeastSquaresApproximation::Scale(Matrix& lhsMatrix, Matrix& augVector, int rowToScale, int columnIndex) {
	double scalar = lhsMatrix[rowToScale][columnIndex];
	for (int i = 0; i < lhsMatrix[rowToScale].size(); i++) {
		lhsMatrix[rowToScale][i] = lhsMatrix[rowToScale][i] / scalar;
	}
	//Help with precision errors
	lhsMatrix[rowToScale][columnIndex] = 1.0;

	for (int i = 0; i < augVector[rowToScale].size(); i++) {
		augVector[rowToScale][i] = augVector[rowToScale][i] / scalar;
	}
}

/**
 * Takes the provided column index and the source row in order to a subtract them
 * from the follow rows and to eliminate them. The column indicated by column index will
 * be all 0's except for the source row.
 *
 * @param lhsMatrix takes the matrix to perform row operations on
 * @param augVector takes the augmented vector to peform row operations on
 * @param sourceRow indicates which row to use as the basis
 * @param columnIndex indicates which column we are using for reference
 */
void LeastSquaresApproximation::Eliminate(Matrix& lhsMatrix, Matrix& augVector, int sourceRow, int columnIndex) {
	int startColumn = columnIndex;
	for (int i = sourceRow + 1; i < lhsMatrix.size(); i++) {
		double scalar = lhsMatrix[i][startColumn];
		for (int j = startColumn + 1; j < lhsMatrix[i].size(); j++) {
			lhsMatrix[i][j] = lhsMatrix[i][j] - (scalar * lhsMatrix[sourceRow][j]);
		}
		augVector[i][0] = augVector[i][0] - (scalar * augVector[sourceRow][0]);
		lhsMatrix[i][startColumn] = 0;
	}
}

/**
 * Works similar to Eliminate. It will start from the bottom row of the matrix
 * and work backwards to eliminate remaining numbers in the matrix that are not
 * the 1's that were solved for already. Only need to look at upper half triangle
 * due to bottom triangle already being 0's.
 *
 * @param lhsMatrix takes the matrix to perform row operations on
 * @param augVector takes the augmented vector to peform row operations on
 */
void LeastSquaresApproximation::BackEliminate(Matrix& lhsMatrix, Matrix& augVector) {
	int lastRowNum = lhsMatrix.size()-1;
	int lastColNum = lhsMatrix[lastRowNum].size()-1;
	for (int i = lastRowNum-1; i >= 0; i--) {
		double scalar = lhsMatrix[i][lastColNum];
		for (int j = lastColNum - 1; j >= 0; j--) {
			lhsMatrix[i][j] = lhsMatrix[i][j] - (scalar * lhsMatrix[i+1][lastColNum]);
		}
		augVector[i][0] = augVector[i][0] - (scalar * augVector[i+1][0]);
		lastColNum--;
	}
}

//--------------------- Public Functions -----------------------//

/**
 * Calculates all the slope (c1) and intercept (c0) for the least squares-approximation
 *
 * @param times provides list of the times
 * @param temps provides list of the temps of the target core
 *
 * @return a std::pair<double, double> that contains c1 and c0 respectively
 *
 * @pre Assumes temps[i] associates with times[i]
 */
SlopeAndIntercept LeastSquaresApproximation::Calculate(const std::vector<int>& times, const std::vector<double>& temps) {
	Setup(times, temps);
	Matrix solved = SolveMatrix(xTx,xTy);
	double c1 = solved[1][0];
	double c0 = solved[0][0];
	SlopeAndIntercept retVal(c1, c0);
	return retVal;
}

//Need a toString method like the piecewise in order to print a line
//Format minTime <= x < maxTime; y = c0 + c1x; least-squares

/**
 * Provides a formatted line of the linear global least squares approximation for a core as a String
 * Line is formatted as such:
 *
 * minTime <= x < maxTime; y = c0 + c1x; least-squares
 *
 * @param coreSquareApprox provides the slope (c1) and intercept (c0) for the approximation
 * @param times provides the limits of the approximation
 *
 * @return string to be used in output
 */
const std::string LeastSquaresApproximation::ToString(const SlopeAndIntercept& coreSquareApprox, const std::vector<int>& times) {
	std::stringstream retVal;
	//Sets significant figs
	retVal << std::setprecision(4) << std::fixed;
	int spacing = 8;

	retVal << std::right << std::setfill(' ') << std::setw(spacing) << times[0] << " <= x <"
		   << std::right << std::setfill(' ') << std::setw(spacing) << times[times.size()-1] << "; y "
		<< std::left << std::setfill(' ') << std::setw(spacing) << " " << " = "
		   << std::right << std::setfill(' ') << std::setw(spacing + (spacing / 2)) << coreSquareApprox.second << " + "
		   << std::right << std::setfill(' ') << std::setw(spacing + (spacing / 2)) << coreSquareApprox.first << "x; least-squares"
		   << "\n";

	return retVal.str();
}