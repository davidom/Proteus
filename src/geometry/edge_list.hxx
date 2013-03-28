#ifndef __PROTEUS_EDGE_LIST_CLASS__
#define __PROTEUS_EDGE_LIST_CLASS__

#include <memory>

namespace Proteus
{
  class edge;
  /**
    * \brief Edge List Class Interface
	*/
  class
  edge_list
  {
	public:
	/**
	 * \brief Public Constructor
	 */
	  edge_list();
	  edge_list(const size_t &);

	/**
	 * \brief Public Destructor
	 */
	  ~edge_list();

	/**
	 * \brief Add Edge to List
	 */
	  const edge & add_edge(const std::initializer_list<size_t> &&);
	  const edge & add_edge(const std::initializer_list<size_t> &);

	/**
	 * \brief Access Edge from List
	 */
	  const edge & operator[] (const size_t &) const;

	private:
	/**
	 * \brief Default Move Constructor is Deleted
	 */
	  edge_list(const edge_list&&) = delete;

	/**
	 * \brief Default Copy Constructor is Deleted
	 */
	  edge_list(const edge_list&) = delete;

	/**
	 * \brief Default Assignment Operator is Deleted
	 */
	  edge_list & operator= (const edge_list&) = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
