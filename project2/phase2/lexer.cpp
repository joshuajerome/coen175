/*
 * File:	lexer.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the lexical analyzer for Simple C.
 *
 *		Extra functionality:
 *		- checking for out of range integer literals
 *		- checking for invalid string constants
 *		- checking for invalid character constants
 */

# include <set>
# include <cstdio>
# include <cerrno>
# include <cctype>
# include <cstdlib>
# include <iostream>
# include <map>
# include "string.h"
# include "lexer.h"
# include "tokens.h"

using namespace std;
int numerrors, lineno = 1;


/* Later, we will associate token values with each keyword */

static map<string,int> keywords = {
    {"auto",AUTO},
    {"break",BREAK},
    {"case",CASE},
    {"char",CHAR},
    {"const",CONST},
    {"continue",CONTINUE},
    {"default",DEFAULT},
    {"do",DO},
    {"double",DOUBLE},
    {"else",ELSE},
    {"enum",ENUM},
    {"extern",EXTERN},
    {"float",FLOAT},
    {"for",FOR},
    {"goto",GOTO},
    {"if",IF},
    {"int",INT},
    {"long",LONG},
    {"register",REGISTER},
    {"return",RETURN},
    {"short",SHORT},
    {"signed",SIGNED},
    {"sizeof",SIZEOF},
    {"static",STATIC},
    {"struct",STRUCT},
    {"switch",SWITCH},
    {"typedef",TYPEDEF},
    {"union",UNION},
    {"unsigned",UNSIGNED},
    {"void",VOID},
    {"volatile",VOLATILE},
    {"while",WHILE}
};


/*
 * Function:	report
 *
 * Description:	Report an error to the standard error prefixed with the
 *		line number.  We'll be using this a lot later with an
 *		optional string argument, but C++'s stupid streams don't do
 *		positional arguments, so we actually resort to snprintf.
 *		You just can't beat C for doing things down and dirty.
 */

void report(const string &str, const string &arg)
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << "line " << lineno << ": " << buf << endl;
    numerrors ++;
}

/*
 * Function:	lexan
 *
 * Description:	Read and tokenize the standard input stream.  The lexeme is
 *		stored in a buffer.
 */

int lexan(string &lexbuf)
{
    static int c = cin.get();
    bool invalid, overflow;
    long val;
    int p;


    /* The invariant here is that the next character has already been read
       and is ready to be classified.  In this way, we eliminate having to
       push back characters onto the stream, merely to read them again. */

    while (!cin.eof()) {
	lexbuf.clear();


	/* Ignore white space */

	while (isspace(c)) {
	    if (c == '\n')
		lineno ++;

	    c = cin.get();
	}


	/* Check for an identifier or a keyword */

	if (isalpha(c) || c == '_') {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isalnum(c) || c == '_');

	    if (keywords.count(lexbuf) > 0)
			return keywords[lexbuf]; 
	    else
			return ID;

	/* Check for a number */

	} else if (isdigit(c)) {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isdigit(c));

	    errno = 0;
	    val = strtol(lexbuf.c_str(), NULL, 0);

	    if (errno != 0 || val != (int) val)
		report("integer constant too large");
	    return NUM;


	/* There must be an easier way to do this.  It might seem stupid at
	   this point to recognize each token separately, but eventually
	   we'll be returning separate token values to the parser, so we
	   might as well do it now. */

	} else {
	    lexbuf += c;

	    switch(c) {


	    /* Check for '|' and '||' */

	    case '|':
		c = cin.get();

		if (c == '|') {
		    lexbuf += c;
		    c = cin.get();
		}
		return OR;


	    /* Check for '=' and '==' */

	    case '=':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return EQL;
		}

		return ASSIGN;


	    /* Check for '&' and '&&' */

	    case '&':
		c = cin.get();

		if (c == '&') {
		    lexbuf += c;
		    c = cin.get();
			return AND;
		}

		return ADDR;


	    /* Check for '!' and '!=' */

	    case '!':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return NEQ;
		}

		return NOT;


	    /* Check for '<' and '<=' */

	    case '<':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return LEQ;
		}

		return LTN;


	    /* Check for '>' and '>=' */

	    case '>':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return GEQ;
		}

		return GTN;


	    /* Check for '-', '--', and '->' */

	    case '-':
		c = cin.get();

		if (c == '-') {
		    lexbuf += c;
		    c = cin.get();
			return DEC;
		} else if (c == '>') {
		    lexbuf += c;
		    c = cin.get();
			return ARROW;
		}

		return MINUS;


	    /* Check for '+' and '++' */

	    case '+':
		c = cin.get();

		if (c == '+') {
		    lexbuf += c;
		    c = cin.get();
			return INC;
		}

		return PLUS;


	    /* Check for simple, single character tokens */

	    case '*':
			c = cin.get();
			return STAR; 
		case '%': 
			c = cin.get();
			return REM; 
		case ':':
			c = cin.get();
			return COLON; 
		case ';':
			c = cin.get();
			return SEMI; 
	    case '(': 
			c = cin.get();
			return LPAREN; 
		case ')': 
			c = cin.get();
			return RPAREN; 
		case '[': 
			c = cin.get();
			return LBRACK; 
		case ']':
			c = cin.get();
			return RBRACK; 
	    case '{': 
			c = cin.get();
			return LBRACE; 
		case '}': 
			c = cin.get();
			return RBRACE; 
		case '.': 
			c = cin.get();
			return DOT; 
		case ',':
			c = cin.get();
			return COMMA; 

	    /* Check for '/' or a comment */

	    case '/':
		c = cin.get();

		if (c == '*') {
		    do {
			while (c != '*' && !cin.eof()) {
			    if (c == '\n')
				lineno ++;

			    c = cin.get();
			}

			c = cin.get();
		    } while (c != '/' && !cin.eof());

		    c = cin.get();
		    break;

		} else {
		    return DIV;
		}


	    /* Check for a string literal */

	    case '"':
		do {
		    p = c;
		    c = cin.get();
		    lexbuf += c;

		    if (c == '\n')
			lineno ++;

		} while (p == '\\' || (c != '"' && c != '\n' && !cin.eof()));

		if (c == '\n' || cin.eof())
		    report("premature end of string constant");
		else {
		    string s = lexbuf.substr(1, lexbuf.size() - 2);
		    s = parseString(s, invalid, overflow);

		    if (invalid)
			report("unknown escape sequence");
		    else if (overflow)
			report("escape sequence out of range");
		}

		c = cin.get();
		return STRING;


	    /* Check for a character literal */

	    case '\'':
		do {
		    p = c;
		    c = cin.get();
		    lexbuf += c;

		    if (c == '\n')
			lineno ++;

		} while (p == '\\' || (c != '\'' && c != '\n' && !cin.eof()));

		if (c == '\n' || cin.eof())
		    report("premature end of character constant");
		else {
		    string s = lexbuf.substr(1, lexbuf.size() - 2);
		    s = parseString(s, invalid, overflow);

		    if (invalid)
			report("unknown escape sequence");
		    else if (overflow)
			report("escape sequence out of range");
		    else if (s.size() == 0)
			report("empty character constant");
		    else if (s.size() != 1)
			report("multi-character character constant");
		}

		c = cin.get();
		return CHARACTER;


	    /* Handle EOF here as well */

	    case EOF:
		return DONE;


	    /* Everything else is illegal */

	    default:
		c = cin.get();
		return ERROR;
	    }
	}
    }

    return DONE;
}


/*
 * Function:	main
 *
 * Description:	Read and tokenize and standard input stream.
 */

