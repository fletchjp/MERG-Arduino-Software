/// @file compiler.cpp
/// @brief function bodies for the compiler
/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "compiler.hpp"
#include "vm.hpp"
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <iostream>
// 3rd party libraries
#include <Streaming.h>

/// @brief This provides an operator for streaming the output of std::string.
///
/// I have added this here as putting in ArduinoCode.h gives duplicate defintions.
inline Print &operator <<(Print &stream, const std::string &arg)
{
  stream.print(arg.c_str());
  return stream;
}

/// This provides an operator for streaming the output of char*.
inline Print &operator <<(Print &stream, const char *arg)
{
  stream.print(arg);
  return stream;
}


namespace client { namespace code_gen
{
    void program::op(int a)
    {
        code.push_back(a);
    }

    void program::op(int a, int b)
    {
        code.push_back(a);
        code.push_back(b);
    }

    void program::op(int a, int b, int c)
    {
        code.push_back(a);
        code.push_back(b);
        code.push_back(c);
    }

    int const* program::find_var(std::string const& name) const
    {
        auto i = variables.find(name);
        if (i == variables.end())
            return 0;
        return &i->second;
    }

    void program::add_var(std::string const& name)
    {
        std::size_t n = variables.size();
        variables[name] = int(n);
    }

    void program::print_variables(std::vector<int> const& stack) const
    {
        for (auto const& p : variables)
        {
            Serial << "    " << p.first << ": " << stack[p.second] << endl;
        }
    }

    void program::print_assembler() const
    {
        auto pc = code.begin();

        std::vector<std::string> locals(variables.size());
        typedef std::pair<std::string, int> pair_;
        for (pair_ const& p : variables)
        {
            locals[p.second] = p.first;
            Serial << "local       "
                << p.first << ", @" << p.second << endl;
        }

        while (pc != code.end())
        {
            switch (*pc++)
            {
                case op_neg:
                    Serial << "op_neg" << endl;
                    break;

                case op_add:
                    Serial << "op_add" << endl;
                    break;

                case op_sub:
                    Serial << "op_sub" << endl;
                    break;

                case op_mul:
                    Serial << "op_mul" << endl;
                    break;

                case op_div:
                    Serial << "op_div" << endl;
                    break;

                case op_load:
                    Serial << "op_load     " << locals[*pc++] << endl;
                    break;

                case op_store:
                    Serial << "op_store    " << locals[*pc++] << endl;
                    break;

                case op_int:
                    Serial << "op_int      " << *pc++ << endl;
                    break;

                case op_stk_adj:
                    Serial << "op_stk_adj  " << *pc++ << endl;
                    break;
            }
        }
    }

    bool compiler::operator()(unsigned int x) const
    {
        program.op(op_int, x);
        return true;
    }

    bool compiler::operator()(ast::variable const& x) const
    {
        int const* p = program.find_var(x.name_);
        if (p == 0)
        {
            error_handler(x, "Undeclared variable: " + x.name_);
            return false;
        }
        program.op(op_load, *p);
        return true;
    }

    bool compiler::operator()(ast::operation const& x) const
    {
        if (!boost::apply_visitor(*this, x.operand_))
            return false;
        switch (x.operator_)
        {
            case '+': program.op(op_add); break;
            case '-': program.op(op_sub); break;
            case '*': program.op(op_mul); break;
            case '/': program.op(op_div); break;
            default: BOOST_ASSERT(0); return false;
        }
        return true;
    }

    bool compiler::operator()(ast::signed_ const& x) const
    {
        if (!boost::apply_visitor(*this, x.operand_))
            return false;
        switch (x.sign)
        {
            case '-': program.op(op_neg); break;
            case '+': break;
            default: BOOST_ASSERT(0); return false;
        }
        return true;
    }

    bool compiler::operator()(ast::expression const& x) const
    {
        if (!boost::apply_visitor(*this, x.first))
            return false;
        for (ast::operation const& oper : x.rest)
        {
            if (!(*this)(oper))
                return false;
        }
        return true;
    }

    bool compiler::operator()(ast::assignment const& x) const
    {
        if (!(*this)(x.rhs))
            return false;
        int const* p = program.find_var(x.lhs.name_);
        if (p == 0)
        {
            error_handler(x.lhs, "Undeclared variable: " + x.lhs.name_);
            return false;
        }
        program.op(op_store, *p);
        return true;
    }

    bool compiler::operator()(ast::variable_declaration const& x) const
    {
        int const* p = program.find_var(x.assign.lhs.name_);
        if (p != 0)
        {
            error_handler(x.assign.lhs, "Duplicate variable: " + x.assign.lhs.name_);
            return false;
        }
        bool r = (*this)(x.assign.rhs);
        if (r) // don't add the variable if the RHS fails
        {
            program.add_var(x.assign.lhs.name_);
            program.op(op_store, *program.find_var(x.assign.lhs.name_));
        }
        return r;
    }

    bool compiler::operator()(ast::statement_list const& x) const
    {
        program.clear();

        // op_stk_adj 0 for now. we'll know how many variables we'll have later
        program.op(op_stk_adj, 0);
        for (ast::statement const& s : x)
        {
            if (!boost::apply_visitor(*this, s))
            {
                program.clear();
                return false;
            }
        }
        program[1] = int(program.nvars()); // now store the actual number of variables
        return true;
    }
}}
