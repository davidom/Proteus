#include "edge.hxx"
#include "edge_list.hxx"
#include "edge_list_impl.hxx"

namespace Proteus
{

  /**
    * Edge List Class Interface
    */

	// Public Constructor
	edge_list::edge_list() : pimpl_{ new impl } {}

	// Destructor
	edge_list::~edge_list() {}

	// Add Edge with rvalue_reference
	const edge & edge_list::add_edge(const std::initializer_list<size_t> &&l)
	{
	  pimpl_->add_edge(std::move(l));
	  return (*(pimpl_->store_.back()));
	}

	// Add Edge with lvalue_reference
	const edge & edge_list::add_edge(const std::initializer_list<size_t> &l)
	{
	  pimpl_->add_edge(l);
	  return (*(pimpl_->store_.back()));
	}

	// Access to Edges stored
	const edge & edge_list::operator[] (const size_t & i) const
	{
	  return (*(pimpl_->store_.at(i)));
	}
}
