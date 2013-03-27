#ifndef __PROTEUS_NODE_LIST_IMPL_CLASS__
#define __PROTEUS_NODE_LIST_IMPL_CLASS__

#include <vector>
#include <memory>

namespace Proteus
{
  class node;
  /**
   * \brief Node List Class Implementation
   */

  class node_list::impl
  {
	public:
	/**
	 * \brief Default Constructor
	 */
	  impl(){}

	/**
	 * \brief Default Destructor
	 */
	  ~impl(){}

	/**
	 * \brief Add Node to "Store" Container
	 */
	  const node & add_node(const std::initializer_list<double> &&);
	  const node & add_node(const std::initializer_list<double> &);

	/**
	 * \brief Data Structure for Storing Nodes
	 */
	  std::vector<std::unique_ptr<node>> store_;

	private:
  };

}

#endif
