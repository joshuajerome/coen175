/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include <string>
# include "Scope.h"

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareSymbol(const std::string &name, const Type &type);

Symbol *checkIdentifier(const std::string &name);

// 2.2.1
bool checkDecls(const Declarators &decls);

// 2.2.3
Type checkLogicalExpression(const Type &left, const Type &right);

// 2.2.4
Type checkEqualityandRelational(const Type &left, const Type &right, const std::string name);

// 2.2.5
Type checkAdd(const Type &left, const Type &right, const std::string name);
Type checkSub(const Type &left, const Type &right, const std::string name);

bool notFunc(const Type &t);
bool checkTs(const Type &left, const Type &right);

// 2.2.6
Type checkMultiplicativeExpression(const Type &left, const Type &right, const std::string name);

// 2.2.7
Type checkLogicalNot(const Type &left, const std::string name);
Type checkNegate(const Type &right, const std::string name);
Type checkDeref(const Type &right, const std::string name);
Type checkAddr(const Type &right, const bool &lvalue);
Type checkSizeOf(const Type &right, const std::string name);
Type checkCast(const Type &left);

// 2.2.8
Type checkParen(const Type& left);
Type checkArray(const Type &left, const Type &right, const std::string name);
Type checkFunc(const Type &left, const Types &args);

// 2.2.9
Type checkReturn(const Type &left, const Type &right);
void checkBreak(int counter);

void checkAssignment(const Type& left, const Type &right, const bool &lvalue);

# endif /* CHECKER_H */
