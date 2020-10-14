#pragma once
#ifndef OTOS_TRAITS_DEVICE_H
#define OTOS_TRAITS_DEVICE_H

#include <cfp/traits/fwd/device.h>
#include <cfp/model/simulation.h>

namespace cfp::traits {

	template <typename Model, typename Device>
	struct device<::cfp::simulation<Model, Device>> {
		using type = Device;
	};
}

#endif // OTOS_TRAITS_DEVICE_H