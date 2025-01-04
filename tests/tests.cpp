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

void Tests::redirectStandardOutput() { oldOutBuffer = std::cout.rdbuf(logFile.rdbuf()); }
void Tests::redirectErrorOutput() { oldErrBuffer = std::cerr.rdbuf(logFile.rdbuf()); }

void Tests::resetStandardOutput() { std::cerr.rdbuf(oldErrBuffer); }

void Tests::resetErrorOutput() { std::cout.rdbuf(oldOutBuffer); }

void Tests::startTest(const std::string &testName) {
    openFile();
    if (!alwaysShowLogMessages) {
        redirectStandardOutput();
        redirectErrorOutput();
    }
    std::cout << "Test n°" << getTestNumber();
    results.push_back(std::pair(testName, Result::NOT_GIVEN));
    if (!results.back().first.empty()) {
        std::cout << " (" << results.back().first << ")";
    }
    std::cout << ":\n";
}

void Tests::endTest() {
    std::stringstream buffer;
    if (!alwaysShowLogMessages) {
        resetStandardOutput();
        resetErrorOutput();
    }
    resultEntered = false;
    if (logFile.is_open() && results.back().second != Result::OK) {
        std::ifstream file(logFileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::cerr << buffer.str() << "\n";
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
        if (resultEntered) results.pop_back();
        setTestResult(Result::ERROR);
        endTest();
        std::cerr << "Tests stopped after an exception was raised: " << exception.what() << "\n";
        testsValid = false;
    }
    catch (...) {
        if (resultEntered) results.pop_back();
        setTestResult(Result::ERROR);
        endTest();
        std::cerr << "Tests stopped after an exception was raised who is not a subclass of std::exception\n";
        testsValid = false;
    }

    if (!endTestSession()) return false;
    return testsValid;
}

void Tests::setTestResult(Result r) {
    results.back().second = r;
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
    case Result::NOT_GIVEN:
        return "NOT GIVEN";
    default:
        return "UNKNOWN";
    }
}

void Tests::setLogFile(const std::string &newFileName) {
    if (!testsRunning) logFileName = newFileName;
}

void Tests::showLogs(bool show) {
    if (!testsRunning) showLogMessages = show;
}

bool Tests::showLogs() const { return showLogMessages; }

void Tests::alwaysShowLogs(bool alwaysShow) {
    if (!testsRunning) alwaysShowLogMessages = alwaysShow;
}

bool Tests::alwaysShowLogs() const { return alwaysShowLogMessages; }
void Tests::displaySummary() const {
    std::cout << "Summary:\n";
    size_t index = 0;
    std::string resultStr;
    for (std::list<std::pair<std::string, Result>>::const_iterator it = results.cbegin(); it != results.cend(); it++) {
        resultStr = resultToStr(it->second);
        std::cout << "\tTest n°" << index << ": " << resultStr;
        if (!it->first.empty()) {
            std::cout << std::string(nbSpacesBeforeTestDescription - resultStr.size(), ' ') << " (" << it->first << ")";
        }
        std::cout << "\n";
        index++;
    }
    std::cout << "End of summary\n";
}

int Tests::getNbErrors() const {
    int nbErrors = 0;
    for (std::pair<std::string, Result> result : results) {
        if (result.second != Result::OK) nbErrors++;
    }
    return nbErrors;
}