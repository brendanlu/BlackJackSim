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
    Logger () {};

    /*
    Pass in name of output file to write into
    */
    void InitLogToFile(const std::string& filename) 
    {
        outFile.open(filename); 
        outFile << colHeaders;
        outFile.flush();
    }

    void Row(LOG_TYPE lt, const std::string& c, const std::string& d)
    {
        outFile << LogLabel(lt) << "," << c << "," << d << "\n";
    }

    ~Logger() 
    {
        outFile.close();
    }

private: 
    std::ofstream outFile;
    
    /*
    The csv data is written in a stacked-like format (long but not wide). 
    This makes subsequent analysis much easier; column filters can be applied
    to get relevant information, whilst the ordering of the data is preserved in 
    the overall log. 

    The current output format is as follows:

        |Source | Where the log record comes from, in [X] format
        |Context| What game event the log comes from
        |Detail | This field will change drastically, depending on context
    */
    std::string colHeaders = "Source,Context,Detail\n";

    inline std::string LogLabel(LOG_TYPE lt) 
    {
        switch (lt) {
            case LOG_TYPE::AGENT : return "[A]"; 
            case LOG_TYPE::DEALER: return "[D]";
            case LOG_TYPE::ENGINE: return "[E]"; 
            case LOG_TYPE::SHOE  : return "[S]";
        }
    }
};

#endif