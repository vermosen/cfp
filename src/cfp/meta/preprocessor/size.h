#pragma once
#ifndef CFP_META_PREPROCESSOR_SIZE_H
#define CFP_META_PREPROCESSOR_SIZE_H

#include <boost/preprocessor/seq/size.hpp>

#include <cfp/meta/preprocessor/sequence.h>

#define CFP_SIZE(SEQ) \
	BOOST_PP_SEQ_SIZE(CFP_SEQ_DOUBLE_PARENS(SEQ));

#endif
