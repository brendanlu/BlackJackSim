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

class Logger 
{
public: 
    Logger() : 
        currShoeNum(0), 
        currHandNum(0)
    {}

    /*
    Pass in name of output file to write into
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
    void WriteRow(LOG_TYPE lt, const std::string& c, const std::string& d)
    {
        // colHeaders = "Source,ShoeNum,HandNum,Context,Detail\n";
        outFile << LogLabel(lt) << "," 
                << currShoeNum  << ","
                << currHandNum  << ","
                << c            << "," 
                << d            << "\n";
    }

    void ManualFlush() 
    {
        outFile.flush(); 
    }

    void FreshShuffleHandler()
    {
        currShoeNum += 1; 
    }

    void Clearhandler() 
    {
        currHandNum += 1; 
    }

    ~Logger() 
    {
        outFile.close();
    }

private: 
    std::ofstream outFile;

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
    std::string colHeaders = "Source,ShoeNum,HandNum,Context,Detail\n";

    int currShoeNum; 
    int currHandNum; 
};

#endif