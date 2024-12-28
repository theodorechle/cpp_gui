#include "tests.hpp"

bool Tests::openFile() {
    debugFile.open(debugFileName);
    if (!debugFile.is_open()) {
        std::cerr << "Can't create file '" << debugFileName << "' used for debug\n";
        return false;
    }
    return true;
}

bool Tests::closeFile() {
    if (debugFile.is_open()) {
        debugFile.close();
    }
    return true;
}

bool Tests::startTestSession() {
    testsRunning = true;
    if (!showDebugMessages || alwaysShowDebugMessages) return true;

    struct stat buffer;
    if (stat(debugFileName.c_str(), &buffer) != -1) {
        std::cerr << "File '" << debugFileName << "' who should have been used for debug already exists\n";
        return false;
    }

    std::cerr << "Started test session\n";
    return true;
}

bool Tests::endTestSession() {
    testsRunning = false;
    std::cerr << "Ended test session\n";
    if (showDebugMessages) {
        displaySummary();
    }
    if (remove(debugFileName.c_str())) {
        std::cerr << "Can't delete file '" << debugFileName << "'\n";
        return false;
    }
    return true;
}

void Tests::redirectStandardOutput() {
    oldOutBuffer = std::cout.rdbuf(debugFile.rdbuf());
}
void Tests::redirectErrorOutput() {
    oldErrBuffer = std::cerr.rdbuf(debugFile.rdbuf());
}

void Tests::resetStandardOutput() {
    std::cerr.rdbuf(oldErrBuffer);
}

void Tests::resetErrorOutput() {
    std::cout.rdbuf(oldOutBuffer);
}

void Tests::startTest() {
    openFile();
    if (!alwaysShowDebugMessages) {
        redirectStandardOutput();
        redirectErrorOutput();
    }
    std::cout << "Test n°" << getTestNumber() << ":\n";
}

void Tests::endTest() {
    std::stringstream buffer;
    if (!alwaysShowDebugMessages) {
        resetStandardOutput();
        resetErrorOutput();
    }
    if (debugFile.is_open() && results.back() != Result::OK) {
        std::ifstream file(debugFileName);
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
    catch (std::exception &exception) {
        std::cerr << "Tests stopped after an exception was raised: " << exception.what() << "\n";
        testsValid = false;
    }
    catch (...) {
        std::cerr << "Tests stopped after an exception was raised who is not an subclass of std::exception\n";
        testsValid = false;
    }

    if (!endTestSession()) return false;
    return testsValid;
}

void Tests::setTestResult(Result r) {
    results.push_back(r);
}

std::string Tests::getFileContent(std::string fileName) const {
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
    default:
        return "Invalid result";
    }
}

void Tests::displayLastResult() {
    std::cout << resultToStr(results.back());
}

void Tests::setDebugFile(const std::string &newFileName) {
    if (!testsRunning) debugFileName = newFileName;
}

void Tests::showDebug(bool show) {
    if (!testsRunning) showDebugMessages = show;
}

bool Tests::showDebug() const {
    return showDebugMessages;
}

void Tests::alwaysShowDebug(bool alwaysShow) {
    if (!testsRunning) alwaysShowDebugMessages = alwaysShow;
}

bool Tests::alwaysShowDebug() const {
    return alwaysShowDebugMessages;
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