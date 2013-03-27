#ifndef __PROTEUS_NODE_LIST_CLASS__
#define __PROTEUS_NODE_LIST_CLASS__

#include <memory>

namespace Proteus
{
  class node;
  /**
    * \brief Node List Class Interface
	*/
  class
  node_list
  {
	public:
	/**
	 * \brief Public Constructor
	 */
	  node_list();
	  node_list(const size_t &);

	/**
	 * \brief Public Destructor
	 */
	  ~node_list();

	/**
	 * \brief Add Node to List
	 */
	  const node & add_node(const std::initializer_list<double> &&);
	  const node & add_node(const std::initializer_list<double> &);

	/**
	 * \brief Access Node from List
	 */
	  const node & operator[] (const size_t &) const;

	private:
	/**
	 * \brief Default Move Constructor is Deleted
	 */
	  node_list(const node_list&&) = delete;

	/**
	 * \brief Default Copy Constructor is Deleted
	 */
	  node_list(const node_list&) = delete;

	/**
	 * \brief Default Assignment Operator is Deleted
	 */
	  node_list & operator= (const node_list&) = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
