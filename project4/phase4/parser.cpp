/*
 * File:		parser.cpp
 *
 * Description: This file contains the public and private function and
 *				variable definitions for the recursive-descent parser for
 *				Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "checker.h"
# include "tokens.h"
# include "lexer.h"

# define NEIGHBORS 69420

using namespace std;

static int lookahead, nexttoken;
static string lexbuf, nextbuf;

Type expression(bool &lvalue);
static Types *parameters();
static void statement();

static const Type integer(INT);
static Type returnType(INT);
static int breakCounter = 0;

enum { PLAIN_DECL, FUNCTION_DECL, ABSTRACT_DECL };


/*
 * Function:	error
 *
 * Description: Report a syntax error to standard error.
 */

static void error()
{
	if (lookahead == DONE)
		report("syntax error at end of file", "");
	else
		report("syntax error at '%s'", lexbuf);

	exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description: Match the next token against the specified token.  A
 *				failure indicates a syntax error and will terminate the
 *				program since our parser does not do error recovery.
 */

static void match(int t)
{
	if (lookahead != t)
		error();

	if (nexttoken) {
		lookahead = nexttoken;
		lexbuf = nextbuf;
		nexttoken = 0;
	} else
		lookahead = lexan(lexbuf);
}


/*
 * Function:	peek
 *
 * Description: Return the next token in the input stream and save it so
 *				that match() will later return it.
 */

static int peek()
{
	if (!nexttoken)
		nexttoken = lexan(nextbuf);

	return nexttoken;
}


/*
 * Function:	number
 *
 * Description: Match the next token as a number and return its value.
 */

static unsigned number()
{
	string buf;


	buf = lexbuf;
	match(NUM);
	return strtol(buf.c_str(), NULL, 0);
}


/*
 * Function:	isSpecifier
 *
 * Description: Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
	return token == INT || token == CHAR;
}


/*
 * Function:	specifier
 *
 * Description: Parse a type specifier.	 Simple C has only int and char.
 *
 *				specifier:
 *				  int
 *				  char
 */

static int specifier()
{
	int typespec = lookahead;

	if (isSpecifier(lookahead))
		match(lookahead);
	else
		error();

	return typespec;
}


/*
 * Function:	declarator
 *
 * Description: Parse all types of declarator: plain, function, and
 *				abstract.  The kind of declarator expected is indicated via
 *				the parameter.
 *
 *				declarator:
 *				  identifier
 *				  * declarator
 *				  declarator ( )
 *				  declarator [ num ]
 *				  ( declarator )
 *
 *				function-declarator:
 *				  identifier ( parameters )
 *				  * function-declarator
 *				  function-declarator ( )
 *				  function-declarator [ num ]
 *				  ( function-declarator )
 *
 *				abstract-declarator:
 *				  empty
 *				  * abstract-declarator
 *				  abstract-declarator ( )
 *				  abstract-declarator [ num ]
 *				  ( abstract-declarator )
 *
 *				Perhaps it is bad style to parse all declarators in one
 *				function, but the parsing logic is so similar, as the type
 *				construction logic will be, that one function reduces the
 *				development effort.	 Also, the first such declarator at the
 *				global level must be treated specially in order to
 *				distinguish between a function definition and a function or
 *				variable declaration, so a function such as this one is
 *				needed anyway.
 */

static bool declarator(Declarators &decls, string &name, int kind = PLAIN_DECL)
{
	bool hasparams = false;


	if (lookahead == '*') {
		match('*');
		hasparams = declarator(decls, name, kind);
		decls.push_front(Pointer());

	} else {
		if (lookahead == '(' && peek() != ')') {
			match('(');
			hasparams = declarator(decls, name, kind);
			match(')');

		} else if (kind != ABSTRACT_DECL) {
			name = lexbuf;
			match(ID);

			if (kind == FUNCTION_DECL && lookahead == '(' && peek() != ')') {
				match('(');
				openScope();
				decls.push_front(Function(parameters()));
				hasparams = true;
				match(')');
			}
		}

		while (1) {
			if (lookahead == '(') {
				match('(');
				match(')');
				decls.push_front(Function(nullptr));

			} else if (lookahead == '[') {
				match('[');
				decls.push_front(Array(number()));
				match(']');

			} else
				break;
		}
	}

	return hasparams;
}


/*
 * Function:	decl
 *
 * Description: Convenience function for calling declarator and reversing
 *				the declarators so we can use them.
 */

bool decl(Declarators &decls, string &name, int kind = PLAIN_DECL)
{
	bool hasparams;

	decls.clear();
	hasparams = declarator(decls, name, kind);
	decls.reverse();

	return hasparams;
}


/*
 * Function:	declaration
 *
 * Description: Parse a variable or function declaration.
 *
 *				declaration:
 *				  specifier declarator-list ';'
 *
 *				declarator-list:
 *				  declarator
 *				  declarator , declarator-list
 */

static void declaration()
{
	int typespec;
	Declarators decls;
	string name;


	typespec = specifier();
	decl(decls, name);
	declareSymbol(name, Type(typespec, decls));

	while (lookahead == ',') {
		match(',');
		decl(decls, name);
		declareSymbol(name, Type(typespec, decls));
	}

	match(';');
}


/*
 * Function:	declarations
 *
 * Description: Parse a possibly empty sequence of declarations.
 *
 *				declarations:
 *				  empty
 *				  declaration declarations
 */

static void declarations()
{
	while (isSpecifier(lookahead))
		declaration();
}


/*
 * Function:	parameter
 *
 * Description: Parse a parameter, which is simply a specifier followed by
 *				a declarator.  Parameters in Simple C are only specified as
 *				part of a function definition so a plain declarator rather
 *				than an abstract declarator is used since the identifier is
 *				always required.
 *
 *				parameter:
 *				  specifier declarator
 */

static void parameter(Types *params)
{
	int typespec;
	Declarators decls;
	string name;


	typespec = specifier();
	decl(decls, name);

	if (!decls.empty() && decls.front().kind() == ARRAY) {
		decls.pop_front();
		decls.push_front(Pointer());
	} else if (!decls.empty() && decls.front().kind() == FUNCTION)
		decls.push_front(Pointer());

	declareSymbol(name, Type(typespec, decls));
	params->push_back(Type(typespec, decls));
}


/*
 * Function:	parameters
 *
 * Description: Parse the parameters of a function, but not the opening or
 *				closing parentheses.
 *
 *				parameters:
 *				  void
 *				  parameter-list
 *
 *				parameter-list:
 *				  parameter
 *				  parameter , parameter-list
 */

static Types *parameters()
{
	Types *params = new Types();

	if (lookahead == VOID)
		match(VOID);

	else {
		parameter(params);

		while (lookahead == ',') {
			match(',');
			parameter(params);
		}
	}

	return params;
}


/*
 * Function:	primaryExpression
 *
 * Description: Parse a primary expression.
 *
 *				primary-expression:
 *				  ( expression )
 *				  identifier
 *				  character
 *				  string
 *				  num
 */

static Type primaryExpression(bool &lvalue)
{
	if (lookahead == '(') {
		match('(');
		Type left = expression(lvalue);
		match(')');
		// lvalue = whether expression is an lvalue
		return left;

	} else if (lookahead == CHARACTER) {
		match(CHARACTER);
		lvalue = false;
		return integer; // promotion

	} else if (lookahead == STRING) {
		match(STRING);
		Declarators decls;
		decls.push_front(Array(NEIGHBORS));
		lvalue = true;
		return Type(CHAR, decls);

	} else if (lookahead == NUM) {
		match(NUM);
		lvalue = false;
		return integer;

	} else if (lookahead == ID) {
		Symbol *s = checkIdentifier(lexbuf);
		match(ID);
		lvalue = true;
		return s->type();

	} else
		error();
		
	return Type(ERROR);
}


/*
 * Function:	postfixExpression
 *
 * Description: Parse a postfix expression.
 *
 *				postfix-expression:
 *				  primary-expression
 *				  postfix-expression [ expression ]
 *				  postfix-expression ( expression-list )
 *				  postfix-expression ( )
 *
 *				expression-list:
 *				  expression
 *				  expression , expression-list
 */

static Type postfixExpression(bool &lvalue)
{
	Type left = primaryExpression(lvalue);
	Types args;
	while (1) {
		if (lookahead == '[') {
			match('[');
			Type right = expression(lvalue);
			match(']');
			cout << "index" << endl;
			left = checkArray(left, right, "[]"); // ?
			lvalue = true;

		} else if (lookahead == '(') {
			match('(');

			if (lookahead != ')') {
				args.push_back(expression(lvalue).promote());

				while (lookahead == ',') {
					match(',');
					args.push_back(expression(lvalue).promote());
				}
			}

			match(')');
			cout << "call" << endl;
			left = checkFunc(left, args);
			lvalue = false;

		} else
			break;
	}
	return left;
}


/*
 * Function:	prefixExpression
 *
 * Description: Parse a prefix expression.
 *
 *				prefix-expression:
 *				  postfix-expression
 *				  ! prefix-expression
 *				  - prefix-expression
 *				  * prefix-expression
 *				  & prefix-expression
 *				  sizeof prefix-expression
 *				  ( specifier abstract-declarator ) prefix-expression
 */

static Type prefixExpression(bool &lvalue)
{
	Declarators decls;
	string name;

	if (lookahead == '!') {
		match('!');
		Type left = prefixExpression(lvalue);
		cout << "check !" << endl;
		left = checkLogicalNot(left);
		lvalue = false;
		return left;

	} else if (lookahead == '-') {
		match('-');
		Type left = prefixExpression(lvalue);
		cout << "check -(neg)" << endl;
		left = checkNegate(left,"-");
		lvalue = false;
		return left;

	} else if (lookahead == '*') {
		match('*');
		Type left = prefixExpression(lvalue);
		cout << "check *(deref)" << endl;
		left = checkDeref(left,"*");
		lvalue = true;
		return left;

	} else if (lookahead == '&') {
		match('&');
		Type left = prefixExpression(lvalue);
		cout << "check &(addr)" << endl;
		left = checkAddr(left, lvalue);
		lvalue = false;
		return left;

	} else if (lookahead == SIZEOF) {
		match(SIZEOF);
		Type left = prefixExpression(lvalue);
		cout << "check sizeof" << endl;
		left = checkSizeOf(left, "sizeof");
		lvalue = false;
		return left;

	} else if (lookahead == '(' && isSpecifier(peek())) {
		match('(');
		int typespec = specifier();
		decl(decls, name, ABSTRACT_DECL);
		match(')');
		Type left = prefixExpression(lvalue);
		Type castAs(typespec, decls);
		cout << "check cast" << endl;
		left = checkCast(castAs);
		lvalue = false;
		return left;

	} else
		return postfixExpression(lvalue);
}


/*
 * Function:	multiplicativeExpression
 *
 * Description: Parse a multiplicative expression.
 *
 *				multiplicative-expression:
 *				  prefix-expression
 *				  multiplicative-expression * prefix-expression
 *				  multiplicative-expression / prefix-expression
 *				  multiplicative-expression % prefix-expression
 */

static Type multiplicativeExpression(bool &lvalue)
{
	Type left = prefixExpression(lvalue);

	while (1) {
		if (lookahead == '*') {
			match('*');
			Type right = prefixExpression(lvalue);
			cout << "check *" << endl;
			left = checkMultiplicativeExpression(left, right, "*");
			lvalue = false;

		} else if (lookahead == '/') {
			match('/');
			Type right = prefixExpression(lvalue);
			cout << "check /" << endl;
			left = checkMultiplicativeExpression(left, right, "/");
			lvalue = false;

		} else if (lookahead == '%') {
			match('%');
			Type right = prefixExpression(lvalue);
			cout << "check %" << endl;
			left = checkMultiplicativeExpression(left, right, "%");
			lvalue = false;

		} else
			break;
	}

	return left;
}


/*
 * Function:	additiveExpression
 *
 * Description: Parse an additive expression.
 *
 *				additive-expression:
 *				  multiplicative-expression
 *				  additive-expression + multiplicative-expression
 *				  additive-expression - multiplicative-expression
 */

static Type additiveExpression(bool &lvalue)
{
	Type left = multiplicativeExpression(lvalue);

	while (1) {
		if (lookahead == '+') {
			match('+');
			Type right = multiplicativeExpression(lvalue);
			cout << "check +" << endl;
			left = checkAdd(left, right, "+");
			lvalue = false;

		} else if (lookahead == '-') {
			match('-');
			Type right = multiplicativeExpression(lvalue);
			cout << "check -" << endl;
			left = checkSub(left, right, "-");
			lvalue = false;

		} else
			break;
	}

	return left;
}


/*
 * Function:	relationalExpression
 *
 * Description: Parse a relational expression.	Note that Simple C does not
 *				have shift operators, so we go immediately to additive
 *				expressions.
 *
 *				relational-expression:
 *				  additive-expression
 *				  relational-expression < additive-expression
 *				  relational-expression > additive-expression
 *				  relational-expression <= additive-expression
 *				  relational-expression >= additive-expression
 */

static Type relationalExpression(bool &lvalue)
{
	Type left = additiveExpression(lvalue);

	while (1) {
		if (lookahead == '<') {
			match('<');
			Type right = additiveExpression(lvalue);
			cout << "check <" << endl;
			left = checkEqualityandRelational(left, right, "<");
			lvalue = false;

		} else if (lookahead == '>') {
			match('>');
			Type right = additiveExpression(lvalue);
			cout << "check >" << endl;
			left = checkEqualityandRelational(left, right, ">");
			lvalue = false;

		} else if (lookahead == LEQ) {
			match(LEQ);
			Type right = additiveExpression(lvalue);
			cout << "check <=" << endl;
			left = checkEqualityandRelational(left, right, "<=");
			lvalue = false;

		} else if (lookahead == GEQ) {
			match(GEQ);
			Type right = additiveExpression(lvalue);
			cout << "check >=" << endl;
			left = checkEqualityandRelational(left, right, ">=");
			lvalue = false;

		} else
			break;
	}
	return left;
}


/*
 * Function:	equalityExpression
 *
 * Description: Parse an equality expression.
 *
 *				equality-expression:
 *				  relational-expression
 *				  equality-expression == relational-expression
 *				  equality-expression != relational-expression
 */

static Type equalityExpression(bool &lvalue)
{
	Type left = relationalExpression(lvalue);

	while (1) {
		if (lookahead == EQL) {
			match(EQL);
			Type right = relationalExpression(lvalue);
			cout << "check ==" << endl;
			left = checkEqualityandRelational(left, right, "==");
			lvalue = false;

		} else if (lookahead == NEQ) {
			match(NEQ);
			Type right = relationalExpression(lvalue);
			cout << "check !=" << endl;
			left = checkEqualityandRelational(left, right, "!=");
			lvalue = false;

		} else
			break;
	}

	return left;
}


/*
 * Function:	logicalAndExpression
 *
 * Description: Parse a logical-and expression.	 Note that Simple C does
 *				not have bitwise-and expressions.
 *
 *				logical-and-expression:
 *				  equality-expression
 *				  logical-and-expression && equality-expression
 */

static Type logicalAndExpression(bool &lvalue)
{
	Type left = equalityExpression(lvalue);
	while (lookahead == AND) {
		match(AND);
		Type right = equalityExpression(lvalue);
		cout << "check &&" << endl;
		left = checkLogicalExpression(left, right);
		lvalue = false;
	}
	return left;
}


/*
 * Function:	expression
 *
 * Description: Parse an expression, or more specifically, a logical-or
 *				expression, since Simple C does not allow comma or
 *				assignment as an expression operator.
 *
 *				expression:
 *				  logical-and-expression
 *				  expression || logical-and-expression
 */

Type expression(bool &lvalue) {
	Type left = logicalAndExpression(lvalue);
	while (lookahead == OR) {
		match(OR);
		Type right = logicalAndExpression(lvalue);
		cout << "check ||" << endl;
		left = checkLogicalExpression(left, right);
		lvalue = false;
	}
	return left;
}


/*
 * Function:	statements
 *
 * Description: Parse a possibly empty sequence of statements.	Rather than
 *				checking if the next token starts a statement, we check if
 *				the next token ends the sequence, since a sequence of
 *				statements is always terminated by a closing brace.
 *
 *				statements:
 *				  empty
 *				  statement statements
 */

static void statements()
{
	while (lookahead != '}')
		statement();
}


/*
 * Function:	assignment
 *
 * Description: Parse an assignment statement.
 *
 *				assignment:
 *				  expression = expression
 *				  expression
 */

static Type assignment(bool &lvalue)
{
	Type left = expression(lvalue);

	if (lookahead == '=') {
		match('=');
		bool leftLvalue = lvalue;
		Type right = expression(lvalue);
		checkAssignment(left, right, leftLvalue);
		lvalue = true;
	}
	return left;

}


/*
 * Function:	statement
 *
 * Description: Parse a statement.	Note that Simple C has so few
 *				statements that we handle them all in this one function.
 *
 *				statement:
 *				  { declarations statements }
 *				  break ;
 *				  return expression ;
 *				  while ( expression ) statement
 *				  for ( assignment ; expression ; assignment ) statement
 *				  if ( expression ) statement
 *				  if ( expression ) statement else statement
 *				  assignment ;
 */

static void statement()
{
	bool lvalue;

	if (lookahead == '{') {
		match('{');
		openScope();
		declarations();
		statements();
		closeScope();
		match('}');

	} else if (lookahead == BREAK) {
		match(BREAK);
		checkBreak(breakCounter);
		match(';');

	} else if (lookahead == RETURN) {
		match(RETURN);
		Type right = expression(lvalue);
		right = checkReturn(returnType, right);
		match(';');

	} else if (lookahead == WHILE) {
		match(WHILE);
		match('(');
		expression(lvalue);
		match(')');
		breakCounter++;
		statement();
		breakCounter--;

	} else if (lookahead == FOR) {
		match(FOR);
		match('(');
		assignment(lvalue);
		match(';');
		expression(lvalue);
		match(';');
		assignment(lvalue);
		match(')');
		breakCounter++;
		statement();
		breakCounter--;

	} else if (lookahead == IF) {
		match(IF);
		match('(');
		expression(lvalue);
		match(')');
		statement();

		if (lookahead == ELSE) {
			match(ELSE);
			statement();
		}

	} else {
		assignment(lvalue);
		match(';');
	}
}


/*
 * Function:	functionOrGlobal
 *
 * Description: Parse a function definition or global declaration.	In the
 *				first declarator, parameters are allowed and if they are
 *				seen then we have a function definition.
 *
 *				function-or-global:
 *				  specifier declarator { declarations statements }
 *				  specifier declarator ;
 *				  specifier declarator , declarator-list ;
 */

static void functionOrGlobal()
{
	int typespec;
	Declarators decls;
	string name;

	typespec = specifier();

	if (decl(decls, name, FUNCTION_DECL)) {
		defineFunction(name, Type(typespec, decls));
		decls.pop_front();
		returnType = Type(typespec, decls);

		match('{');
		declarations();
		statements();
		closeScope();
		match('}');

	} else {
		declareSymbol(name, Type(typespec, decls));

		while (lookahead == ',') {
			match(',');
			decl(decls, name);
			declareSymbol(name, Type(typespec, decls));
		}

		match(';');
	}
}


/*
 * Function:	main
 *
 * Description: Analyze the standard input stream.
 */

int main()
{
	lookahead = lexan(lexbuf);
	openScope();

	while (lookahead != DONE)
		functionOrGlobal();

	closeScope();
	exit(EXIT_SUCCESS);
}
