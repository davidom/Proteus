#include "cell.hxx"
#include "cell_impl.hxx"
#include <vector>

namespace Proteus
{
  /**
	* Cell Class Implementation
	*/

  // Destructor
  cell::impl::~impl(){}

  // Copy Constructor
  cell::impl::impl(const impl & rhs) : 
  	nl_(rhs.nl_)
  	{}

  // Only Public Constructor
  cell::impl::impl(const std::initializer_list<size_t> &&l) : 
  	nl_{ std::move(l) } 
	{}

  cell::impl::impl(const std::initializer_list<size_t> &l) : 
  	nl_{ l } 
	{}
}
