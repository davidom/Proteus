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
  	def_(rhs.def_)
  	{}

  // Only Public Constructor
  face::impl::impl(const std::initializer_list<size_t> &&l) : 
  	def_{ std::move(l) } 
	{}

  face::impl::impl(const std::initializer_list<size_t> &l) : 
  	def_{ l } 
	{}
}
