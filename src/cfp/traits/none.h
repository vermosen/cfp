#pragma once
#ifndef OTOS_TRAITS_NONE_H
#define OTOS_TRAITS_NONE_H

namespace cfp::traits {

	template <typename T>
	struct none {
		static constexpr typename std::underlying_type<T>::type value = 0;
	};
}

#endif // OTOS_TRAITS_NONE_H