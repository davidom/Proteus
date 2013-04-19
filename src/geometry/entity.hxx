#ifndef __PROTEUS_ENTITY_CLASS__
#define __PROTEUS_ENTITY_CLASS__

#include <utility>

namespace Proteus
{
  template<typename T>
  class entity
  {
	private:
	  entity() = delete;
	  T def_;

	public:
	  entity(entity<T> & rhs) : def_( rhs.def_ ) {}
	  entity(const entity<T> & rhs) : def_( rhs.def_ ) {}
	  entity(entity<T> && rhs) : def_( std::move(rhs.def_) ) {}
	  entity(const entity<T> && rhs) : def_( std::move(rhs.def_) ) {}
	  template<typename ...Args> entity(Args&& ...args) :
	    def_{{ std::forward<Args>(args)... }} {}
	  auto operator[] (const std::size_t & n) const -> decltype(def_[n]) 
	    {return def_[n];}
	  const entity<T> & operator= (const entity<T> & rhs)
	    { this->def_ = rhs.def_;}
	  const entity<T> & operator= (const entity<T> && rhs)
	    { this->def_ = std::move(rhs.def_);}
  };
}

#endif
