#ifndef __PROTEUS_NODE_CLASS__
#define __PROTEUS_NODE_CLASS__


#include <memory>

namespace Proteus
{

  /**
	* \brief Node Class Interface
	*/
  class
  node
  {

	public:

	/** 
	 * \brief Copy Constructor
	 */
	  node(const node&);

	/**
	 * \brief Assignment Operator
	 */
	  node & operator= (const node&);

	/** 
	 * \brief Destructor
	 */
	  ~node();
	  
	// Public Interface
	/** 
	 * \brief Public Constructor
	 */
	  node(const std::initializer_list<double> &);

	/**
	 * \brief operator[]
	 */
	  const double & operator[] (const size_t&);

	private:
	/**
	 * \brief Default Constructor is Deleted
	 */
	  node() = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
