#include "node.hxx"
#include "node_impl.hxx"
#include <vector>

namespace Proteus
{

  /*
	Node Class Interface
  */
  // Only Public Constructor
  node::node(const std::initializer_list<double> &l) : 
  	pimpl_{ new impl(l) }
	{}

  // Destructor
  node::~node() {}

  // Copy Constructor
  node::node(const node &other) :
  	pimpl_{new impl(*(other.pimpl_))} 
	{}

  // Assignment Operator
  node & node::operator=
  (const node & rhs)
  {
	pimpl_->pos_ = rhs.pimpl_->pos_;
  }

  // Public Interface
  //-operator[]
  const double & node::operator[]
  (const size_t & n)
  {
	return pimpl_->pos_.at(n);
  }

}