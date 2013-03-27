#include "node.hxx"
#include "node_list.hxx"
#include "node_list_impl.hxx"

namespace Proteus
{

  /**
    * Node List Class Interface
    */

	// Public Constructor
	node_list::node_list() : pimpl_{ new impl } {}

	// Destructor
	node_list::~node_list() {}

	// Add Node with rvalue_reference
	const node & node_list::add_node(const std::initializer_list<double> &&l)
	{
	  pimpl_->add_node(std::move(l));
	  return (*(pimpl_->store_.back()));
	}

	// Add Node with lvalue_reference
	const node & node_list::add_node(const std::initializer_list<double> &l)
	{
	  pimpl_->add_node(l);
	  return (*(pimpl_->store_.back()));
	}

	// Access to Nodes stored
	const node & node_list::operator[] (const size_t & i) const
	{
	  return (*(pimpl_->store_.at(i)));
	}
}
