/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "generator.h"
# include "checker.h"
# include "string.h"
# include "tokens.h"
# include "lexer.h"

using namespace std;

static int lookahead, nexttoken;
static string lexbuf, nextbuf;

static unsigned loopDepth;
static Type returnType(INT);

static Expression *expression();
static Types *parameters();
static Statement *statement();

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
 * Function:	number
 *
 * Description:	Match the next token as a number and return its value.
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
    else
	error();

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

static bool declarator(Declarators &decls, string &name, int kind = PLAIN_DECL)
{
    Declarators subdecls;
    bool hasparams = false;


    if (lookahead == '*') {
	match('*');
	hasparams = declarator(decls, name, kind);
	decls.push_front(Pointer());

    } else {
	if (lookahead == '(' && peek() != ')') {
	    match('(');
	    hasparams = declarator(subdecls, name, kind);
	    decls.splice_after(decls.before_begin(), subdecls);
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
 * Description:	Convenience function for calling declarator and reversing
 *		the declarators so we can use them.
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
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier
 *		  character
 *		  string
 *		  num
 */

static Expression *primaryExpression()
{
    Expression *expr;


    if (lookahead == '(') {
	match('(');
	expr = expression();
	match(')');

    } else if (lookahead == CHARACTER) {
	lexbuf = lexbuf.substr(1, lexbuf.size() - 2);
	expr = new Number(parseString(lexbuf)[0]);
	match(CHARACTER);

    } else if (lookahead == STRING) {
	lexbuf = lexbuf.substr(1, lexbuf.size() - 2);
	expr = new String(parseString(lexbuf));
	match(STRING);

    } else if (lookahead == NUM) {
	expr = new Number(number());

    } else if (lookahead == ID) {
	expr = new Identifier(checkIdentifier(lexbuf));
	match(ID);

    } else {
	expr = nullptr;
	error();
    }

    return expr;
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

static Expression *postfixExpression()
{
    Expression *left, *right;


    left = primaryExpression();

    while (1) {
	if (lookahead == '[') {
	    match('[');
	    right = expression();
	    left = checkArray(left, right);
	    match(']');

	} else if (lookahead == '(') {
	    Expressions args;

	    match('(');

	    if (lookahead != ')') {
		args.push_back(expression());

		while (lookahead == ',') {
		    match(',');
		    args.push_back(expression());
		}
	    }

	    left = checkCall(left, args);
	    match(')');

	} else
	    break;
    }

    return left;
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

static Expression *prefixExpression()
{
    int typespec;
    Expression *expr;
    Declarators decls;
    string name;


    if (lookahead == '!') {
	match('!');
	expr = prefixExpression();
	expr = checkNot(expr);

    } else if (lookahead == '-') {
	match('-');
	expr = prefixExpression();
	expr = checkNegate(expr);

    } else if (lookahead == '*') {
	match('*');
	expr = prefixExpression();
	expr = checkDereference(expr);

    } else if (lookahead == '&') {
	match('&');
	expr = prefixExpression();
	expr = checkAddress(expr);

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);
	expr = prefixExpression();
	expr = checkSizeof(expr);

    } else if (lookahead == '(' && isSpecifier(peek())) {
	match('(');
	typespec = specifier();
	decl(decls, name, ABSTRACT_DECL);
	match(')');
	expr = prefixExpression();
	expr = checkCast(Type(typespec, decls), expr);

    } else
	expr = postfixExpression();

    return expr;
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

static Expression *multiplicativeExpression()
{
    Expression *left, *right;


    left = prefixExpression();

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    right = prefixExpression();
	    left = checkMultiply(left, right);

	} else if (lookahead == '/') {
	    match('/');
	    right = prefixExpression();
	    left = checkDivide(left, right);

	} else if (lookahead == '%') {
	    match('%');
	    right = prefixExpression();
	    left = checkRemainder(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *additiveExpression()
{
    Expression *left, *right;


    left = multiplicativeExpression();

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    right = multiplicativeExpression();
	    left = checkAdd(left, right);

	} else if (lookahead == '-') {
	    match('-');
	    right = multiplicativeExpression();
	    left = checkSubtract(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *relationalExpression()
{
    Expression *left, *right;


    left = additiveExpression();

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    right = additiveExpression();
	    left = checkLessThan(left, right);

	} else if (lookahead == '>') {
	    match('>');
	    right = additiveExpression();
	    left = checkGreaterThan(left, right);

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    right = additiveExpression();
	    left = checkLessOrEqual(left, right);

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    right = additiveExpression();
	    left = checkGreaterOrEqual(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *equalityExpression()
{
    Expression *left, *right;


    left = relationalExpression();

    while (1) {
	if (lookahead == EQL) {
	    match(EQL);
	    right = relationalExpression();
	    left = checkEqual(left, right);

	} else if (lookahead == NEQ) {
	    match(NEQ);
	    right = relationalExpression();
	    left = checkNotEqual(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *logicalAndExpression()
{
    Expression *left, *right;


    left = equalityExpression();

    while (lookahead == AND) {
	match(AND);
	right = equalityExpression();
	left = checkLogicalAnd(left, right);
    }

    return left;
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

static Expression *expression()
{
    Expression *left, *right;


    left = logicalAndExpression();

    while (lookahead == OR) {
	match(OR);
	right = logicalAndExpression();
	left = checkLogicalOr(left, right);
    }

    return left;
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

static Statements statements()
{
    Statements stmts;


    while (lookahead != '}')
	stmts.push_back(statement());

    return stmts;
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

static Statement *assignment()
{
    Expression *expr;


    expr = expression();

    if (lookahead == '=') {
	match('=');
	return checkAssignment(expr, expression());
    }

    return new Simple(expr);
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

static Statement *statement()
{
    Scope *scope;
    Expression *expr;
    Statement *stmt, *init, *incr;
    Statements stmts;


    if (lookahead == '{') {
	match('{');
	openScope();
	declarations();
	stmts = statements();
	scope = closeScope();
	match('}');
	return new Block(scope, stmts);

    } else if (lookahead == BREAK) {
	match(BREAK);
	stmt = checkBreak(loopDepth);
	match(';');
	return stmt;

    } else if (lookahead == RETURN) {
	match(RETURN);
	expr = expression();
	stmt = checkReturn(expr, returnType);
	match(';');
	return stmt;

    } else if (lookahead == WHILE) {
	match(WHILE);
	match('(');
	expr = expression();
	checkTest(expr);
	match(')');
	loopDepth ++;
	stmt = statement();
	loopDepth --;
	return new While(expr, stmt);

    } else if (lookahead == FOR) {
	match(FOR);
	match('(');
	init = assignment();
	match(';');
	expr = expression();
	checkTest(expr);
	match(';');
	incr = assignment();
	match(')');
	loopDepth ++;
	stmt = statement();
	loopDepth --;
	return new For(init, expr, incr, stmt);

    } else if (lookahead == IF) {
	match(IF);
	match('(');
	expr = expression();
	checkTest(expr);
	match(')');
	stmt = statement();

	if (lookahead != ELSE)
	    return new If(expr, stmt, nullptr);

	match(ELSE);
	return new If(expr, stmt, statement());

    } else {
	stmt = assignment();
	match(';');
	return stmt;
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
    int typespec;
    Declarators decls;
    Statements stmts;
    Procedure *proc;
    Symbol *symbol;
    Scope *scope;
    string name;


    typespec = specifier();

    if (decl(decls, name, FUNCTION_DECL)) {
	symbol = defineFunction(name, Type(typespec, decls));
	decls.pop_front();
	returnType = Type(typespec, decls);

	match('{');
	declarations();
	stmts = statements();
	scope = closeScope();
	proc = new Procedure(symbol, new Block(scope, stmts));
	match('}');

	if (numerrors == 0)
	    proc->generate();

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
 * Description:	Analyze the standard input stream.
 */

int main()
{
    lookahead = lexan(lexbuf);
    openScope();

    while (lookahead != DONE)
	functionOrGlobal();

    generateGlobals(closeScope());
    exit(EXIT_SUCCESS);
}
