#ifndef COMM_H
#define COMM_H

#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
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
    SHOE,
    LOGGER
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
it flushes this stream into a file. 

To ensure that this (potentially slow) write process does not hold up the 
calling state, it presents a different recieving stringstream, and uses a thread 
to write the old one to file. 

It will also attempt to balance its write frequency such that the writer thread 
is kept as busy as possible relative to the logging load. 
*/
class Logger 
{
public: 
    Logger() : 
        currChunk(0),
        adjust(true), 
        logLevelConfig(3), 
        lastLogChunkTime(0),
        lastWriteTime(0),
        currShoeNum(0), 
        currTableNum(0)
    {
        dynamChunk = INIT_CHUNK_SIZE; 

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
        // record the start time of a chunk to get a rough idea of how 
        // frequently messages come in
        if (currChunk == 0) {
            logChunkStart = std::chrono::system_clock::now();
        }

        if (static_cast<int>(ll) <= logLevelConfig) {
            (*inStream) << LogLabel(lt) << "," 
                        << currShoeNum  << ","
                        << currTableNum << ","
                        << c            << "," 
                        << d            << "\n";

            currChunk += 1; 
        }

        // dynamChunk may or may not have been adjusted in the other thread
        // and may or may not change between this section of code and the next
        // chunk adjustment method call
        //
        // keep a clear record of what chunk size the timing corresponds to
        if (currChunk > dynamChunk) {
            recordedChunkSize = dynamChunk; 
            lastLogChunkTime = 
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() - logChunkStart
                );
            ManualFlush(); 
        }
    }

    void ManualFlush() 
    {
        // ensures thread finishes last flush oepration
        if (outThread.joinable()) {
            outThread.join(); 
        }

        SwapStreams();
        currChunk = 0; 

        outThread = std::thread(&Logger::OutStreamToFile, this); 
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
    static const int INIT_CHUNK_SIZE = 10000;

    int dynamChunk; 
    int currChunk; 

    // turn on/off dynamic chunk adjustment logic
    bool adjust; 

    std::ofstream outFile;
    int logLevelConfig;

    // recieving stream for messages into logger
    std::unique_ptr<std::stringstream> inStream; 
    // stream being written into file
    std::unique_ptr<std::stringstream> outStream;  

    // NOTE: the current design does not need the mutex
    std::mutex outStreamMutex; 
    std::thread outThread;

    // this tells us what chunk size the last timings relate to 
    int recordedChunkSize; 

    std::chrono::system_clock::time_point logChunkStart; 
    std::chrono::microseconds lastLogChunkTime;
    std::chrono::milliseconds lastWriteTime;

    inline std::string LogLabel(LOG_TYPE lt) 
    {
        switch (lt) {
            case LOG_TYPE::AGENT  : return "[A]"; 
            case LOG_TYPE::DEALER : return "[D]";
            case LOG_TYPE::ENGINE : return "[E]"; 
            case LOG_TYPE::SHOE   : return "[S]";
            case LOG_TYPE::LOGGER : return "[!]"; 
            default               : return "[.]";
        }
    }

    inline void SwapStreams() 
    {
        // swap the two streams, taking care that it is not in the middle of 
        // copying and writing the output string
        // std::lock_guard<std::mutex> lock(outStreamMutex);
        std::swap(inStream, outStream);
    }

    // CALLED IN THREAD
    // ----------------
    void OutStreamToFile() 
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

        if (adjust) {
            DynamChunkAdjust();
        }

        return; 
    }

    // CALLED IN THREAD
    // ----------------
    void DynamChunkAdjust()
    {
        int NON_TRIVIAL_CHUNK = 5000; 

        // go for adjustment if %change > tol
        double TOL = 0.05; 

        double tWrite = static_cast<double>(lastWriteTime.count()); 
        double tRatio; 

        int idealChunkSize;

        // ensure nontrivial chunk and no division by 0
        if (recordedChunkSize > NON_TRIVIAL_CHUNK && tWrite > 0) {
            // ideally want the following ratio to be around 1
            // this means that the writer thread is constantly active
            // but does not cause the logger to block the calling state (much)
            //
            // will generally favour larger and larger write loads
            //
            // if too small, the writer thread is too busy 
            // if too large, the writer thread is too idle
            tRatio = static_cast<double>(lastLogChunkTime.count()) / tWrite;
            idealChunkSize = std::round(tRatio * recordedChunkSize);

            // now compare to existing dynamChunk (which may or may not be the
            // same as the recordedChunkSize)
            if (abs(idealChunkSize - dynamChunk) / dynamChunk > TOL) {
                WriteRow(
                    LOG_LEVEL::DETAIL, 
                    LOG_TYPE::LOGGER,
                    "Log Chunk Adjustment", 
                    "Chunk size adjusted from " 
                        + std::to_string(dynamChunk)
                        + " to "
                        + std::to_string(idealChunkSize)
                );
                dynamChunk = idealChunkSize; 
            }
            else {
                // assuming some sense of constancy in future logging load
                //
                // this 'NON_TRIVIAL_CHUNK' 
                adjust = false; 
            }
        }

        return; 
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