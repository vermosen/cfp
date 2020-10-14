#pragma once
#ifndef OTOS_UTILS_FWD_DICTIONARY_H
#define OTOS_UTILS_FWD_DICTIONARY_H

#include <map>
#include <string>
#include <array>

#include <cfp/meta/preprocessor/pair.h>
#include <cfp/meta/preprocessor/size.h>

namespace cfp {
namespace util {

  // an enum dictionary and conversion class
  template <typename Enum>
  struct dictionary {
    dictionary() {}
    ~dictionary() {}
    static std::string key(Enum);
    
    static Enum value(const std::string&);
    static Enum value(const std::string&, std::nothrow_t) noexcept;

    static const std::map<Enum, std::string> m_left;			// enum to string
    static const std::map<std::string, Enum> m_right;			// string to enum

  private:
    static std::map<Enum, std::string> left();
    static std::map<std::string, Enum> right();

    static bool value_impl(const std::string&, Enum&) noexcept;
  };

  template <typename Enum>
  class create;
}}

// macro for declaring an enum dictionary
#define OTOS_DECLARE_ENUM(X, SEQ)							              \
namespace cfp {                                            \
namespace util {                                            \
                                                            \
  template <>                                               \
  class create<X> : public dictionary<X> {                  \
  private:                                                  \
    create();                                               \
                                                            \
  public:                                                   \
    static constexpr std::size_t size = OTOS_SIZE(SEQ)      \
                                                            \
    static constexpr const                                  \
    std::array<std::pair<X, const char*>, size>             \
    initalizers() {                                         \
      return                                                \
      {                                                     \
        {                                                   \
          OTOS_FOR_EACH_DECLARE_PAIR(X, SEQ)		            \
        }                                                   \
      };                                                    \
    }                                                       \
  };                                                        \
}}                                                          \

#endif // OTOS_UTILS_FWD_DICTIONARY_H