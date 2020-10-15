#pragma once
#ifndef CFP_META_PREPROCESSOR_SEQUENCE_H
#define CFP_META_PREPROCESSOR_SEQUENCE_H

#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

// Doubles the parentheses of a Boost.PP sequence,
// i.e. (a, b)(c, d) becomes ((a, b))((c, d))
#define CFP_SEQ_DOUBLE_PARENS_0(...) \
     ((__VA_ARGS__)) CFP_SEQ_DOUBLE_PARENS_1

#define CFP_SEQ_DOUBLE_PARENS_1(...) \
     ((__VA_ARGS__)) CFP_SEQ_DOUBLE_PARENS_0

#define CFP_SEQ_DOUBLE_PARENS_0_END
#define CFP_SEQ_DOUBLE_PARENS_1_END

#define CFP_SEQ_DOUBLE_PARENS(SEQ) \
    BOOST_PP_CAT(CFP_SEQ_DOUBLE_PARENS_0 SEQ, _END)

#endif // CFP_META_PREPROCESSOR_SEQUENCE_H
