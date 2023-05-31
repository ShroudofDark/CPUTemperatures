/**
 * The Data Pre-Processor class is designed to take the input data
 * and process it into a more usable form that can be accessed by 
 * other classes.
 * 
 * @author Jacob McFadden
 */
#ifndef DATA_PRE_PROCESSOR_H_INCLUDED
#define DATA_PRE_PROCESSOR_H_INCLUDED

#include <vector>

using CoreTempReading = std::pair<int, std::vector<double>>;

class DataPreProcessor
{
private: 
	
	const int NUM_CORES = 4; //!< Expected number of cores we are reading from

	std::vector<int> timeReadings = {}; //!< A list of when the core times were read
	std::vector<double> readingsCore0 = {};//!< A list of temperature readings for core 0 : ordered by time acquired
	std::vector<double> readingsCore1 = {}; //!< A list of temperature readings for core 1 : ordered by time acquired
	std::vector<double> readingsCore2 = {}; //!< A list of temperature readings for core 2 : ordered by time acquired
	std::vector<double> readingsCore3 = {}; //!< A list of temperature readings for core 3 : ordered by time acquired

public:

	/**
	 * Construct a pre-processor object that can be used to access data in easy to use format
	 * 
	 * @param readings is input container (vector of pair(int,vector<double>))
	 * 
	 * @pre vector<double> size = 4 (4 cores read)
	 */
	DataPreProcessor(const std::vector<CoreTempReading>& readings);

	/**
	 * Fetches all the readings of one specific core
	 *
	 * @param coreNum specifies which core readings to return
	 * 
	 * @return a container of all the temperature readings of the specific core
	 * 
	 * @pre coreNum >= 0 && coreNum < 4
	 */
	const std::vector<double>& GetCoreReadings(int coreNum) const;

	/**
	 * Fetches all the times the readings took place at
	 *
	 * @return a container of all the times readings occured
	 */
	const std::vector<int>& GetTimes() const { return timeReadings; }
};
#endif
