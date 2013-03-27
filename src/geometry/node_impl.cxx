#include "node.hxx"
#include "node_impl.hxx"
#include <vector>

namespace Proteus
{
  /**
	* Node Class Implementation
	*/

  // Destructor
  node::impl::~impl(){}

  // Copy Constructor
  node::impl::impl(const impl & rhs) : 
  	pos_(rhs.pos_)
  	{}

  // Only Public Constructor
  node::impl::impl(const std::initializer_list<double> &&l) : 
  	pos_{ std::move(l) } 
	{}

  node::impl::impl(const std::initializer_list<double> &l) : 
  	pos_{ l } 
	{}
}
