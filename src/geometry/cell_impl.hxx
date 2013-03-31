#ifndef __PROTEUS_CELL_IMPL_CLASS__
#define __PROTEUS_CELL_IMPL_CLASS__

#include <vector>

namespace Proteus
{
  /**
   * \brief Cell Class Implementation
   */
  class cell::impl
  {
	 public:
	 /** 
	  * \brief Public Constructor
	  */
	   impl(const std::initializer_list<size_t> &&l);
	   impl(const std::initializer_list<size_t> &l);

	 /**
	  * \brief Copy Constructor
	  */
	   impl(const impl &);

	/**
	 * \brief Move Contructor
	 */
	   impl(impl &&) = default;

	 /**
	  * \brief Destructor
	  */
	   ~impl();

	/**
	 * \brief Holder for Node List
	 */
	   std::vector<size_t> def_;

	private:
	/**
	 * \brief Default Constructor is Deleted
	 */
	   impl() = delete;
  };

}

#endif
