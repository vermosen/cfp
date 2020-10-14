#pragma once
#ifndef OTOS_META_PREPROCESSOR_SEQUENCE_H
#define OTOS_META_PREPROCESSOR_SEQUENCE_H

#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

// Doubles the parentheses of a Boost.PP sequence,
// i.e. (a, b)(c, d) becomes ((a, b))((c, d))
#define OTOS_SEQ_DOUBLE_PARENS_0(...) \
     ((__VA_ARGS__)) OTOS_SEQ_DOUBLE_PARENS_1

#define OTOS_SEQ_DOUBLE_PARENS_1(...) \
     ((__VA_ARGS__)) OTOS_SEQ_DOUBLE_PARENS_0

#define OTOS_SEQ_DOUBLE_PARENS_0_END
#define OTOS_SEQ_DOUBLE_PARENS_1_END

#define OTOS_SEQ_DOUBLE_PARENS(SEQ) \
    BOOST_PP_CAT(OTOS_SEQ_DOUBLE_PARENS_0 SEQ, _END)

#endif // OTOS_META_PREPROCESSOR_SEQUENCE_H
