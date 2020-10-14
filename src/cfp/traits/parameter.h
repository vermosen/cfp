#pragma once
#ifndef OTOS_TRAITS_PARAMETER_H
#define OTOS_TRAITS_PARAMETER_H

#include <cfp/traits/fwd/parameter.h>
#include <cfp/model/simulation.h>
#include <cfp/model/parameter.h>

namespace cfp::traits {

	template <typename Model, typename Device>
	struct parameter<::cfp::simulation<Model, Device>> {
		using type = cfp::parameter<Model>;
	};
}

#endif // OTOS_TRAITS_PARAMETER_H