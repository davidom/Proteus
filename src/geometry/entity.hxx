#ifndef __PROTEUS_ENTITY_CLASS__
#define __PROTEUS_ENTITY_CLASS__

#include <utility>

namespace Proteus
{
  /** \brief Entity Class
   *
   * This class is a wrapper for a container and was designed with the STL
   *  containers in mind. To that end, the entity must be defined at the time
   *  it is instantiated. Once defined, the data the class wraps cannot be
   *  changed except through the supplied API for the command class.
  */

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
      /// Constructor
      template<typename ...Args> entity(Args&& ...args) :
        def_{{ std::forward<Args>(args)... }} {}
      const entity<T> & operator= (const entity<T> & rhs)
        { this->def_ = rhs.def_;}
      const entity<T> & operator= (const entity<T> && rhs)
        { this->def_ = std::move(rhs.def_);}
      /// Data Access
      auto operator[] (const std::size_t & n) const -> decltype(def_[n]) 
        {return def_[n];}
  };
}

#endif
