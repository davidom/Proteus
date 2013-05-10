#ifndef __PROTEUS_PIMPL_IDIOM__
#define __PROTEUS_PIMPL_IDIOM__

#include <memory>

namespace Proteus
{
  /*
	The following code is borrowed as is from herbsutter.com/gotw/_101/
  */

  template<typename T>
  class pimpl {
	private:
	  std::unique_ptr<T> m;
	public:
	  pimpl();
	  template<typename ...Args> pimpl(Args&& ...);
	  ~pimpl();
	  T* operator->();
	  T& operator*();
  };
}
#endif
