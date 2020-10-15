#pragma once
#ifndef CFP_TRAITS_DELIMITER_H
#define CFP_TRAITS_DELIMITER_H

namespace cfp::traits {

	template <typename T>
	struct delimiter {
		static constexpr char value = '|';
	};
}

#endif // CFP_TRAITS_DELIMITER_H