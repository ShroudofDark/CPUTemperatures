#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

#include "parseTemps.h"
#include "DataPreProcessor.h"
#include "PiecewiseLinearInterpolation.h"
#include "LeastSquaresApproximation.h"

using namespace std;

using CoreTempReading = std::pair<int, std::vector<double>>;
using SlopeAndIntercept = std::pair<double, double>;

void outputOrganizer(const string& core0Output, const string& core1Output, 
                     const string& core2Output, const string& core3Output, const string& inputFileName) {
    
    string fn = inputFileName;
    //Get only the filename of the base file (no extensions)
    size_t extensionSpot = fn.find_last_of('.');
    if (extensionSpot != std::string::npos) {
        if (extensionSpot > 1) {
            fn = fn.substr(0, extensionSpot - 1);
        }
    }

    std::ofstream core0Out(fn + "-core-0.txt");
    core0Out << core0Output;
    core0Out.close();

    std::ofstream core1Out(fn + "-core-1.txt");
    core1Out << core1Output;
    core1Out.close();

    std::ofstream core2Out(fn + "-core-2.txt");
    core2Out << core2Output;
    core2Out.close();

    std::ofstream core3Out(fn + "-core-3.txt");
    core3Out << core3Output;
    core3Out.close();
}

int main(int argc, char** argv)
{
    // Input validation
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " input_file_name" << "\n";
        return 1;
    }

    ifstream input_temps(argv[1]);
    if (!input_temps) {
        cout << "ERROR: " << argv[1] << " could not be opened" << "\n";
        return 2;
    }
    // End Input Validation

    // vector
    auto readings = parse_raw_temps<std::vector<CoreTempReading>>(input_temps);
    DataPreProcessor processedData(readings);

    //Declare variables
    PiecewiseLinearInterpolation interpolationCalculator;
    LeastSquaresApproximation leastSquareCalculator;

    std::vector<SlopeAndIntercept> core0LineParts;
    std::vector<SlopeAndIntercept> core1LineParts;
    std::vector<SlopeAndIntercept> core2LineParts;
    std::vector<SlopeAndIntercept> core3LineParts;

    SlopeAndIntercept core0SquareApprox;
    SlopeAndIntercept core1SquareApprox;
    SlopeAndIntercept core2SquareApprox;
    SlopeAndIntercept core3SquareApprox;

    //Initialize Variables
    interpolationCalculator.Calculate(core0LineParts, processedData.GetTimes(), processedData.GetCoreReadings(0));
    interpolationCalculator.Calculate(core1LineParts, processedData.GetTimes(), processedData.GetCoreReadings(1));
    interpolationCalculator.Calculate(core2LineParts, processedData.GetTimes(), processedData.GetCoreReadings(2));
    interpolationCalculator.Calculate(core3LineParts, processedData.GetTimes(), processedData.GetCoreReadings(3));

    core0SquareApprox = leastSquareCalculator.Calculate(processedData.GetTimes(), processedData.GetCoreReadings(0));
    core1SquareApprox = leastSquareCalculator.Calculate(processedData.GetTimes(), processedData.GetCoreReadings(1));
    core2SquareApprox = leastSquareCalculator.Calculate(processedData.GetTimes(), processedData.GetCoreReadings(2));
    core3SquareApprox = leastSquareCalculator.Calculate(processedData.GetTimes(), processedData.GetCoreReadings(3));

    //Output information
    string core0Report = interpolationCalculator.ToString(core0LineParts, processedData.GetTimes()) 
                            + leastSquareCalculator.ToString(core0SquareApprox, processedData.GetTimes());
    string core1Report = interpolationCalculator.ToString(core1LineParts, processedData.GetTimes())
                            + leastSquareCalculator.ToString(core1SquareApprox, processedData.GetTimes());
    string core2Report = interpolationCalculator.ToString(core2LineParts, processedData.GetTimes())
                            + leastSquareCalculator.ToString(core2SquareApprox, processedData.GetTimes());
    string core3Report = interpolationCalculator.ToString(core3LineParts, processedData.GetTimes())
                            + leastSquareCalculator.ToString(core3SquareApprox, processedData.GetTimes());

    outputOrganizer(core0Report, core1Report, core2Report, core3Report, argv[1]);
}