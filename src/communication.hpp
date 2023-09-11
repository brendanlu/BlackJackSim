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
Simple logging class which is easy to wrap in Cython. 
Write into csv format, enabling post-sim data analysis. 
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
    }

    void Record()
    {
        
    }

    ~Logger() 
    {
        outFile.close();
    }
private: 
    std::ofstream outFile;
    std::string colHeaders = "Source,Card,Context,Action,";

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