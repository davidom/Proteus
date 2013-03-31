#include "cell.hxx"
#include "cell_list.hxx"
#include "cell_list_impl.hxx"

namespace Proteus
{

  /**
    * Cell List Class Interface
    */

	// Public Constructor
	cell_list::cell_list() : pimpl_{ new impl } {}

	// Destructor
	cell_list::~cell_list() {}

	// Add Cell with rvalue_reference
	const cell & cell_list::add_cell(const std::initializer_list<size_t> &&l)
	{
	  pimpl_->add_cell(std::move(l));
	  return (*(pimpl_->store_.back()));
	}

	// Add Cell with lvalue_reference
	const cell & cell_list::add_cell(const std::initializer_list<size_t> &l)
	{
	  pimpl_->add_cell(l);
	  return (*(pimpl_->store_.back()));
	}

	// Access to Cells stored
	const cell & cell_list::operator[] (const size_t & i) const
	{
	  return (*(pimpl_->store_.at(i)));
	}
}
