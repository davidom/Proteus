#include "face.hxx"
#include "face_impl.hxx"
#include <vector>

namespace Proteus
{
  /**
	* Face Class Implementation
	*/

  // Destructor
  face::impl::~impl(){}

  // Copy Constructor
  face::impl::impl(const impl & rhs) : 
  	nl_(rhs.nl_)
  	{}

  // Only Public Constructor
  face::impl::impl(const std::initializer_list<size_t> &&l) : 
  	nl_{ std::move(l) } 
	{}

  face::impl::impl(const std::initializer_list<size_t> &l) : 
  	nl_{ l } 
	{}
}
