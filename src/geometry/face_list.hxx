#ifndef __PROTEUS_FACE_LIST_CLASS__
#define __PROTEUS_FACE_LIST_CLASS__

#include <memory>

namespace Proteus
{
  class face;
  /**
    * \brief Face List Class Interface
	*/
  class
  face_list
  {
	public:
	/**
	 * \brief Public Constructor
	 */
	  face_list();
	  face_list(const size_t &);

	/**
	 * \brief Public Destructor
	 */
	  ~face_list();

	/**
	 * \brief Add Face to List
	 */
	  const face & add_face(const std::initializer_list<size_t> &&);
	  const face & add_face(const std::initializer_list<size_t> &);

	/**
	 * \brief Access Face from List
	 */
	  const face & operator[] (const size_t &) const;

	private:
	/**
	 * \brief Default Move Constructor is Deleted
	 */
	  face_list(const face_list&&) = delete;

	/**
	 * \brief Default Copy Constructor is Deleted
	 */
	  face_list(const face_list&) = delete;

	/**
	 * \brief Default Assignment Operator is Deleted
	 */
	  face_list & operator= (const face_list&) = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
