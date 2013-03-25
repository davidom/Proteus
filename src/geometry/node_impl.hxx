#ifndef __PROTEUS_NODE_IMPL_CLASS__
#define __PROTEUS_NODE_IMPL_CLASS__

#include <vector>

namespace Proteus
{

  /**
   * \brief Node Class Implementation
   */
  class node::impl
  {
	 public:
	 /** 
	  * \brief Public Constructor
	  */
	   impl(const std::initializer_list<double> &l);

	 /**
	  * \brief Copy Constructor
	  */
	   impl(const impl &);

	 /**
	  * \brief Destructor
	  */
	   ~impl();

	/**
	 * \brief Holder for Node position
	 */
	   std::vector<double> pos_;

	private:
	/**
	 * \brief Default Constructor is Deleted
	 */
	   impl() = delete;
  };

}

#endif
