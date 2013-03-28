#include "edge.hxx"
#include "edge_impl.hxx"
#include <vector>

namespace Proteus
{

  /*
	Edge Class Interface
  */
  // Only Public Constructor
  edge::edge(const std::initializer_list<size_t> &&l) : 
  	pimpl_{ new impl(std::move(l)) }
	{}

  edge::edge(const std::initializer_list<size_t> &l) : 
  	pimpl_{ new impl( l ) }
	{}

  // Destructor
  edge::~edge() {}

  // Copy Constructor
  edge::edge(const edge &other) :
  	pimpl_{new impl(*(other.pimpl_))} 
	{}

  // Assignment Operator
  edge & edge::operator=
  (const edge & rhs)
  {
	pimpl_->nl_ = rhs.pimpl_->nl_;
	return *this;
  }

  // Public Interface
  //-operator[]
  const size_t & edge::operator[]
  (const size_t & n) const
  {
	return pimpl_->nl_.at(n);
  }

  const size_t & edge::operator[]
  (const size_t && n) const
  {
	return pimpl_->nl_.at(std::move(n));
  }

}
