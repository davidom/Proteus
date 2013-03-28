#ifndef __PROTEUS_FACE_LIST_IMPL_CLASS__
#define __PROTEUS_FACE_LIST_IMPL_CLASS__

#include <vector>
#include <memory>

namespace Proteus
{
  class face;
  /**
   * \brief Node List Class Implementation
   */

  class face_list::impl
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
	  const face & add_face(const std::initializer_list<size_t> &&);
	  const face & add_face(const std::initializer_list<size_t> &);

	/**
	 * \brief Data Structure for Storing Nodes
	 */
	  std::vector<std::unique_ptr<face>> store_;

	private:
  };

}

#endif
