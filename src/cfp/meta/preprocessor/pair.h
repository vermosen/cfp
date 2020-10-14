#pragma once
#ifndef OTOS_META_PREPROCESSOR_PAIR_H
#define OTOS_META_PREPROCESSOR_PAIR_H

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <cfp/meta/preprocessor/sequence.h>

#define OTOS_DECLARE_PAIR(R, DATA, I, ELEM)                     \
    BOOST_PP_COMMA_IF(I) {                                      \
          DATA::BOOST_PP_TUPLE_ELEM(2, 0, ELEM)                 \
        , BOOST_PP_TUPLE_ELEM(2, 1, ELEM)                       \
    }                                                           \


#define OTOS_FOR_EACH_DECLARE_PAIR(X, SEQ)                      \
	BOOST_PP_SEQ_FOR_EACH_I(OTOS_DECLARE_PAIR                     \
        , X, OTOS_SEQ_DOUBLE_PARENS(SEQ))

#endif // OTOS_META_PREPROCESSOR_PAIR_H