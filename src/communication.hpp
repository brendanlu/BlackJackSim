#ifndef COMM_H
#define COMM_H

#include <array>
#include <chrono>
#include <fstream>
#include <memory>
#include <mutex>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

/*
The valid actions the simulation engine can process
*/
enum class ACTION : char 
{
    HIT = 'H',
    STAND = 'S', 
};

/*
Some logging things. 
*/
static std::string CONTEXT_STRING_1 = "Simulation Status"; 

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

/*
Simple logging class that writes into csv format.

Recieves messages and formats them into a recieving stringstream. Periodically, 
it flushes this stream into a file. To ensure that this (potentially slow) write 
process does not hold up the calling state, it presents a different recieving
stringstream, and uses a thread to write the old one to file. 
*/
class Logger 
{
public: 
    Logger() : 
        currChunk(0),
        logLevelConfig(3), 
        currShoeNum(0), 
        currTableNum(0)
    {
        dynamChunk = FLUSH_CHUNK_SIZE; 

        inStream.reset(); 
        outStream.reset(); 

        inStream = std::make_unique<std::stringstream>(); 
        outStream = std::make_unique<std::stringstream>();
    }

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
            (*inStream) << LogLabel(lt) << "," 
                        << currShoeNum  << ","
                        << currTableNum << ","
                        << c            << "," 
                        << d            << "\n";

            currChunk += 1; 
        }

        if (currChunk > FLUSH_CHUNK_SIZE) {
            ManualFlush(); 
        }
    }

    void ManualFlush() 
    {
        // ensures thread finishes last flush oepration
        if (outThread.joinable()) {
            outThread.join(); 
        }

        swapStreams();
        currChunk = 0; 

        outThread = std::thread(&Logger::outStreamToFile, this); 
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
        // complete one final write operation
        ManualFlush(); 
        outThread.join(); 

        outFile.close();

        // release stream memory
        inStream.reset(); 
        outStream.reset(); 
    }

private:
    // the logger will flush to file using its internal threaded mechanism
    // when it has a certain number of log rows in its stringstream buffer
    //
    // this is the initial message count buffer limit, but it will attempt
    // to do its own optimizations during usage
    static const int FLUSH_CHUNK_SIZE = 10000;

    int dynamChunk; 
    int currChunk; 

    std::ofstream outFile;
    int logLevelConfig;

    // recieving stream for messages into logger
    std::unique_ptr<std::stringstream> inStream; 
    // stream being written into file
    std::unique_ptr<std::stringstream> outStream;  

    // NOTE: the current design does not need the mutex
    std::mutex outStreamMutex; 
    std::thread outThread;

    std::chrono::milliseconds lastWriteTime;

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

    inline void swapStreams() 
    {
        // swap the two streams, taking care that it is not in the middle of 
        // copying and writing the output string
        // std::lock_guard<std::mutex> lock(outStreamMutex);
        std::swap(inStream, outStream); 
    }

    void outStreamToFile() 
    {
        auto start = std::chrono::system_clock::now();

        // write the stream to be outputted into the filstream
        // std::lock_guard<std::mutex> lock(outStreamMutex);
        outFile << outStream->str();
        outStream->str(""); 
        outStream->clear(); 
        outFile.flush(); 

        // record the file write time
        lastWriteTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start
        );
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