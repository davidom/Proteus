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
    typename list,
    typename range
  >
  class obverse_entity_iterator
  {
    private:
      obverse_entity_iterator() = delete;
      const list & list_;
      const range & range_;
      
      decltype(range_.begin()) range_itr_;
      std::size_t index_;

      void update_index()
      {
        ++index_;
        
        if(range_.empty()) return;

        while(range_itr_ != range_.end())
        {
          if(index_ < *range_itr_) break;
          else if(index_ > *range_itr_) ++range_itr_;
          else { ++index_; ++range_itr_; }
        }
      }
      void update_index_back()
      {
        --index_;
        
        if(range_.empty()) return;

        while(range_itr_ != range_.end())
        {
          if(index_ < *range_itr_) break;
          else if(index_ > *range_itr_) --range_itr_;
          else { --index_; --range_itr_; }
        }
      }
      void reset_to_first_valid()
      {
        index_ = 0; range_itr_ = range_.begin();
        
        if(range_.empty()) return;

        while(index_ >= *range_itr_ && range_itr_ != range_.end()) { ++index_; ++range_itr_; }
      }
      void reset_to_last_valid()
      {
        index_ = list_.size() - 1; range_itr_ = --range_.end();

        if(range_.empty()) return;

        while(index_ <= *range_itr_ && index_ > 0 ) { --index_; --range_itr_; }
      }

    public:
      obverse_entity_iterator(const list & l_, const range & r_)
        : list_(l_),
          range_(r_),
          range_itr_(range_.begin())
        {reset_to_first_valid();}
      
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
      const obverse_entity_iterator & operator++() { update_index(); return *this; }
      /// Pre-decrement operator
      const obverse_entity_iterator & operator--() { update_index_back(); return *this; }
      /// Resets the iterator to point to the beginning of the range-list
      void reset() { reset_to_first_valid(); }
      /// Resets the iterator to point to the end of the range-list
      void reset_end() { reset_to_last_valid(); }
      /// Returns iterator pointing at beginning of range-list
      const obverse_entity_iterator begin() const
      {
        return obverse_entity_iterator(list_,range_);
      }
      /// Returns iterator pointing at end of range-list
      const obverse_entity_iterator end() const
      {
        obverse_entity_iterator ei(list_,range_); ei.reset_end(); return ei;
      }
      /// Comparison Operator
      bool operator==(const obverse_entity_iterator &ei) const
      {
        return ei.index_ == this->index_;
      }
      /// Comparison Operator
      bool operator!=(const obverse_entity_iterator &ei) const
      {
        return ei.index_ != this->index_;
      }
      bool operator<=(const obverse_entity_iterator &ei) const
      {
        return this->index_ <= ei.index_;
      }
      /// Returns the index to which the iterator points to in the range
      auto index() -> decltype(index_) { return index_; } 

  };

  template
  <
    typename list
  >
  class all_range
  {
    private:
      const list & list_;
      std::size_t index_;
      all_range() = delete;

    public:
      ~all_range() = default;
      all_range(list l) : list_(l), index_(0) {}
      auto operator*() const -> decltype(index_) { return index_; }
      all_range begin() const { return all_range(static_cast<std::size_t>(0)); }
      all_range end() const { return all_range(static_cast<std::size_t>(list_.size())); }
      all_range & operator=(const all_range & rhs) { this->index_ = rhs.index_; return *this; }
      const all_range & operator++() { ++index_; return *this; }
      const all_range & operator--() { --index_; return *this; }
      bool operator!=(const all_range &ia) const { return ia.index_ != this->index_; }
  };
}

#endif
