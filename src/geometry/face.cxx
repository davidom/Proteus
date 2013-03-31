#include "face.hxx"
#include "face_impl.hxx"
#include <vector>

namespace Proteus
{

  /*
	Face Class Interface
  */
  // Only Public Constructor
  face::face(const std::initializer_list<size_t> &&l) : 
  	pimpl_{ new impl(std::move(l)) }
	{}

  face::face(const std::initializer_list<size_t> &l) : 
  	pimpl_{ new impl( l ) }
	{}

  // Destructor
  face::~face() {}

  // Copy Constructor
  face::face(const face &other) :
  	pimpl_{new impl(*(other.pimpl_))} 
	{}

  // Assignment Operator
  face & face::operator=
  (const face & rhs)
  {
	pimpl_->def_ = rhs.pimpl_->def_;
	return *this;
  }

  // Public Interface
  //-operator[]
  const size_t & face::operator[]
  (const size_t & n) const
  {
	return pimpl_->def_.at(n);
  }

  const size_t & face::operator[]
  (const size_t && n) const
  {
	return pimpl_->def_.at(std::move(n));
  }

}
