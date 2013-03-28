#ifndef __PROTEUS_FACE_CLASS__
#define __PROTEUS_FACE_CLASS__

#include <memory>

namespace Proteus
{

  /**
	* \brief Face Class Interface
	*/
  class
  face
  {
	public:

	/** 
	 * \brief Copy Constructor
	 */
	  face(const face&);

	/**
	 * \brief Assignment Operator
	 */
	  face & operator= (const face&);

	/**
	 * \brief Move Constructor
	 */
	  face(face &&) = default;

	/** 
	 * \brief Destructor
	 */
	  ~face();
	  
	/** 
	 * \brief Public Constructor
	 */
	  face(const std::initializer_list<size_t> &&);
	  face(const std::initializer_list<size_t> &);

	/**
	 * \brief operator[]
	 */
	  const size_t & operator[] (const size_t &) const;
	  const size_t & operator[] (const size_t &&) const;

	private:
	/**
	 * \brief Default Constructor is Deleted
	 */
	  face() = delete;

	/**
	 * \brief Private Implementation of Public Interface
	 */
	  class impl;
	  std::unique_ptr<impl> pimpl_;
  };

}

#endif
