#pragma once
#ifndef OTOS_TRAITS_DELIMITER_H
#define OTOS_TRAITS_DELIMITER_H

namespace cfp::traits {

	template <typename T>
	struct delimiter {
		static constexpr char value = '|';
	};
}

#endif // OTOS_TRAITS_DELIMITER_H