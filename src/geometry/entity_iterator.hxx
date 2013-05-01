#ifndef __PROTEUS_ENTITY_ITERATOR_CLASS__
#define __PROTEUS_ENTITY_ITERATOR_CLASS__

namespace Proteus
{
  /** \brief Entity Iterator
   * 
   * Entity Iterator is to be used to iterate over a "range" of entities in the
   *  "list" specified by the indices in the "range". As implemented the "range"
   *  is used as would a std::vector--that is a sequential container.
  */

  template
  <
    typename list,
	typename range 
  >
  class entity_iterator
  {
    private:
	  entity_iterator() = delete;
	  const list & list_;
	  const range & range_;

	  decltype(range_.begin()) range_itr_;

	public:
	  /** \brief Constructor
	   *
	   * @param l_ random-access container 
	   * @param r_ sequential-access container
	  */
	  entity_iterator(const list & l_, const range & r_)
	    : list_(l_),
		  range_(r_),
		  range_itr_(range_.begin())
		{}

      /// \brief Dereference Operator
	  auto operator*() const -> decltype(list_[*range_itr_])
	  {
		return list_[*range_itr_];
	  }
	  /// Pre-increment operator
	  const entity_iterator & operator++() { ++range_itr_; return *this; }
	  /// Pre-decrement operator
	  const entity_iterator & operator--() { --range_itr_; return *this; }
	  /// Resets the iterator to point to the beginning of the range-list
	  void reset() { range_itr_ = range_.begin(); }
	  /// Resets the iterator to point to the end of the range-list
	  void reset_end() { range_itr_ = range_.end(); }
	  /// Returns iterator pointing at beginning of range-list
      const entity_iterator begin() const
	  {
		return entity_iterator(list_,range_);
	  }
	  /// Returns iterator pointing at end of range-list
      const entity_iterator end() const
	  {
		entity_iterator ei(list_,range_); ei.reset_end(); return ei;
	  }
	  /// Comparison Operator
	  bool operator==(const entity_iterator &ei) const
	  {
        return ei.range_itr_ == this->range_itr_;
	  }
	  /// Comparison Operator
	  bool operator!=(const entity_iterator &ei) const
	  {
        return ei.range_itr_ != this->range_itr_;
	  }
	  /// Returns the index to which the iterator points to in the range
	  auto index() -> decltype(*range_itr_) { return *range_itr_; } 
  };

}

#endif
