/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <cassert>
# include <iostream>
# include "generator.h"
# include "machine.h"
# include "Label.h"
# include "Tree.h"

using namespace std;

static int offset;
static string funcname;
static ostream &operator <<(ostream &ostr, Expression *expr);

static Register *eax = new Register("%eax", "%al");
static Register *ecx = new Register("%ecx", "%cl");
static Register *edx = new Register("%edx", "%dl");

static vector<Register *> registers = {eax, ecx, edx};


/* These will be replaced with functions in the next phase.  They are here
   as placeholders so that Call::generate() is finished. */


void assign(Expression *expr, Register *reg)
{
    if (expr != nullptr)
    {
        if (expr->_register != nullptr)
            expr->_register->_node = nullptr;
        
        expr->_register = reg;
    }

    if (reg != nullptr)
    {
        if (reg->_node != nullptr)
            reg->_node->_register = nullptr;
        
        reg->_node = expr;
    }
}

void load(Expression *expr, Register *reg)
{
    if (reg->_node != expr)
    {
        if (reg->_node != nullptr)
        {
            unsigned n = reg->_node->type().size();
            offset -=n;
            reg->_node->_offset = offset;
            cout << (n == 1 ? "\tmovb\t" : "\tmovl\t");
            cout << reg << ", " << offset << "(%ebp)" << endl;
        }

        if (expr != nullptr)
        {
            unsigned n = expr->type().size();
            cout << (n == 1 ? "\tmovb\t" : "\tmovl\t");
            cout << expr << ", " << reg->name(n) << endl;
        }

        assign(expr, reg);
    }
}

Register *getreg()
{
    for (auto reg : registers)
        if (reg->_node == nullptr)
            return reg;
    
    load(nullptr, registers[0]);
    return registers[0];
}

void load(Expression *expr)
{
    if (expr->_register == nullptr)
        load(expr, getreg());
}

/*
 * Function:	align (private)
 *
 * Description:	Return the number of bytes necessary to align the given
 *		offset on the stack.
 */

static int align(int offset)
{
    if (offset % STACK_ALIGNMENT == 0)
	return 0;

    return STACK_ALIGNMENT - (abs(offset) % STACK_ALIGNMENT);
}


/*
 * Function:	operator << (private)
 *
 * Description:	Convenience function for writing the operand of an
 *		expression using the output stream operator.
 */

static ostream &operator <<(ostream &ostr, Expression *expr)
{
    if (expr->_register != nullptr)
	return ostr << expr->_register;

    expr->operand(ostr);
    return ostr;
}


/*
 * Function:	Expression::operand
 *
 * Description:	Write an expression as an operand to the specified stream.
 */

void Expression::operand(ostream &ostr) const
{
    assert(_offset != 0);
    ostr << _offset << "(%ebp)";
}


/*
 * Function:	Identifier::operand
 *
 * Description:	Write an identifier as an operand to the specified stream.
 */

void Identifier::operand(ostream &ostr) const
{
    if (_symbol->_offset == 0)
	ostr << global_prefix << _symbol->name();
    else
	ostr << _symbol->_offset << "(%ebp)";
}


/*
 * Function:	Number::operand
 *
 * Description:	Write a number as an operand to the specified stream.
 */

void Number::operand(ostream &ostr) const
{
    ostr << "$" << _value;
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression.
 *
 * 		On a 32-bit Linux platform, the stack needs to be aligned
 * 		on a 4-byte boundary.  (Online documentation seems to
 * 		suggest that 16 bytes are required, but 4 bytes seems to
 * 		work and is much easier.)  Since all arguments are 4-bytes
 *		wide, the stack will always be aligned.
 *
 *		On a 32-bit OS X platform, the stack needs to aligned on a
 *		16-byte boundary.  So, if the stack will not be aligned
 *		after pushing the arguments, we first adjust the stack
 *		pointer.  However, this trick only works if none of the
 *		arguments are themselves function calls.
 *
 *		To handle nested function calls, we need to generate code
 *		for the nested calls first, which requires us to save their
 *		results and then push them on the stack later.  For
 *		efficiency, we only first generate code for the nested
 *		calls, but generate code for ordinary arguments in place.
 */

void Call::generate()
{
    unsigned numBytes;
    Expression *designator, *pointer;


    /* Generate code for any nested function calls first. */

    numBytes = 0;

    for (int i = _args.size() - 1; i >= 0; i --) {
	numBytes += _args[i]->type().size();

	if (STACK_ALIGNMENT != SIZEOF_ARG && _args[i]->_hasCall)
	    _args[i]->generate();
    }


    /* Align the stack if necessary. */

    if (align(numBytes) != 0) {
	cout << "\tsubl\t$" << align(numBytes) << ", %esp" << endl;
	numBytes += align(numBytes);
    }


    /* Generate code for any remaining arguments and push them on the stack. */

    for (int i = _args.size() - 1; i >= 0; i --) {
	if (STACK_ALIGNMENT == SIZEOF_ARG || !_args[i]->_hasCall)
	    _args[i]->generate();

	cout << "\tpushl\t" << _args[i] << endl;
	assign(_args[i], nullptr);
    }


    /* Call the function and then reclaim the stack space. */

    load(nullptr, eax);
    load(nullptr, ecx);
    load(nullptr, edx);

    if (_expr->isDereference(pointer) && _expr->type().isFunction())
	designator = pointer;
    else
	designator = _expr;

    if (designator->type().isPointer()) {
	designator->generate();

	if (designator->_register == nullptr)
	    load(designator, getreg());

	cout << "\tcall\t*" << designator << endl;
	assign(designator, nullptr);

    } else
	cout << "\tcall\t" << designator << endl;

    if (numBytes > 0)
	cout << "\taddl\t$" << numBytes << ", %esp" << endl;

    assign(this, eax);
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
    for (auto stmt : _stmts) {
	stmt->generate();

	for (auto reg : registers)
	    assert(reg->_node == nullptr);
    }
}


/*
 * Function:	Simple::generate
 *
 * Description:	Generate code for a simple (expression) statement, which
 *		means simply generating code for the expression.
 */

void Simple::generate()
{
    _expr->generate();
    assign(_expr, nullptr);
}


/*
 * Function:	Procedure::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 */

void Procedure::generate()
{
    int param_offset;


    /* Assign offsets to the parameters and local variables. */

    param_offset = 2 * SIZEOF_REG;
    offset = param_offset;
    allocate(offset);


    /* Generate our prologue. */

    funcname = _id->name();
    cout << global_prefix << funcname << ":" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << funcname << ".size, %esp" << endl;


    /* Generate the body of this function. */

    _body->generate();


    /* Generate our epilogue. */

    cout << endl << global_prefix << funcname << ".exit:" << endl;
    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl << endl;

    offset -= align(offset - param_offset);
    cout << "\t.set\t" << funcname << ".size, " << -offset << endl;
    cout << "\t.globl\t" << global_prefix << funcname << endl << endl;
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
    const Symbols &symbols = scope->symbols();

    for (auto symbol : symbols)
	if (!symbol->type().isFunction()) {
	    cout << "\t.comm\t" << global_prefix << symbol->name() << ", ";
	    cout << symbol->type().size() << endl;
	}
}


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for an assignment statement.
 *
 *		NOT FINISHED: Only works if the right-hand side is an
 *		integer literal and the left-hand side is an integer
 *		scalar.
 */

void Assignment::generate()
{
    _right->generate();

    load(_right);

    cout << "\tmovl\t" << _right << ", " << _left << endl;

    assign(_right, nullptr);
    assign(_left, nullptr);
}

void Unary::generate(string operation)
{
    _expr->generate();
    load(_expr);

    /* not */
    if (operation == "not")
        cout << "\tcmpl\t" << "$0, " << _expr << endl;

    /* negate */
    if (operation == "negl")
        cout << "\t" << operation << "\t" << _expr << endl;

    assign (_expr, nullptr);
    assign(this, getreg());
}

void Binary::generate(string operation)
{
    _left->generate();
    _right->generate();

    /* divide and remainder */
    if (operation == "idivl")
    {
        load(_left, eax);
        load(nullptr, edx);
        load(_right, ecx);

        cout << "\tcltd\t" << endl;
        cout << "\tidivl\t" << _right << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);
        return;
    }

    load(_left);

    /* addl, subl, and imul */
    if (operation == "addl" || operation == "subl" || operation == "imul")
    {
        cout << "\t" << operation << "\t" << _right << ", " << _left << endl;

        assign(_right, nullptr);
        assign(this, _left->_register);
        return;
    }

    /* comparison */
    if (operation == "setl" || operation == "setg" || operation == "setle" || 
        operation == "setge" || operation == "sete" || operation == "setne")
    {
        load(_left);

        cout << "\tcmpl\t" << _right << ", " << _left << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);

        assign(this, getreg());
        
        cout << "\t" << operation << "\t" << _register->byte() << endl;
        cout << "\tmovzbl\t" << _register->byte() << ", " << _register << endl;
        return;
    }
}

void Add::generate() { Binary::generate("addl"); }

void Subtract::generate() { Binary::generate("subl"); }

void Multiply::generate() { Binary::generate("imul"); }

void Divide::generate()
{
    Binary::generate("idivl");
    assign(this, eax);
}

void Remainder::generate()
{
    Binary::generate("idivl");
    assign(this, edx);
}

void LessThan::generate() { Binary::generate("setl"); }

void GreaterThan::generate() { Binary::generate("setg"); }

void LessOrEqual::generate() { Binary::generate("setle"); }

void GreaterOrEqual::generate() { Binary::generate("setge"); }

void Equal::generate() { Binary::generate("sete"); }

void NotEqual::generate() { Binary::generate("setne"); }

void Not::generate()
{
    Unary::generate("not");

    cout << "\tsete\t" << _register->byte() << endl;
    cout << "\tmovzbl\t" << _register->byte() << ", " << _register << endl;
}

void Negate::generate() { Unary::generate("negl"); }

/* CHECK L21S09 */

// void Expression::test(const Label &label, bool ifTrue)
// {
//     generate();

//     if (_register == nullptr)
//         load(this, getreg());

//     cout << "\tcmpl\t$0, " << this << endl;
//     cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

//     assign(this, nullptr);
// }

/* This is really all the students need to do, and they can even skip the
   loop for stack alignment. */

# if 0
void Function::generate()
{
    Parameters *params = _id->type().parameters();
    const Symbols &symbols = _body->declarations()->symbols();

    offset = 0;

    for (unsigned i = 0; i < symbols.size(); i ++)
	if (i >= params->size()) {
	    offset -= symbols[i]->type().size();
	    symbols[i]->_offset = offset;
	} else
	    symbols[i]->_offset = 8 + 4 * i;

    while ((offset - 8) % STACK_ALIGNMENT != 0)
	offset --;

    cout << global_prefix << _id->name() << ":" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << -offset << ", %esp" << endl;

    _body->generate();

    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl << endl;

    cout << "\t.globl\t" << global_prefix << _id->name() << endl << endl;
}
# endif
