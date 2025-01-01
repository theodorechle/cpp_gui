#include "tests.hpp"

bool Tests::openFile() {
    logFile.open(logFileName);
    if (!logFile.is_open()) {
        std::cerr << "Can't create file '" << logFileName << "' used for logging\n";
        return false;
    }
    return true;
}

bool Tests::closeFile() {
    if (logFile.is_open()) {
        logFile.close();
    }
    return true;
}

bool Tests::startTestSession() {
    testsRunning = true;
    if (!showLogMessages || alwaysShowLogMessages) return true;

    struct stat buffer;
    if (stat(logFileName.c_str(), &buffer) != -1) {
        std::cerr << "File '" << logFileName << "' who should have been used for logging already exists\n";
        return false;
    }

    std::cout << "Started test session \"" << testsName << "\"\n";
    return true;
}

bool Tests::endTestSession() {
    resetErrorOutput();
    resetStandardOutput();
    testsRunning = false;
    std::cout << "Ended test session \"" << testsName << "\"\n";
    displaySummary();

    // if nothing was written, the file wasn't created
    if (std::filesystem::exists(logFileName) && remove(logFileName.c_str())) {
        std::cerr << "Can't delete file '" << logFileName << "'\n";
        return false;
    }
    return true;
}

void Tests::redirectStandardOutput() {
    oldOutBuffer = std::cout.rdbuf(logFile.rdbuf());
}
void Tests::redirectErrorOutput() {
    oldErrBuffer = std::cerr.rdbuf(logFile.rdbuf());
}

void Tests::resetStandardOutput() {
    std::cerr.rdbuf(oldErrBuffer);
}

void Tests::resetErrorOutput() {
    std::cout.rdbuf(oldOutBuffer);
}

void Tests::startTest() {
    openFile();
    if (!alwaysShowLogMessages) {
        redirectStandardOutput();
        redirectErrorOutput();
    }
    std::cout << "Test n°" << getTestNumber() << ":\n";
}

void Tests::endTest() {
    std::stringstream buffer;
    if (!alwaysShowLogMessages) {
        resetStandardOutput();
        resetErrorOutput();
    }
    if (!resultEntered) setTestResult(Result::UNKNOWN);
    resultEntered = false;
    if (logFile.is_open() && results.back() != Result::OK) {
        std::ifstream file(logFileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::cerr << buffer.str();
    }
    closeFile();
    openFile(); // reopen it to erase the existing content
}

bool Tests::runTests() {
    bool testsValid = true;
    if (!startTestSession()) return false;
    try {
        tests();
    }
    catch (const std::exception &exception) {
        endTest();
        std::cerr << "Tests stopped after an exception was raised: " << exception.what() << "\n";
        testsValid = false;
    }
    catch (...) {
        endTest();
        std::cerr << "Tests stopped after an exception was raised who is not a subclass of std::exception\n";
        testsValid = false;
    }

    if (!endTestSession()) return false;
    return testsValid;
}

void Tests::setTestResult(Result r) {
    results.push_back(r);
    resultEntered = true;
}

std::string Tests::getFileContent(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string Tests::resultToStr(Result result) const {
    switch (result) {
    case Result::OK:
        return "OK";
    case Result::KO:
        return "KO";
    case Result::ERROR:
        return "ERROR";
    case Result::UNKNOWN:
        return "No result";
    default:
        return "Not valid result";
    }
}

void Tests::displayLastResult() {
    std::cout << resultToStr(results.back());
}

void Tests::setLogFile(const std::string &newFileName) {
    if (!testsRunning) logFileName = newFileName;
}

void Tests::showLogs(bool show) {
    if (!testsRunning) showLogMessages = show;
}

bool Tests::showLogs() const {
    return showLogMessages;
}

void Tests::alwaysShowLogs(bool alwaysShow) {
    if (!testsRunning) alwaysShowLogMessages = alwaysShow;
}

bool Tests::alwaysShowLogs() const {
    return alwaysShowLogMessages;
}
void Tests::displaySummary() const {
    std::cout << "Summary:\n";
    size_t index = 0;
    for (std::list<Result>::const_iterator it = results.cbegin(); it != results.cend(); it++) {
        std::cout << "\tTest n°" << index << ": " << resultToStr(*it) << "\n";
        index++;
    }
    std::cout << "End of summary\n";
}

int Tests::getNbErrors() const {
    return results.size() - std::count(results.cbegin(), results.cend(), Result::OK);
}