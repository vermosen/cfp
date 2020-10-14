#pragma once
#ifndef OTOS_TRAITS_COMPOSABLE_H
#define OTOS_TRAITS_COMPOSABLE_H

namespace cfp::traits {

	template <typename T>
	struct composable {
		static constexpr bool value = false;
	};
}

#endif // OTOS_TRAITS_COMPOSABLE_H