#include "node.hxx"
#include "node_list.hxx"
#include "node_list_impl.hxx"

namespace Proteus
{

  /**
    * \brief Node List Class Implementation
	*/

  // Add Node to Storage Container
  const node & node_list::impl::add_node
    (const std::initializer_list<double> && l)
  {
	store_.emplace_back(std::unique_ptr<node>(new node(std::move(l))));
	return (*(store_.back()));
  }

  const node & node_list::impl::add_node
    (const std::initializer_list<double> & l)
  {
	store_.emplace_back(std::unique_ptr<node>(new node(l)));
	return (*(store_.back()));
  }
}
