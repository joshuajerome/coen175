/*
 * Function:	checker.cpp
 *
 * Description: This file contains the public and private function and
 *				variable definitions for the semantic checker for Simple C.
 *
 *				If a symbol is redeclared, the redeclaration is discarded
 *				and the original declaration is retained, although in the
 *				case of a function definition, the parameters are updated
 *				so long as the declaration implied by the definition is not
 *				erroneous.
 *
 *				Extra functionality:
 *				- inserting an undeclared symbol with the error type
 */

# include <map>
# include <cassert>
# include <iostream>
# include "lexer.h"
# include "tokens.h"
# include "checker.h"

using namespace std;

static map <string, Type> externs;
static Scope *current, *globals;
static const Type error(ERROR);

static const Type integer(INT);
static const Type character(CHARACTER);

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";

static string E1 = "'%s' has invalid type";
static string E2 = "break statement not within loop";
static string E3 = "invalid return type";
static string E4 = "lvalue required in expression";
static string E5 = "invalid operands to binary %s";
static string E6 = "invalid operand to unary %s";
static string E7 = "invalid type in cast expression";
static string E8 = "called object is not a function";
static string E9 = "invalid arguments to called function";


/*
 * Function:	openScope
 *
 * Description: Create a scope and make it the new current scope.
 */

Scope *openScope()
{
	current = new Scope(current);

	if (globals == nullptr)
		globals = current;

	return current;
}


/*
 * Function:	closeScope
 *
 * Description: Remove the current scope, and make its enclosing scope the
 *				new current scope.
 */

Scope *closeScope()
{
	Scope *old = current;
	current = current->enclosing();
	return old;
}

/*
 * Function:	defineFunction
 *
 * Description: Define a function with the specified NAME and TYPE.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
	cout << "define " << name << " as " << type << endl;

	checkDecls(type.declarators());
	Symbol *symbol = globals->find(name);

	if (symbol == nullptr) {
		if (externs.count(name) && externs.at(name) != type)
			report(conflicting, name);
		else
			externs.insert(make_pair(name, type));

		symbol = new Symbol(name, type);
		globals->insert(symbol);

	} else {
		if (symbol->type().isFunction() && symbol->type().parameters())
			report(redefined, name);
		else if (type != symbol->type())
			report(conflicting, name);
		else {
			symbol = new Symbol(name, type);
			globals->remove(name);
			globals->insert(symbol);
		}
	}

	return symbol;
}


/*
 * Function:	declareSymbol
 *
 * Description: Declare a symbol with the specified NAME and TYPE.	Any
 *				redeclaration is discarded.
 */

Symbol *declareSymbol(const string &name, const Type &type)
{
	cout << "declare " << name << " as " << type << endl;

	checkDecls(type.declarators());
	Symbol *symbol = current->find(name);

	if (symbol == nullptr) {
		if (current == globals || type.isFunction()) {
			if (externs.count(name) && externs.at(name) != type)
				report(conflicting, name);
			else
				externs.insert(make_pair(name, type));
		}

		symbol = new Symbol(name, type);
		current->insert(symbol);

	} else if (current != globals && !type.isFunction())
		report(redeclared, name);

	else if (type != symbol->type())
		report(conflicting, name);

	return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description: Check if NAME is declared.	If it is undeclared, then
 *				declare it as having the error type in order to eliminate
 *				future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
	Symbol *symbol = current->lookup(name);

	if (symbol == nullptr) {
		report(undeclared, name);
		symbol = new Symbol(name, error);
		current->insert(symbol);
	}

	return symbol;
}

// 2.2.1
bool checkDecls(const Declarators &decls) {
	for (auto it = decls.begin(); it != decls.end(); it++) {
		if (it->kind() != POINTER && next(it) != decls.end()) {
			if (it->kind() == ARRAY && next(it)->kind() == FUNCTION) {
				return false;
			} else if (it->kind() == FUNCTION && next(it)->kind() != POINTER)
				return false;
		} 
	}
	return true;
}

// 2.2.3
Type checkLogicalExpression(const Type &left, const Type &right) {
	if (left == error || right == error) {
		return error;
	}
	return integer;
}

// 2.2.4
Type checkEqualityandRelational(const Type &left, const Type &right) {
	
	Type left_type = left.promote(), right_type = right.promote();

	if (left_type == error || right_type == error)
		return error;

	if (left_type != right_type) {
		report(E5);
		return error;
	}

	return integer;
}

// 2.2.5
Type checkAdd(const Type &left, const Type &right) {

	Type left_type = left.promote(), right_type = right.promote();
	
	// both are integer
	if (left_type == integer && right_type == integer)
		return integer;

	if (left_type.isPointer() && (right_type == integer) && notFunc(left_type))
		return left_type;
	if ((left_type == integer) && right_type.isPointer() && notFunc(right_type))
		return right_type;

	report(E5);
	return error;
}

Type checkSub(const Type &left, const Type &right) {

	Type left_type = left.promote(), right_type = right.promote();

	// both are integer
	if (left_type == integer && right_type == integer) 
		return integer;

	// left is pointer to T and right is type int
	if (left_type.isPointer() && (right_type == integer) && notFunc(left_type)) 
		return left_type;

	// both are pointer to the same T
	if (left_type.isPointer() && right_type.isPointer() && 
		checkTs(left_type, right_type) &&
		notFunc(left_type) && notFunc(right_type)) {
			return integer;
		}
	
	report(E5);
	return error;
}

bool notFunc(const Type &t) {
	auto it = t.declarators().begin();
	it++;
	return it->kind() != FUNCTION;
}

bool checkTs(const Type &left, const Type &right) {
	auto itLeft = left.declarators().begin(), 
		itRight = right.declarators().begin();
	itLeft++, itRight++;
	return itLeft == itRight;
}

// 2.26
Type checkMultiplicativeExpression(const Type &left, const Type &right) {

	Type left_type = left.promote(), right_type = right.promote();

	if ((left_type == integer) && (right_type == integer)) return integer;

	report(E5);
	return error;
}

// 2.2.7
Type checkLogicalNot(const Type &right) {
	Type right_type = right.promote();
	if (right_type == integer)
		return integer;
	return error; // and is NOT an lvalue
}

Type checkNegate(const Type &right) {
	Type right_type = right.promote();
	if (right_type == integer)
		return integer;
	report(E6);
	return error; // and is NOT an lvalue
}

Type checkDeref(const Type &right) {
	Type right_type = right.promote();
	if (right_type.isPointer()) {
		Declarators d = right_type.declarators();
		d.pop_front();
		return Type(right_type.specifier(), d); // and is an lvalue
	}
	report(E6);
	return error;
}

Type checkAddr(const Type &right) {
	bool lvalue = false;
	// check is lvalue
	if (lvalue) {
		Declarators d = right.declarators();
		d.push_front(Pointer());
		return Type(right.specifier(), d); // and is NOT an lvalue
	}
	return error;	
}

Type checkSizeOf(const Type &right) {
	if (notFunc(right)) return integer;
	report(E6);
	return error; // and is NOT an lvalue
}

Type checkCast(const Type &left) {
	Type left_type = left.promote();
	if (checkDecls(left_type.declarators()) && !left_type.isArray() && !left_type.isFunction()) {
		return left_type; // and is not an lvalue
	}
	return error;
}





