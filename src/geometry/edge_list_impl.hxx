#ifndef __PROTEUS_EDGE_LIST_IMPL_CLASS__
#define __PROTEUS_EDGE_LIST_IMPL_CLASS__

#include <vector>
#include <memory>

namespace Proteus
{
  class edge;
  /**
   * \brief Node List Class Implementation
   */

  class edge_list::impl
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
	  const edge & add_edge(const std::initializer_list<size_t> &&);
	  const edge & add_edge(const std::initializer_list<size_t> &);

	/**
	 * \brief Data Structure for Storing Nodes
	 */
	  std::vector<std::unique_ptr<edge>> store_;

	private:
  };

}

#endif
