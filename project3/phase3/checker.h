# ifndef CHECKER_h
# define CHECKER_h
# include "type.h"

void openScope();
void closeScope();
void defineFunction(const std::string &name, const Type &type);
void declareSymbol(const std::string &name, const Type &type);
void checkIdentifier(const std::string &name);

# endif