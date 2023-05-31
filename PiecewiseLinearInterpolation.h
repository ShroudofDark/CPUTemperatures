/**
 * The Piecewise Line Interpolation class will take a data set of one core
 * and produce a y = mx + b for every consecutive point from the provided 
 * readings (i.e. 0-30, 30-60, 60-90, ...).
 * 
 * @author Jacob McFadden
 */
#ifndef PIECEWISE_LINEAR_INTERPOLATION_H_INCLUDED
#define PIECEWISE_LINEAR_INTERPOLATION_H_INCLUDED

#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>

using SlopeAndIntercept = std::pair<double, double>;

class PiecewiseLinearInterpolation
{
private:
	/**
 	 * Helper function to calculate slope of line between two points
	 * i.e. m of y = mx + b
	 *
	 * @param x0 is the lower time reading
	 * @param x1 is the higher time reading
	 * @param y0 is temp reading associated with lower time reading
	 * @param y1 is temp reading associated with higher time reading
	 * 
	 * @return a double that represents the slope
	 */
	const double CalculateSlope(const int& x0, const int& x1, const double& y0, const double& y1);
	
	/**
	 * Helper function to calculate the y-intercept of a line
	 * i.e. b of y = mx + b
	 * 
	 * @param x time of reading 
	 * @param y temp associated with the time reading
	 * @param slope the slope of the line (aka m)
	 * 
	 * @return a double that represents the y-intercept
	 */
	const double CalculateYIntercept(const int& x, const double& y, const double& slope);

public:

	/**
	 * Calculates all the slopes and y-intercepts of the provided core readings and times.
	 * 
	 * @param coreLineParts container for slopes and y-intercepts to pass in as reference, function updates it with values
	 * @param times provides list of the times
	 * @param temps provides list of the temps of the target core
	 * 
	 * @pre Assumes temps[i] associates with times[i]
	 */
	void Calculate(std::vector<SlopeAndIntercept>& coreLineParts, const std::vector<int>& times, const std::vector<double>& temps);

	/**
	 * Provides a formatted list of the piecewise interpolations for a core as a String
	 * Each line follows a format akin to:
	 *
	 * time1 <= x < time2; y_# = b + mx; interpolation
	 *
	 * @param coreLineParts provides the slope and y-intercept for each interpolation
	 * @param times provides the limits of the interpolation
	 *
	 * @return string to be used in output
	 */
	const std::string ToString(const std::vector<SlopeAndIntercept>& coreLineParts, const std::vector<int>& times);
};
#endif