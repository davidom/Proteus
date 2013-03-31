#ifndef __PROTEUS_CELL_LIST_CLASS__
#define __PROTEUS_CELL_LIST_CLASS__

#include <memory>

namespace Proteus
{
  class cell;
  /**
    * \brief Cell List Class Interface
	*/
  class
  cell_list
  {
	public:
	/**
	 * \brief Public Constructor
	 */
	  cell_list();
	  cell_list(const size_t &);

	/**
	 * \brief Public Destructor
	 */
	  ~cell_list();

	/**
	 * \brief Add Cell to List
	 */
	  const cell & add_cell(const std::initializer_list<size_t> &&);
	  const cell & add_cell(const std::initializer_list<size_t> &);

	/**
	 * \brief Access Cell from List
	 */
	  const cell & operator[] (const size_t &) const;

	private:
	/**
	 * \brief Default Move Constructor is Deleted
	 */
	  cell_list(const cell_list&&) = delete;

	/**
	 * \brief Default Copy Constructor is Deleted
	 */
	  cell_list(const cell_list&) = delete;

	/**
	 * \brief Default Assignment Operator is Deleted
	 */
	  cell_list & operator= (const cell_list&) = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
