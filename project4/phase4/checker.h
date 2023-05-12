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
Type checkEqualityandRelational(const Type &left, const Type &right);

// 2.2.5
Type check_ADD(const Type &left, const Type &right);
Type check_SUB(const Type &left, const Type &right);

bool notFunc(const Type &t);
bool checkTs(const Type &left, const Type &right);

// 2.2.6
Type checkMultiplicativeExpression(const Type &left, const Type &right);

// 2.2.7
Type checkLogicalNot(const Type &left);
Type checkNegate(const Type &right);
Type checkDeref(const Type &right);
Type checkAddr(const Type &right);
Type checkSizeOf(const Type &right);
Type checkCast(const Type &left);

# endif /* CHECKER_H */
