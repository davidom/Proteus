#include "edge.hxx"
#include "edge_impl.hxx"
#include <vector>

namespace Proteus
{
  /**
	* Edge Class Implementation
	*/

  // Destructor
  edge::impl::~impl(){}

  // Copy Constructor
  edge::impl::impl(const impl & rhs) : 
  	nl_(rhs.nl_)
  	{}

  // Only Public Constructor
  edge::impl::impl(const std::initializer_list<size_t> &&l) : 
  	nl_{ std::move(l) } 
	{}

  edge::impl::impl(const std::initializer_list<size_t> &l) : 
  	nl_{ l } 
	{}
}
