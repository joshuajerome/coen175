# include <iostream>
# include "tokens.h"
# include "lexer.h"
# include <bits/stdc++.h>

/* Function Prototypes */
void match(int t);
int peek();

void transaltion_unit();
void function_or_global(); // function-definition

void declarations();
void declaration();
bool declarator(bool paramsOkay); // function-declarator and declarator

bool is_specifier(int token);
void specifier();

void parameters();
void parameter();

void statements();
void statement();
void assignment();

/* E1 --> E9 */
void or_expression();
void and_expression();
void equality();
void comparison();
void add_sub();
void mul_div_rem();
void prefix_expression();
void postfix_expression();
void base_expression();

void expression_list();
void abstract_declarator();

using namespace std;

string lexbuf, nextbuf;
int lookahead = lexan(lexbuf), nexttoken = lexan(lexbuf);

void match(int t) {
	if (lookahead != t)
		report("match error");

	if (nexttoken) {
		lookahead = nexttoken;
		lexbuf = nextbuf;
		nexttoken = 0;
	} else
		lookahead = lexan(lexbuf);
}

int peek() {
	if (!nexttoken)
		nexttoken = lexan(lexbuf);
	
	return nexttoken;
}

void translation_unit() {
	while (lookahead != DONE) {
		function_or_global();
	}
}

void function_or_global() {
	specifier();
	if (declarator(true)) {
		match(LBRACE);
		declarations();
		statements();
		match(RBRACE);
	} else {
		while (lookahead == COMMA) {
			match(COMMA);
			declarator(false);
		}
		match(SEMI);
	}
}

void declarations() {
	while (is_specifier(lookahead)) {
		declaration();
	}
}

void declaration() {
	specifier();
	declarator(false);
	while (lookahead == COMMA) {
		match(COMMA);
		declarator(false);
	}
	match(SEMI);
}

bool declarator(bool paramsOkay = false) {
	bool paramsSeen = false;

	if (lookahead == STAR) {
		match(STAR);
		paramsSeen = declarator(paramsOkay);

	} else {

		if (lookahead == ID) {
			match(ID);

			if (paramsOkay && lookahead == LPAREN && peek() != RPAREN) {

				paramsSeen = true;

				match(LPAREN);
				parameters();
				match(RPAREN);
			}

		} else if (lookahead == LPAREN) {
			match(LPAREN);
			paramsSeen = declarator(paramsOkay);
			match(RPAREN);
		}

		while (1) {
			if (lookahead == LBRACK) {
				match(LBRACK);
				match(NUM);
				match(RBRACK);
			} else if (lookahead == LPAREN) {
				match(LPAREN);
				match(RPAREN);
			} else break;
		}
	}
	return paramsSeen;
}

bool is_specifier(int token) {
	return token == INT || token == CHAR;
}

void specifier() {
	is_specifier(lookahead)? match(lookahead) : report("match error"); 
}

void parameters() {
	if (lookahead == VOID) {
		match(VOID);
	} else {
		parameter();
		while (lookahead == COMMA) {
			match(COMMA);
			parameter();
		}
	}
}

void parameter() {
	if (is_specifier(lookahead)) {
		specifier();	
	}
	declarator(false);
}

void statements() {
	while (lookahead != RBRACE) {
		statement();
	}
}

void statement() {

	if (lookahead == LBRACE) {
		match(LBRACE);
		declarations();
		statements();
		match(RBRACE);
	} else if (lookahead == BREAK) {
		match(BREAK);
		match(SEMI);
	} else if (lookahead == RETURN) {
		match(RETURN);
		or_expression();
		match(SEMI);
	} else {
		if (lookahead == WHILE) {
			match(WHILE);
			match(LPAREN);
			or_expression();
			match(RPAREN);
			statement();
		} else if (lookahead == FOR) {
			match(FOR);
			match(LPAREN);
			assignment();
			match(SEMI);
			or_expression();
			match(SEMI);
			assignment();
			match(RPAREN);
			statement();
		} else if (lookahead == IF) {
			match(IF);
			match(LPAREN);
			or_expression();
			match(RPAREN);
			statement();
			if (lookahead == ELSE) {
				match(ELSE);
				statement();
			}
		} else {
			assignment();
			match(SEMI);
		}
	}

}

void assignment() {
	or_expression();
	if (lookahead == ASSIGN) {
		match(ASSIGN);
		or_expression();
	}
}

void or_expression() {
	and_expression();
	while (lookahead == OR) {
		match(OR);
		and_expression();
		cout << "or" << endl;
	}
}

void and_expression() {
	equality();
	while (lookahead == AND) {
		match(AND);
		equality();
		cout << "and" << endl;
	}
}

void equality() {
	comparison();
	while (lookahead == EQL || lookahead == NEQ) {
		int tmp = lookahead;
		match(lookahead);
		comparison();
		switch (tmp) {
			case EQL:
				cout << "eql" << endl;
				break;
			case NEQ:
				cout << "neq" << endl;
				break;
		}
	}
}

void comparison() {
	add_sub();
	while (lookahead == LTN || lookahead == GTN || lookahead == LEQ || lookahead == GEQ) {
		int tmp = lookahead;
		match(lookahead);
		add_sub();
		switch (tmp) {
			case LTN:
				cout << "ltn" << endl;
				break;
			case GTN:
				cout << "gtn" << endl;
				break;
			case LEQ:
				cout << "leq" << endl;
				break;
			case GEQ:
				cout << "geq" << endl;
				break;
		}
	}
}

void add_sub() {
	mul_div_rem();
	while (lookahead == PLUS || lookahead == MINUS) {
		int tmp = lookahead;
		match(lookahead);
		mul_div_rem();
		switch (tmp) {
			case PLUS:
				cout << "add" << endl;
				break;
			case MINUS:
				cout << "sub" << endl;
				break;
		}
	}
}

void mul_div_rem() {
	prefix_expression();
	while (lookahead == STAR || lookahead == DIV || lookahead == REM) {
		int tmp = lookahead;
		match(lookahead);
		prefix_expression();
		switch (tmp) {
			case STAR:
				cout << "mul" << endl;
				break;
			case DIV:
				cout << "div" << endl;
				break;
			case REM:
				cout << "rem" << endl;
				break;
		}
	}
}

void prefix_expression() {

	if (lookahead == LPAREN && is_specifier(peek())) {
		match(LPAREN);
		specifier();
		abstract_declarator(); 
		match(RPAREN);
		prefix_expression();
		cout << "cast" << endl;

	} else if (lookahead == STAR || lookahead == NOT || lookahead == MINUS || lookahead == SIZEOF || lookahead == ADDR) {
		int tmp = lookahead;
		match(lookahead);
		prefix_expression();
		switch (tmp) {
			case STAR:
				cout << "deref" << endl;
				break;
			case NOT:
				cout << "not" << endl;
				break;
			case MINUS:
				cout << "neg" << endl;
				break;
			case SIZEOF:
				cout << "sizeof" << endl;
				break;
			case ADDR:
				cout << "addr" << endl;
				break;
		}
	} else {
		postfix_expression();
	}
}

void postfix_expression() {
	base_expression();
	while (lookahead == LBRACK || lookahead == LPAREN) {
		if (lookahead == LBRACK) {
			match(LBRACK);
			or_expression();
			match(RBRACK);
			cout << "index" << endl;

		} else if (lookahead == LPAREN) {
			if (peek() == RPAREN) {
				match(LPAREN);
				match(RPAREN);
			} else {
				match(LPAREN);
				expression_list();
				match(RPAREN);
			}
			cout << "call" << endl;
		}
	}
}

void base_expression() {
	if (lookahead == ID) {
		match(ID);
	} else if (lookahead == NUM) {
		match(NUM);
	} else if (lookahead == STRING) {
		match(STRING);
	} else if (lookahead == CHARACTER) {
		match(CHARACTER);
	} else if (lookahead == LPAREN) {
		match(LPAREN);
		or_expression();
		match(RPAREN);
	}
}

void abstract_declarator() {
	if (lookahead == STAR) {
		match(STAR);
		abstract_declarator();
	} else {
		if (lookahead == LPAREN) {
			match(LPAREN);
			abstract_declarator();
			match(RPAREN);
		}
		while(1) {
			if (lookahead == LPAREN) {
				match(LPAREN);
				match(RPAREN);
			} else if (lookahead == LBRACK) {
				match(LBRACK);
				match(NUM);
				match(RBRACK);
			} else break;
		}
	}
}

void expression_list() {
	or_expression();
	while (lookahead == COMMA) {
		match(lookahead);
		or_expression();
	}
}

int main()
{
	translation_unit();
	cout << lineno - 1 << endl;
}
