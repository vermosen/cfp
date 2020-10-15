#pragma once
#ifndef CFP_TRAITS_COMPOSABLE_H
#define CFP_TRAITS_COMPOSABLE_H

namespace cfp::traits {

	template <typename T>
	struct composable {
		static constexpr bool value = false;
	};
}

#endif // CFP_TRAITS_COMPOSABLE_H