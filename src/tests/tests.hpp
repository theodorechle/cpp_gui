#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <sys/stat.h>

enum class Result {
    OK,
    KO,
    ERROR
};

/**
 * To use this class, you must create a subclass of it and use this subclass.
 * The tests must be written by overriding the 'tests' method;
 * To run the tests, call runTests.
 * Every new test method must call startTest at the beginning of the method and endTest at the end.
 * 
 * Notice that if a non-catchable error such as a segfault or a stack overflow is raised, the program will crash.
 * Also, in this case, a temporary file used by the class may not be erased.
 * This file is used to register all debug messages of a test and is read after the test execution to display it if the test fails.
 * This default behavior can be changed.
 * 
 * If it's specified to always show debug messages, all messages will be displayed when they are written.
 * If it's not, they will be shown only when a test ends.
 * If it's specified to not show debug messages, they will not be registered and shown even if it's specified to always show debug messages.
 */
class Tests {
    std::list<Result> results;
    std::string debugFileName = "debugFile";
    std::ofstream debugFile = std::ofstream();
    bool testsRunning = false;
    bool showDebugMessages = true;
    bool alwaysShowDebugMessages = false;
    std::streambuf *oldErrBuffer = nullptr;
    std::streambuf *oldOutBuffer = nullptr;

    bool openFile();
    bool closeFile();

    /**
     * Starts a test session
     */
    bool startTestSession();

    /**
     * Ends a test session
     */
    bool endTestSession();

    size_t getTestNumber() const { return results.size(); }

    std::string resultToStr(Result result) const;
    void displayLastResult();
    void displaySummary() const;

    void redirectStandardOutput();
    void redirectErrorOutput();
    void resetStandardOutput();
    void resetErrorOutput();

protected:
    void startTest();
    void endTest();
    void setTestResult(Result r);
    std::string getFileContent(std::string fileName) const;
public:
    bool runTests();
    
    virtual void tests() = 0;

    /**
     * By default the file name is "debugFile" but you can change it by calling this method with a new file name.
     * Note that this file will only be used if tests are not running and if the file does not already exists.
     */
    void setDebugFile(const std::string &newFileName);

    /**
     * If false, will never show debug messages even if specified to always show debug messages.
     */
    void showDebug(bool show);
    bool showDebug() const;

    /**
     * If true, every debug message will be written, all messages will be displayed when they are written.
     * If false, they will be shown only when a test ends.
     */
    void alwaysShowDebug(bool alwaysShow);
    bool alwaysShowDebug() const;

    int getNbErrors() const;
};