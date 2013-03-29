#include "cell.hxx"
#include "cell_impl.hxx"
#include <vector>

namespace Proteus
{

  /*
	Cell Class Interface
  */
  // Only Public Constructor
  cell::cell(const std::initializer_list<size_t> &&l) : 
  	pimpl_{ new impl(std::move(l)) }
	{}

  cell::cell(const std::initializer_list<size_t> &l) : 
  	pimpl_{ new impl( l ) }
	{}

  // Destructor
  cell::~cell() {}

  // Copy Constructor
  cell::cell(const cell &other) :
  	pimpl_{new impl(*(other.pimpl_))} 
	{}

  // Assignment Operator
  cell & cell::operator=
  (const cell & rhs)
  {
	pimpl_->nl_ = rhs.pimpl_->nl_;
	return *this;
  }

  // Public Interface
  //-operator[]
  const size_t & cell::operator[]
  (const size_t & n) const
  {
	return pimpl_->nl_.at(n);
  }

  const size_t & cell::operator[]
  (const size_t && n) const
  {
	return pimpl_->nl_.at(std::move(n));
  }

}
