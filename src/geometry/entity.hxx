#ifndef __PROTEUS_ENTITY_CLASS__
#define __PROTEUS_ENTITY_CLASS__

#include <memory>

namespace Proteus
{
  /**
  	* \brief Entity Class Template Implementation
	*/

  template<typename T>
  class
  entity_impl
  {
	private:
	  entity_impl() = delete;
	  T def_;
	public:
	  template<typename ...Args> entity_impl(Args&& ...);
	  entity_impl(const entity_impl<T> & rhs);
	  ~entity_impl();

	  auto operator[](const size_t & n) -> decltype(def_[n])
	  { return def_[n]; };
  };

  template<typename T>
  template<typename ...Args>
  entity_impl<T>::entity_impl(Args&& ...args)
    : def_{{ std::forward<Args>(args)... }}
	  {}

  template<typename T>
  entity_impl<T>::entity_impl(const entity_impl<T> & rhs)
    : def_{{ rhs.def_ }}
	  {}

  template<typename T>
  entity_impl<T>::~entity_impl() {}

  /**
    * \brief Entity Class Template Interface
	*/

  template<typename T>
  class
  entity
  {
	private:
	  entity() = delete;
	  std::unique_ptr<T> pimpl_;

	public:
	  template<typename ...Args> entity(Args&& ...);
	  entity(const entity<T> & rhs);
	  ~entity();

	  auto operator[](const size_t & n) -> decltype((*pimpl_)[n])
	  { return (*pimpl_)[n]; };
  };

  template<typename T>
  template<typename ...Args>
  entity<T>::entity(Args&& ...args)
    : pimpl_{{ new T{ std::forward<Args>(args)... } }}
	  {}

  template<typename T>
  entity<T>::entity(const entity<T> & rhs)
    : pimpl_{{ new T(*(rhs.pimpl_)) }}
	  {}

  template<typename T>
  entity<T>::~entity() {}

}

#endif
