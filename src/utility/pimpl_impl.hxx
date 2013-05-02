#ifndef __PROTEUS_PIMPL_IMPL__
#define __PROTEUS_PIMPL_IMPL__

#include <utility>

namespace Proteus
{
  /*
	The following code is borrowed as is from herbsutter.com/gotw/_101/
  */

  template<typename T>
  pimpl<T>::pimpl() : m{ new T{} } { }
 
  template<typename T>
  template<typename ...Args>
  pimpl<T>::pimpl( Args&& ...args )
     : m{ new T{ std::forward<Args>(args)... } } { }
	  
  template<typename T>
  pimpl<T>::~pimpl() { }
   
  template<typename T>
  T* pimpl<T>::operator->() { return m.get(); }
    
  template<typename T>
  T& pimpl<T>::operator*() { return *m.get(); }
}

#endif
