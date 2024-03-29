/*
 * Function:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		If a symbol is redeclared, the redeclaration is discarded
 *		and the original declaration is retained, although in the
 *		case of a function definition, the parameters are updated
 *		so long as the declaration implied by the definition is not
 *		erroneous.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
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

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";


/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new current scope.
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
 * Description:	Remove the current scope, and make its enclosing scope the
 *		new current scope.
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
 * Description:	Define a function with the specified NAME and TYPE.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    cout << "define " << name << " as " << type << endl;

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
 * Description:	Declare a symbol with the specified NAME and TYPE.  Any
 *		redeclaration is discarded.
 */

Symbol *declareSymbol(const string &name, const Type &type)
{
    cout << "declare " << name << " as " << type << endl;

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
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
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
