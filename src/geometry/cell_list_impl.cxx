#include "cell.hxx"
#include "cell_list.hxx"
#include "cell_list_impl.hxx"

namespace Proteus
{

  /**
    * \brief Node List Class Implementation
	*/

  // Add Node to Storage Container
  const cell & cell_list::impl::add_cell
    (const std::initializer_list<size_t> && l)
  {
	store_.emplace_back(std::unique_ptr<cell>(new cell(std::move(l))));
	return (*(store_.back()));
  }

  const cell & cell_list::impl::add_cell
    (const std::initializer_list<size_t> & l)
  {
	store_.emplace_back(std::unique_ptr<cell>(new cell(l)));
	return (*(store_.back()));
  }
}
