#ifndef __PROTEUS_EDGE_CLASS__
#define __PROTEUS_EDGE_CLASS__

#include <memory>

namespace Proteus
{

  /**
	* \brief Edge Class Interface
	*/
  class
  edge
  {
	public:

	/** 
	 * \brief Copy Constructor
	 */
	  edge(const edge&);

	/**
	 * \brief Assignment Operator
	 */
	  edge & operator= (const edge&);

	/**
	 * \brief Move Constructor
	 */
	  edge(edge &&) = default;

	/** 
	 * \brief Destructor
	 */
	  ~edge();
	  
	/** 
	 * \brief Public Constructor
	 */
	  edge(const std::initializer_list<size_t> &&);
	  edge(const std::initializer_list<size_t> &);

	/**
	 * \brief operator[]
	 */
	  const size_t & operator[] (const size_t &) const;
	  const size_t & operator[] (const size_t &&) const;

	private:
	/**
	 * \brief Default Constructor is Deleted
	 */
	  edge() = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
