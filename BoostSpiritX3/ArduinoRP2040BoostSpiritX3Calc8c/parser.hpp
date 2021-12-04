/// @file parser.hpp
/// @brief parser for custom diagnostics handler
///
/// Copied from main program

#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "vm.hpp"
#include "compiler.hpp"

#include "statement_def.hpp"

namespace x3 = boost::spirit::x3;

/// parse function moved from the main program
bool parse(std::string const& source) {
    using client::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

#ifdef BOOST_SPIRIT_X3_NO_EXCEPTION
    /// Declare position cache
    x3::position_cache<std::vector<iterator_type> > pos_cache(iter, end);
    /// Declare diagnostics handler
    custom::diagnostics_handler<iterator_type> diags { iter, end };
#endif

    client::vmachine vm;                                    // Our virtual machine
    client::code_gen::program program;                      // Our VM program
    client::ast::statement_list ast;                        // Our AST

    using boost::spirit::x3::with;
    using client::parser::error_handler_type;
    using client::parser::error_handler_tag;
    error_handler_type error_handler(iter, end, std::cerr); // Our error handler

    // Our compiler
    client::code_gen::compiler compile(program, error_handler);

#ifdef BOOST_SPIRIT_X3_NO_EXCEPTION
    /// change the tag in the with for the position cache. 
    auto const parser =
         //with<client::parser::annotate_position>(std::ref(pos_cache)) [
         with<client::parser::position_cache_tag>(std::ref(pos_cache)) [
            //with<custom::diagnostics_handler_tag>(diags) [
                 client::statement()
            //]
        ];
#else
    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler))
        [
            client::statement()
        ];
#endif

    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

        Serial << "-------------------------\n";

    if (success && iter == end)
    {
        if (compile(ast))
        {
            Serial << "Success\n";
            Serial << "-------------------------\n";
            vm.execute(program());

            Serial << "-------------------------\n";
            Serial << "Assembler----------------\n";
            program.print_assembler();

            Serial << "-------------------------\n";
            Serial << "Results------------------\n";
            program.print_variables(vm.get_stack());
        }
        else
        {
            Serial << "Compile failure\n";
        }
    }
    else
    {
        Serial << "Parse failure\n";
    }

    Serial << "-------------------------\n\n";

    
    
    return success;
}





#endif
