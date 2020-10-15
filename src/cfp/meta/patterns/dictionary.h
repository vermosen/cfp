#pragma once
#ifndef CFP_UTILS_DICTIONARY_H
#define CFP_UTILS_DICTIONARY_H

#include <sstream>
#include <typeinfo>

#include <cfp/meta/patterns/fwd/dictionary.h>

#include <cfp/traits/none.h>
#include <cfp/traits/delimiter.h>
#include <cfp/traits/composable.h>

namespace cfp {
namespace util {

  template <typename Enum>
  const std::map<Enum, std::string> dictionary<Enum>::m_left = dictionary<Enum>::left();

  template <typename Enum>
  const std::map<std::string, Enum> dictionary<Enum>::m_right = dictionary<Enum>::right();

  template<typename Enum>
  inline std::map<Enum, std::string> dictionary<Enum>::left() {
    std::map<Enum, std::string> m;
    for (auto x : create<Enum>::initalizers())
      m[x.first] = std::string(x.second);
    return m;
  }

  template<typename Enum>
  inline std::map<std::string, Enum> dictionary<Enum>::right() {
    std::map<std::string, Enum> m;
    for (auto x : create<Enum>::initalizers())
      m[std::string(x.second)] = x.first;
    return m;
  }

  template <typename Enum>
  inline std::string dictionary<Enum>::key(Enum en) {

    using ud_type = typename std::underlying_type<Enum>::type;

    std::stringstream sss;

    if constexpr (traits::composable<Enum>::value == true) {

      bool valid = false; char sep = '(';
      for (auto& kv : m_left) {

        auto _and = static_cast<ud_type>(en);
        _and &= static_cast<ud_type>(kv.first);

        // if there's a zero key, then we always append
        if (_and != 0 || static_cast<ud_type>(kv.first) == 0) { 
          sss << sep << kv.second;
          valid = true;
          sep = traits::delimiter<Enum>::value;
        }
      }

      if (valid) {
        sss << ')';
        return sss.str();
      }
    }
    else {
      auto it = dictionary<Enum>::m_left.find(en);
      if (it != dictionary<Enum>::m_left.cend())
        return it->second;
    }

    sss << "[key not found for type "
      << typeid(en).name() << "]";

    return sss.str();
  }

  template<typename Enum>
  inline bool dictionary<Enum>::value_impl(const std::string& str, Enum& en) noexcept {

    // reset the enum
    en = static_cast<Enum>(traits::none<Enum>::value);

    if (str.empty()) return false;

    if constexpr (traits::composable<Enum>::value == true) {
      if (*str.cbegin() == '(' && *str.crbegin() == ')') {      // TODO: handle the case without parenthesis

        std::size_t pos = 1; bool exit = false;

        // we're trying to interpolate the string "(val1|val2|...|valn)"
        while (!exit) {

          auto end = str.find(traits::delimiter<Enum>::value, pos);

          if (end == std::string::npos) {
            end = str.size() - 1; exit = true;
          }

          auto key = str.substr(pos, end - pos);
          const auto& kv = dictionary<Enum>::m_right.find(key);
          if (kv != dictionary<Enum>::m_right.cend()) {
            en = en | kv->second;
            pos = end + 1;
          }
          else {
            return false;
          }
        }

        return true;
      }
      else {
        auto it = dictionary<Enum>::m_right.find(str);
        if (it != dictionary<Enum>::m_right.cend()) {
          en = it->second;
          return true;
        }
      }
    }
    else {
      auto it = dictionary<Enum>::m_right.find(str);
      if (it != dictionary<Enum>::m_right.cend()) {
        en = it->second;
        return true;
      }
    }

    return false;
  }

  template<typename Enum>
  inline Enum dictionary<Enum>::value(const std::string& str, std::nothrow_t) noexcept {

    auto en = static_cast<Enum>(traits::none<Enum>::value);
    value_impl(str, en);
    return en;
  }

  template<typename Enum>
  inline Enum dictionary<Enum>::value(const std::string& str) {

    auto en = static_cast<Enum>(traits::none<Enum>::value);

    if (!value_impl(str, en)) {
      throw std::logic_error("unknown enum value passed");
    }

    return en;
  }
}}

#endif // CFP_UTILS_DICTIONARY_H