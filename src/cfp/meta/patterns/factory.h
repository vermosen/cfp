#pragma once
#ifndef OTOS_META_PATTERNS_FACTORY_H
#define OTOS_META_PATTERNS_FACTORY_H

#include <map>
#include <memory>

namespace cfp {
namespace meta {

  /*!
    * An abstract factory template class.
    * The template arguments are:
    *   - Base: the base class to instanciate
    *   - Key: the key type used to retrieve the implementation
    *   - Args: the derived classes ctor argument list
    */
  template <
      typename Base
    , typename Key
    , typename ... Args
  > class factory {
  protected:
    typedef std::map<Key, Base* (*)(Args ...)> map_type; // map of constructors

  public:
    static std::shared_ptr<Base> create_instance(
        Key const& key
      , Args ... args) {

      typename map_type::iterator it = get_map()->find(key);

      if (it == get_map()->end())
        return std::shared_ptr<Base>();
      else
        return std::shared_ptr<Base>(it->second(args...));
    }

    static std::size_t size() {
      return get_map()->size();
    }

  protected:
    static std::unique_ptr<map_type>& get_map() {
      if (!m_map) {
        m_map.reset(new map_type());
      }

      return m_map;
    }

    static std::unique_ptr<map_type> m_map;
  };

  namespace details {

    template <
        typename Base
      , typename Derived
      , typename ... Args
    >
      inline Base* create(Args ... args) {        // create method
      return new Derived(args...);
    }
  }

  template <
      typename Base
    , typename Key
    , typename Derived
    , typename ... Args
  > struct register_class : factory<Base, Key, Args...> {
    register_class(Key const& key) {
      factory<Base, Key, Args...>::get_map()->insert(
        std::make_pair(key, &details::create<Base, Derived, Args...>));
    }
  };
}}

#define DEFINE_FACTORY(B, K, ...)                                   \
namespace cfp { namespace meta {                                   \
template<>                                                          \
std::unique_ptr<typename factory<B, K, ##__VA_ARGS__>::map_type>    \
factory<                                                            \
    B                                                               \
  , K                                                               \
  , ##__VA_ARGS__                                                   \
>::m_map                                                            \
__attribute__((init_priority(300)))                                 \
(new typename factory<B, K, ##__VA_ARGS__>::map_type());            \
}}

#define DECLARE_REGISTER(B, K, D, ...)    \
static cfp::meta::register_class<        \
  B, K, D, ##__VA_ARGS__                  \
> m_register

#define DEFINE_REGISTER(B, K, V, D, ...)  \
cfp::meta::register_class<               \
      B, K, D, ##__VA_ARGS__              \
> D::m_register(V)

#endif // OTOS_META_PATTERNS_FACTORY_H
