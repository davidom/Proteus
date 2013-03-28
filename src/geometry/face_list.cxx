#include "face.hxx"
#include "face_list.hxx"
#include "face_list_impl.hxx"

namespace Proteus
{

  /**
    * Face List Class Interface
    */

	// Public Constructor
	face_list::face_list() : pimpl_{ new impl } {}

	// Destructor
	face_list::~face_list() {}

	// Add Face with rvalue_reference
	const face & face_list::add_face(const std::initializer_list<size_t> &&l)
	{
	  pimpl_->add_face(std::move(l));
	  return (*(pimpl_->store_.back()));
	}

	// Add Face with lvalue_reference
	const face & face_list::add_face(const std::initializer_list<size_t> &l)
	{
	  pimpl_->add_face(l);
	  return (*(pimpl_->store_.back()));
	}

	// Access to Faces stored
	const face & face_list::operator[] (const size_t & i) const
	{
	  return (*(pimpl_->store_.at(i)));
	}
}
