#ifndef __PROTEUS_ENTITY_ITERATOR_CLASS__
#define __PROTEUS_ENTITY_ITERATOR_CLASS__
#include <iostream>
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
	  auto operator->() const -> decltype(&(list_[*range_itr_]))
	  {
		return &(list_[*range_itr_]);
	  }
	  auto operator*() const -> decltype(list_[*range_itr_])
	  {
		return list_[*range_itr_];
	  }
	  auto operator[](std::size_t n) const -> decltype(list_[*range_itr_][n])
	  {
		return list_[*range_itr_][n];
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

  template
  <
    typename list
  >
  class all_range
  {
	private:
	  const list & list_;
	  all_range() = delete;

    class index_adapter {
	  private:
	    std::size_t index_;
		index_adapter() = delete;

	  public:
		~index_adapter() = default;
		index_adapter(const std::size_t index) : index_(index) {}
		auto operator*() const -> decltype(index_) { return index_; }
		index_adapter & operator=(const index_adapter & rhs) { this->index_ = rhs.index_; return *this; }
		const index_adapter & operator++() { ++index_; return *this; }
		const index_adapter & operator--() { --index_; return *this; }
		bool operator!=(const index_adapter &ia) const { return ia.index_ != this->index_; }
	};

	public:
	  ~all_range() = default;
	  all_range(list l) : list_(l) {}
	  index_adapter begin() const { return index_adapter(static_cast<std::size_t>(0)); }
	  index_adapter end() const { return index_adapter(static_cast<std::size_t>(list_.size())); }
  };
}

#endif
