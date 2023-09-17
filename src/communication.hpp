#ifndef COMM_H
#define COMM_H

#include <array>
#include <fstream>
#include <iostream>
#include <string>

/*
The valid actions the simulation engine can process
*/
enum class ACTION : char 
{
    HIT = 'H',
    STAND = 'S', 
};

enum class ERR_CODE : int 
{
    SUCCESS = 0
};

/*
Really simple logging class, which is basically a wrapper of std::ofstream atm.

Writes into csv format.
*/
enum class LOG_TYPE : int
{
    AGENT,
    DEALER,
    ENGINE,
    SHOE
};

enum class LOG_LEVEL : int
{
    NONE = 0, 
    BASIC = 1,
    DETAIL = 2, 
    VERBOSE = 3
};

class Logger 
{
public: 
    Logger() : 
        logLevelConfig(3), 
        currShoeNum(0), 
        currTableNum(0)
    {}

    /*
    Controls the level of information logged. Affects simulation speed. 
    */
    void SetLogLevel(int ll) 
    {
        if (ll > 3) {
            ll = 3; 
        }
        if (ll < 0) {
            ll = 0; 
        }

        logLevelConfig = ll; 
    }

    /*
    Pass in name of output file to write into. 
    This will open the file and write + flush the data column headers in. 
    */
    void InitLogFile(const std::string& filename) 
    {
        outFile.open(filename, std::ios::out); 
        outFile << colHeaders;
        outFile.flush();
    }

    /*
    Boolean value to indicate valid configuration and ready for use 
    */
    operator bool() const 
    {
        return outFile.is_open(); 
    }

    /*
    Write a new csv row. 
    */
    inline void WriteRow(
        LOG_LEVEL ll, LOG_TYPE lt, 
        const std::string& c, const std::string& d
    )
    {
        if (static_cast<int>(ll) <= logLevelConfig) {
            outFile << LogLabel(lt) << "," 
                    << currShoeNum  << ","
                    << currTableNum << ","
                    << c            << "," 
                    << d            << "\n";
        }
    }

    void ManualFlush() 
    {
        outFile.flush(); 
    }

    inline void FreshShuffleHandler()
    {
        currShoeNum += 1; 
    }

    inline void Clearhandler() 
    {
        currTableNum += 1; 
    }

    ~Logger() 
    {
        outFile.close();
    }

private: 
    std::ofstream outFile;
    int logLevelConfig; 

    inline std::string LogLabel(LOG_TYPE lt) 
    {
        switch (lt) {
            case LOG_TYPE::AGENT : return "[A]"; 
            case LOG_TYPE::DEALER: return "[D]";
            case LOG_TYPE::ENGINE: return "[E]"; 
            case LOG_TYPE::SHOE  : return "[S]";
            default              : return "[.]";
        }
    }
    
    /*
    The csv data is written in a stacked-like format (long but not wide). 
    This makes subsequent analysis much easier; column filters can be applied
    to get relevant information, whilst the ordering of the data is preserved in 
    the overall log. Horizontal merging operations can also be performed using 
    the identifer columns, to 'de-stack' data. 

    The current output format is as follows:

        |Source | Where the log record comes from, in [X] format
        |ShoeNum| Which number shoe it was from
        |HandNum| Which hand in the shoe it was from
        |Context| What game event the log comes from
        |Detail | This field will change drastically, depending on context
    */
    const std::string colHeaders = "Source,ShoeNum,TableNum,Context,Detail\n";

    int currShoeNum; 
    int currTableNum; 
};

#endif