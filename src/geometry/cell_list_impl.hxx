#ifndef __PROTEUS_CELL_LIST_IMPL_CLASS__
#define __PROTEUS_CELL_LIST_IMPL_CLASS__

#include <vector>
#include <memory>

namespace Proteus
{
  class cell;
  /**
   * \brief Node List Class Implementation
   */

  class cell_list::impl
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
	  const cell & add_cell(const std::initializer_list<size_t> &&);
	  const cell & add_cell(const std::initializer_list<size_t> &);

	/**
	 * \brief Data Structure for Storing Nodes
	 */
	  std::vector<std::unique_ptr<cell>> store_;

	private:
  };

}

#endif
