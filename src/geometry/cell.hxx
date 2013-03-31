#ifndef __PROTEUS_CELL_CLASS__
#define __PROTEUS_CELL_CLASS__

#include <memory>

namespace Proteus
{

  /**
	* \brief Cell Class Interface
	*/
  class
  cell
  {
	public:

	/** 
	 * \brief Copy Constructor
	 */
	  cell(const cell&);

	/**
	 * \brief Assignment Operator
	 */
	  cell & operator= (const cell&);

	/**
	 * \brief Move Constructor
	 */
	  cell(cell &&) = default;

	/** 
	 * \brief Destructor
	 */
	  ~cell();
	  
	/** 
	 * \brief Public Constructor
	 */
	  cell(const std::initializer_list<size_t> &&);
	  cell(const std::initializer_list<size_t> &);

	/**
	 * \brief operator[]
	 */
	  const size_t & operator[] (const size_t &) const;
	  const size_t & operator[] (const size_t &&) const;

	private:
	/**
	 * \brief Default Constructor is Deleted
	 */
	  cell() = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
