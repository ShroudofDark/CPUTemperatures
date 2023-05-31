#include "DataPreProcessor.h"

/**
 * Construct a pre-processor object that can be used to access data in easy to use format
 *
 * @param readings is input container (vector of pair(int,vector<double>))
 *
 * @pre vector<double> size = 4 (4 cores read)
 */
DataPreProcessor::DataPreProcessor(const std::vector<CoreTempReading>& readings) {
	int numReadings = readings.size();
	std::vector<double> temps;
	for (int i = 0; i < numReadings; i++) {
		timeReadings.push_back(readings[i].first);
		temps = readings[i].second;

		readingsCore0.push_back(temps[0]);
		readingsCore1.push_back(temps[1]);
		readingsCore2.push_back(temps[2]);
		readingsCore3.push_back(temps[3]);
	}
}

/**
 * Fetches all the readings of one specific core
 *
 * @param coreNum specifies which core readings to return
 *
 * @return a container of all the temperature readings of the specific core
 * 
 * @pre coreNum >= 0 && coreNum < 4
 */
const std::vector<double>& DataPreProcessor::GetCoreReadings(int coreNum) const{
	switch(coreNum) {
		case 0:
			return readingsCore0;
		case 1:
			return readingsCore1;
		case 2:
			return readingsCore2;
		case 3:
			return readingsCore3;
		default:
			return {}; //Provide empty as default. Empty would indicate error.
	}
}