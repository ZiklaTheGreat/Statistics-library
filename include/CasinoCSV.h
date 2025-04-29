#ifndef __CASINOCSV_H__
#define __CASINOCSV_H__

#include "../lib/include/CSVConverter.h"
#include "../lib/include/Writer.h"
#include "../lib/include/Reader.h"
#include "../lib/include/CSVFileOut.h"
#include "../lib/include/CSVFileIn.h"

#include <string>
#include <sstream>
#include <iomanip>

class CasinoCSVConverter : public CSVConverter<double>
{
public:
    std::string convert(const double &data) override
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2); // 6 decimal places
        stream << data;
        return stream.str();
    }

    double convert(const std::string &data) override
    {
        return std::stod(data);
    }
};

class CasinoCSVWriter : public Writer<double, CasinoCSVConverter, CSVFileOut>
{
public:
    CasinoCSVWriter() : Writer() {}
    CasinoCSVWriter(const std::string &pFile) : Writer(pFile) {}
};

class CasinoCSVReader : public Reader<double, CasinoCSVConverter, CSVFileIn>
{
public:
    CasinoCSVReader() : Reader() {}
    CasinoCSVReader(const std::string &pFile) : Reader(pFile) {}
};

#endif // __CASINOCSV_H__