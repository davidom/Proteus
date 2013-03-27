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
	 * \brief Move Constructor
	 */
	  node(node &&) = default;

	/** 
	 * \brief Destructor
	 */
	  ~node();
	  
	/** 
	 * \brief Public Constructor
	 */
	  node(const std::initializer_list<double> &&);
	  node(const std::initializer_list<double> &);

	/**
	 * \brief operator[]
	 */
	  const double & operator[] (const size_t &) const;
	  const double & operator[] (const size_t &&) const;

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
