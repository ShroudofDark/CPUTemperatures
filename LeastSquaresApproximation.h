/**
 * The Least Square Approximation class will take a data set of one core
 * and find a linear approximation using a discrete calculation of the provided
 * data (global) to calculate a y = c0 + c1x.
 *
 * @author Jacob McFadden
 */
#ifndef LEAST_SQUARES_APPROXIMATION_H_INCLUDED
#define LEAST_SQUARES_APPROXIMATION_H_INCLUDED

#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>

using SlopeAndIntercept = std::pair<double, double>;
using Matrix = std::vector<std::vector<double>>; //Outside vector = row, inside = column

class LeastSquaresApproximation
{
private:

	Matrix x, y, xT, xTx, xTy; //!< List of Matrices to be used in calculations

	/**
	 * Will set up the Matrices x, y, xT, xTx,xTy
	 * 
	 * @param times provides list of the times
	 * @param temps provides list of the temps of the target core
	 */
	void Setup(const std::vector<int>& times, const std::vector<double>& temps);

	/**
	 * Will transpose a provide Matrix
	 * 
	 * @param toTranspose is the Matrix we wish to transpose
	 * 
	 * @return the transpose Matrix
	 * 
	 * @pre Matrix is implemented as vector<vector<double>>
	 */
	Matrix Transpose(const Matrix& toTranspose);

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
	Matrix MatrixDotProduct(const Matrix& lhs, const Matrix& rhs);

	//--------- Matrix Solvers ---------//
	
	/**
	 * Takes a matrix on the left and an augmented vector (aka verticle vector)
	 * and solves it before performing row operations until the left hand matrix is
	 * an identity matrix and the augmented vector is changed by those operations.
	 * 
	 * @param lhsMatrix takes the matrix to perform row operations on
	 * @param augVector takes the augmented vector to peform row operations on
	 * 
	 * @return a augmented vector with the updated values
	 */
	Matrix SolveMatrix(const Matrix& lhsMatrix, const Matrix& augVector);
	
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
	void Pivot(Matrix& lhsMatrix, Matrix& augVector, int startRow, int columnIndex);

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
	void Scale(Matrix& lhsMatrix, Matrix& augVector, int rowToScale, int columnIndex);

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
	void Eliminate(Matrix& lhsMatrix, Matrix& augVector, int sourceRow, int columnIndex);
	
	/**
	 * Works similar to Eliminate. It will start from the bottom row of the matrix
	 * and work backwards to eliminate remaining numbers in the matrix that are not
	 * the 1's that were solved for already. Only need to look at upper half triangle
	 * due to bottom triangle already being 0's. 
	 *
	 * @param lhsMatrix takes the matrix to perform row operations on
	 * @param augVector takes the augmented vector to peform row operations on
	 */
	void BackEliminate(Matrix& lhsMatrix, Matrix& augVector);
public:

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
	SlopeAndIntercept Calculate(const std::vector<int>& times, const std::vector<double>& temps);

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
	const std::string ToString(const SlopeAndIntercept& coreSquareApprox, const std::vector<int>& times);
};
#endif