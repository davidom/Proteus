#include "face.hxx"
#include "face_list.hxx"
#include "face_list_impl.hxx"

namespace Proteus
{

  /**
    * \brief Node List Class Implementation
	*/

  // Add Node to Storage Container
  const face & face_list::impl::add_face
    (const std::initializer_list<size_t> && l)
  {
	store_.emplace_back(std::unique_ptr<face>(new face(std::move(l))));
	return (*(store_.back()));
  }

  const face & face_list::impl::add_face
    (const std::initializer_list<size_t> & l)
  {
	store_.emplace_back(std::unique_ptr<face>(new face(l)));
	return (*(store_.back()));
  }
}
