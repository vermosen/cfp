#pragma once
#ifndef OTOS_META_PREPROCESSOR_SIZE_H
#define OTOS_META_PREPROCESSOR_SIZE_H

#include <boost/preprocessor/seq/size.hpp>

#include <cfp/meta/preprocessor/sequence.h>

#define OTOS_SIZE(SEQ) \
	BOOST_PP_SEQ_SIZE(OTOS_SEQ_DOUBLE_PARENS(SEQ));

#endif
