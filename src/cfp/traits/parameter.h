#pragma once
#ifndef CFP_TRAITS_PARAMETER_H
#define CFP_TRAITS_PARAMETER_H

#include <cfp/traits/fwd/parameter.h>
#include <cfp/model/simulation.h>
#include <cfp/model/parameter.h>

namespace cfp::traits {

	template <typename Model, typename Device>
	struct parameter<simulation<Model, Device>> {
		using type = cfp::parameter<Model>;
	};
}

#endif // CFP_TRAITS_PARAMETER_H