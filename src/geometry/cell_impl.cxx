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
  	def_(rhs.def_)
  	{}

  // Only Public Constructor
  cell::impl::impl(const std::initializer_list<size_t> &&l) : 
  	def_{ std::move(l) } 
	{}

  cell::impl::impl(const std::initializer_list<size_t> &l) : 
  	def_{ l } 
	{}
}
