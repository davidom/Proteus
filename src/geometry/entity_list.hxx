#ifndef __PROTEUS_ENTITY_LIST_CLASS__
#define __PROTEUS_ENTITY_LIST_CLASS__

#include <vector>

namespace Proteus
{

  template
  <
    typename T
  >
  class entity_list
  {
	private:
	  std::vector<T> container_;

	public:
	  entity_list() = default;
	  entity_list(size_t n) { container_.reserve(n); }
	  ~entity_list() = default;

	  template<typename ...Args>
	  void push(Args&& ...args) { container_.emplace_back( std::forward<Args>(args)... ); }
	  size_t size() { return container_.size(); }
	  const T& operator[] (size_t n) { return container_[n]; }
  };

}
#endif
