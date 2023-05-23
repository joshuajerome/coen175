# include "generator.h"
# include <iostream>

using namespace std;

static ostream &operator<<(ostream &ostr, Expression *expr);

void Block::generate()
{
    for (auto stmt : _stmts)
    {
        stmt->generate();
    }
}

void Simple::generate()
{
    _expr->generate();
}

static ostream &operator<<(ostream &ostr, Expression *expr)
{
    expr->operand(ostr);
    return ostr;
}

void Expression::operand(ostream &ostr) const
{
    
}

void Number::operand(ostream &ostr) const
{
    ostr << "$" << value();
}

void Identifier::operand(ostream &ostr) const
{
    if (_symbol->_offset == 0)
    {
        ostr << _symbol->name();
    }
    else
    {
        ostr << _symbol->_offset << "(%ebp)" << endl;
    }
}

void generateGlobals(Scope *scope)
{
    for (auto it : scope->symbols())
    {
        if (it->type().isFunction())
        {
            cout << "\t.comm\t" << it->name() << ", " << it->type().size() << endl;
        }
    }
}

void Procedure::generate() {
    
    int offsetCounter = 0;
    for (int i = 0; i < int(_body->declarations()->symbols().size()); i++)
    {
        Symbol *symbol = _body->declarations()->symbols()[i];
        if (i >= int(_id->type().parameters()->size()))
        {
            offsetCounter -= symbol->_offset;
            symbol->_offset = offsetCounter;
        }
        else
        {
            symbol->_offset = 8 + 4 * i;
        }
    }
    cout << _id->name() << ":";
    
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << -offsetCounter << ",%esp" << endl;

    _body->generate();

    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret\t" << endl;

    cout << ".globl " << _id->name() << endl;
}

void Assignment::generate() {
    cout << "\tmovl\t" << _right << ", " << _left << endl;
}

void Call::generate() {
    for (int i = int(_args.size() - 1); i >= 0; i--)
    {
        cout << "\tpushl\t" << _args.at(i) << endl;
    }
    cout << "\tcall\t" << _expr << endl;
}  


