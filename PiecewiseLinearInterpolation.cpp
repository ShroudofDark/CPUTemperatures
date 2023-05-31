#include "PiecewiseLinearInterpolation.h"

//--------------------- Private Functions -----------------------//

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
const double PiecewiseLinearInterpolation::CalculateSlope(const int& x0, const int& x1, const double& y0, const double& y1) {
	double retVal = 0.0;
	retVal = (y1 - y0) / (x1 - x0);
	return retVal;
}
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
const double PiecewiseLinearInterpolation::CalculateYIntercept(const int& x, const double& y, const double& slope) {
	double retVal = 0.0;
	retVal = y - (slope * x);
	return retVal;
}

//--------------------- Public Functions -----------------------//

/**
 * Calculates all the slopes and y-intercepts of the provided core readings and times.
 *
 * @param coreLineParts container for slopes and y-intercepts to pass in as reference, function updates it with values
 * @param times provides list of the times
 * @param temps provides list of the temps of the target core
 *
 * @pre Assumes temps[i] associates with times[i]
 */
void PiecewiseLinearInterpolation::Calculate(std::vector<SlopeAndIntercept>& coreLineParts, const std::vector<int>& times, const std::vector<double>& temps) {
	int counterCap = 0;
	if (times.size() == temps.size()) {
		counterCap = temps.size();
	}
	for (int i = 0; i < counterCap - 1; i++) {
		double slope;
		double yIntercept;

		int time0 = times[i];
		int time1 = times[i + 1];
		double temp0 = temps[i];
		double temp1 = temps[i + 1];

		slope = CalculateSlope(time0, time1, temp0, temp1);
		yIntercept = CalculateYIntercept(time0, temp0, slope);

		SlopeAndIntercept interpolationParts(slope, yIntercept);
		coreLineParts.push_back(interpolationParts);
	}
}

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
const std::string PiecewiseLinearInterpolation::ToString(const std::vector<SlopeAndIntercept>& coreLineParts, const std::vector<int>& times) {
	std::stringstream retVal;

	//Sets significant figs
	retVal << std::setprecision(4) << std::fixed;

	int spacing = 8;
	int countCap = times.size();

	for (int i = 0; i < countCap - 1; i++) {
		retVal << std::right << std::setfill(' ') << std::setw(spacing) << times[i] << " <= x <" 
			   << std::right << std::setfill(' ') << std::setw(spacing) << times[i+1] << "; y_"
			   << std::left  << std::setfill(' ') << std::setw(spacing) << i << " = "
			   << std::right << std::setfill(' ') << std::setw(spacing + (spacing / 2)) << coreLineParts[i].second << " + "
			   << std::right << std::setfill(' ') << std::setw(spacing + (spacing / 2)) << coreLineParts[i].first << "x; interpolation"
			   << "\n";
	}
	return retVal.str();
}