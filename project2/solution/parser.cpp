/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "tokens.h"
# include "lexer.h"

using namespace std;

static int lookahead, nexttoken;
static string lexbuf, nextbuf;

static void expression();
static void parameters();
static void statement();

enum { PLAIN_DECL, FUNCTION_DECL, ABSTRACT_DECL };


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
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
 * Description:	Return the next token in the input stream and save it so
 *		that match() will later return it.
 */

static int peek()
{
    if (!nexttoken)
	nexttoken = lexan(nextbuf);

    return nexttoken;
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
    return token == INT || token == CHAR;
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.  Simple C has only int and char.
 *
 *		specifier:
 *		  int
 *		  char
 */

static int specifier()
{
	int typespec = lookahead;

    if (isSpecifier(lookahead))
		match(lookahead);
    else error();

	return typespec;
}


/*
 * Function:	declarator
 *
 * Description:	Parse all types of declarator: plain, function, and
 *		abstract.  The kind of declarator expected is indicated via
 *		the parameter.
 *
 *		declarator:
 *		  identifier
 *		  * declarator
 *		  declarator ( )
 *		  declarator [ num ]
 *		  ( declarator )
 *
 *		function-declarator:
 *		  identifier ( parameters )
 *		  * function-declarator
 *		  function-declarator ( )
 *		  function-declarator [ num ]
 *		  ( function-declarator )
 *
 *		abstract-declarator:
 *		  empty
 *		  * abstract-declarator
 *		  abstract-declarator ( )
 *		  abstract-declarator [ num ]
 *		  ( abstract-declarator )
 *
 *		Perhaps it is bad style to parse all declarators in one
 *		function, but the parsing logic is so similar, as the type
 *		construction logic will be, that one function reduces the
 *		development effort.  Also, the first such declarator at the
 *		global level must be treated specially in order to
 *		distinguish between a function definition and a function or
 *		variable declaration, so a function such as this one is
 *		needed anyway.
 */

static bool declarator(int kind = PLAIN_DECL)
{
    bool hasparams = false;


    if (lookahead == '*') {
	match('*');
	hasparams = declarator(kind);

    } else {
	if (lookahead == '(' && peek() != ')') {
	    match('(');
	    hasparams = declarator(kind);
	    match(')');

	} else if (kind != ABSTRACT_DECL) {
		string name = lexbuf;
	    match(ID);

	    if (kind == FUNCTION_DECL && lookahead == '(' && peek() != ')') {
		match('(');
		parameters();
		hasparams = true;
		match(')');
	    }
	}

	while (1) {
	    if (lookahead == '(') {
			match('(');
			match(')');

	    } else if (lookahead == '[') {
			match('[');
			match(NUM);
			match(']');

	    } else
		break;
	}
    }

    return hasparams;
}


/*
 * Function:	declaration
 *
 * Description:	Parse a variable or function declaration.
 *
 *		declaration:
 *		  specifier declarator-list ';'
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
	int typespec = specifier();
	string type = (typespec == INT ? "int" : "char");
    declarator();

    while (lookahead == ',') {
		match(',');
		declarator();
    }

    match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead))
	declaration();
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which is simply a specifier followed by
 *		a declarator.  Parameters in Simple C are only specified as
 *		part of a function definition so a plain declarator rather
 *		than an abstract declarator is used since the identifier is
 *		always required.
 *
 *		parameter:
 *		  specifier declarator
 */

static void parameter()
{
    specifier();
    declarator();
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  parameter-list
 *
 *		parameter-list:
 *		  parameter
 *		  parameter , parameter-list
 */

static void parameters()
{
    if (lookahead == VOID)
	match(VOID);

    else {
	parameter();

	while (lookahead == ',') {
	    match(',');
	    parameter();
	}
    }
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier
 *		  character
 *		  string
 *		  num
 */

static void primaryExpression()
{
    if (lookahead == '(') {
	match('(');
	expression();
	match(')');

    } else if (lookahead == CHARACTER)
	match(CHARACTER);

    else if (lookahead == STRING)
	match(STRING);

    else if (lookahead == NUM)
	match(NUM);

    else if (lookahead == ID)
	match(ID);

    else
	error();
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 *		  postfix-expression ( expression-list )
 *		  postfix-expression ( )
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */

static void postfixExpression()
{
    primaryExpression();

    while (1) {
	if (lookahead == '[') {
	    match('[');
	    expression();
	    match(']');
	    cout << "index" << endl;

	} else if (lookahead == '(') {
	    match('(');

	    if (lookahead != ')') {
		expression();

		while (lookahead == ',') {
		    match(',');
		    expression();
		}
	    }

	    match(')');
	    cout << "call" << endl;

	} else
	    break;
    }
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof prefix-expression
 *		  ( specifier abstract-declarator ) prefix-expression
 */

static void prefixExpression()
{
    if (lookahead == '!') {
	match('!');
	prefixExpression();
	cout << "not" << endl;

    } else if (lookahead == '-') {
	match('-');
	prefixExpression();
	cout << "neg" << endl;

    } else if (lookahead == '*') {
	match('*');
	prefixExpression();
	cout << "deref" << endl;

    } else if (lookahead == '&') {
	match('&');
	prefixExpression();
	cout << "addr" << endl;

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);
	prefixExpression();
	cout << "sizeof" << endl;

    } else if (lookahead == '(' && isSpecifier(peek())) {
	match('(');
	specifier();
	declarator(ABSTRACT_DECL);
	match(')');
	prefixExpression();
	cout << "cast" << endl;

    } else
	postfixExpression();
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

static void multiplicativeExpression()
{
    prefixExpression();

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    prefixExpression();
	    cout << "mul" << endl;

	} else if (lookahead == '/') {
	    match('/');
	    prefixExpression();
	    cout << "div" << endl;

	} else if (lookahead == '%') {
	    match('%');
	    prefixExpression();
	    cout << "rem" << endl;

	} else
	    break;
    }
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

static void additiveExpression()
{
    multiplicativeExpression();

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    multiplicativeExpression();
	    cout << "add" << endl;

	} else if (lookahead == '-') {
	    match('-');
	    multiplicativeExpression();
	    cout << "sub" << endl;

	} else
	    break;
    }
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static void relationalExpression()
{
    additiveExpression();

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    additiveExpression();
	    cout << "ltn" << endl;

	} else if (lookahead == '>') {
	    match('>');
	    additiveExpression();
	    cout << "gtn" << endl;

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    additiveExpression();
	    cout << "leq" << endl;

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    additiveExpression();
	    cout << "geq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

static void equalityExpression()
{
    relationalExpression();

    while (1) {
	if (lookahead == EQL) {
	    match(EQL);
	    relationalExpression();
	    cout << "eql" << endl;

	} else if (lookahead == NEQ) {
	    match(NEQ);
	    relationalExpression();
	    cout << "neq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

static void logicalAndExpression()
{
    equalityExpression();

    while (lookahead == AND) {
	match(AND);
	equalityExpression();
	cout << "and" << endl;
    }
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

static void expression()
{
    logicalAndExpression();

    while (lookahead == OR) {
	match(OR);
	logicalAndExpression();
	cout << "or" << endl;
    }
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */

static void statements()
{
    while (lookahead != '}')
	statement();
}


/*
 * Function:	assignment
 *
 * Description:	Parse an assignment statement.
 *
 *		assignment:
 *		  expression = expression
 *		  expression
 */

static void assignment()
{
    expression();

    if (lookahead == '=') {
	match('=');
	expression();
    }
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  break ;
 *		  return expression ;
 *		  while ( expression ) statement
 *		  for ( assignment ; expression ; assignment ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  assignment ;
 */

static void statement()
{
    if (lookahead == '{') {
	match('{');
	declarations();
	statements();
	match('}');

    } else if (lookahead == BREAK) {
	match(BREAK);
	match(';');

    } else if (lookahead == RETURN) {
	match(RETURN);
	expression();
	match(';');

    } else if (lookahead == WHILE) {
	match(WHILE);
	match('(');
	expression();
	match(')');
	statement();

    } else if (lookahead == FOR) {
	match(FOR);
	match('(');
	assignment();
	match(';');
	expression();
	match(';');
	assignment();
	match(')');
	statement();

    } else if (lookahead == IF) {
	match(IF);
	match('(');
	expression();
	match(')');
	statement();

	if (lookahead == ELSE) {
	    match(ELSE);
	    statement();
	}

    } else {
	assignment();
	match(';');
    }
}


/*
 * Function:	functionOrGlobal
 *
 * Description:	Parse a function definition or global declaration.  In the
 *		first declarator, parameters are allowed and if they are
 *		seen then we have a function definition.
 *
 * 		function-or-global:
 * 		  specifier declarator { declarations statements }
 * 		  specifier declarator ;
 * 		  specifier declarator , declarator-list ;
 */

static void functionOrGlobal()
{
    specifier();

    if (declarator(FUNCTION_DECL)) {
	match('{');
	declarations();
	statements();
	match('}');

    } else {
	while (lookahead == ',') {
	    match(',');
	    declarator();
	}

	match(';');
    }
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    lookahead = lexan(lexbuf);

    while (lookahead != DONE)
	functionOrGlobal();

    cout << lineno - 1 << endl;
    exit(EXIT_SUCCESS);
}
