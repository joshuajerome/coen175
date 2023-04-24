/** 
 * File: debug.h
 * Author: Jordan Randleman
 * 
 * Purpose: 
 *   Generate unique print statments to automatically print the file name, 
 *   func name, and line number (to stderr) that the print statement occurs at.
 * 
 * Provided Macros:
 *   1. DEBUG_LOCATION()
 *      * Print out the current location information.
 *      * Example:
 *        int main() { DEBUG_LOCATION(); } // prints: DEBUG> <file.cpp>:<main>:1
 * 
 *   2. DEBUG_EXPRESSION(X)
 *      * Print out the current location information, as well as what expression X is.
 *      * Example:
 *        int main() { DEBUG_EXPRESSION(2*3); } // prints: DEBUG> <file.cpp>:<main>:1; Expression "2*3" = 6
 *  
 * Automatically Disable the Macros:
 *   Add '#define DONE_DEBUGGING' above '#include "debug.h"' to automatically deactivate 
 *   all print statements, without having to delete/comment them from your source code.
 */


#ifndef COEN_175_DEBUG_H_PRINT_STATEMENTS
#define COEN_175_DEBUG_H_PRINT_STATEMENTS

#ifndef DONE_DEBUGGING

#include <iostream>

#define DEBUG_LOCATION()\
    (std::cerr << "DEBUG> <" << __FILE__ << ">:<" << __func__ << ">:" << __LINE__ << '\n');

#define DEBUG_EXPRESSION(...)\
    (std::cerr << "DEBUG> <" << __FILE__ << ">:<" << __func__ << ">:" << __LINE__ << "; Expression \"" << #__VA_ARGS__ << "\" = " << (__VA_ARGS__) << '\n');

#else

#define DEBUG_LOCATION()
#define DEBUG_EXPRESSION(...)

#endif /* #ifndef DONE_DEBUGGING */
#endif /* #ifndef COEN_175_DEBUG_H_PRINT_STATEMENTS */