#ifndef __PROTEUS_ENTITY_ITERATOR_CLASS__
#define __PROTEUS_ENTITY_ITERATOR_CLASS__

namespace Proteus
{
  
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
	  entity_iterator(const list & l_, const range & r_)
	    : list_(l_),
		  range_(r_),
		  range_itr_(range_.begin())
		{}

	  auto operator*() const -> decltype(list_[*range_itr_])
	  {
		return list_[*range_itr_];
	  }
	  const entity_iterator & operator++() { ++range_itr_; return *this; }
	  const entity_iterator & operator--() { --range_itr_; return *this; }
	  void reset() { range_itr_ = range_.begin(); }
	  void reset_end() { range_itr_ = range_.end(); }
      const entity_iterator begin() const
	  {
		return entity_iterator(list_,range_);
	  }
      const entity_iterator end() const
	  {
		entity_iterator ei(list_,range_); ei.reset_end(); return ei;
	  }
	  bool operator==(const entity_iterator &ei) const
	  {
        return ei.range_itr_ == this->range_itr_;
	  }
	  bool operator!=(const entity_iterator &ei) const
	  {
        return ei.range_itr_ != this->range_itr_;
	  }
	  auto index() -> decltype(*range_itr_) { return *range_itr_; } 
  };

}

#endif
