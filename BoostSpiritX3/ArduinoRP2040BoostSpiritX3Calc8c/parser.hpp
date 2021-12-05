/// @file parser.hpp
/// @brief parser for custom diagnostics handler
///
/// Copied from main program
///
/// This is the focus of the remaining problem.
///
/// There is an undefined reference to client::parser::parse_rule with a very long name.
///
/// It seems that the parse rule for the parser is not being instantiated.

// without diagnostics handler
// undefined reference to 
// `_ZN6client6parser10parse_ruleIN9__gnu_cxx17__normal_iteratorIPKcNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEEN5boost6spirit2x37contextINS0_18position_cache_tagESt17reference_wrapperINSF_14position_cacheISt6vectorISC_SaISC_EEEEENSG_INSF_11skipper_tagEKNSF_10char_classINSE_13char_encoding5asciiENSF_9space_tagEEENSF_11unused_typeEEEEEEEbNSF_6detail7rule_idINS0_16expression_classEEERT_RKS13_RKT0_RNS_3ast10expressionE'
// collect2.exe: error: ld returned 1 exit status

// with diagnostics handler
//`_ZN6client6parser10parse_ruleIN9__gnu_cxx17__normal_iteratorIPKcNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEEN5boost6spirit2x37contextIN6custom23diagnostics_handler_tagENSH_19diagnostics_handlerISC_EENSG_INS0_18position_cache_tagESt17reference_wrapperINSF_14position_cacheISt6vectorISC_SaISC_EEEEENSG_INSF_11skipper_tagEKNSF_10char_classINSE_13char_encoding5asciiENSF_9space_tagEEENSF_11unused_typeEEEEEEEEEbNSF_6detail7rule_idINS0_16expression_classEEERT_RKS18_RKT0_RNS_3ast10expressionE'
// collect2.exe: error: ld returned 1 exit status

#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "vm.hpp"
#include "compiler.hpp"

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
            with<custom::diagnostics_handler_tag>(diags) [
                 client::statement()
            ]
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
        for (auto const& statement : ast) {
            // I need to sort out an output statement.
            //Serial << "got: " << statement << endl;
            diags(pos_cache.position_of(ast).begin(), " trial to see what happens");
        }
        Serial << "-------------------------\n";
      
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
