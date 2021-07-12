#ifndef ERRORCHECKER_H
#define ERRORCHECKER_H

#include <string>

class ErrorChecker {
public:
    /**
     * Prints the current OpenGL error codes. "prefix" should be a string that indicates
     * a location in your code, such as "line 100" or "after glClear".
     */
    static void printGLErrors(std::string prefix);
};

#endif // ERRORCHECKER_H
