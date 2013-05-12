#ifndef __PROTEUS_ENTITY_TRAIT_CLASS__
#define __PROTEUS_ENTITY_TRAIT_CLASS__

#include <iostream>
#include <unordered_map>

namespace Proteus
{
  /** \brief Entity Trait
   *
  */
  template
  <
    typename trait,
	typename container = std::unordered_map<std::size_t,trait>
  >
  class entity_trait
  {
    private:
      container container_;
      bool set_all_flag;
      trait set_all_value;

    public:
	  /// Consructor
      entity_trait() {set_all_flag = false;}
      ~entity_trait() = default;

      /// Returns the number of traits stored in container
      auto size() const -> decltype(container_.size())
	  {
		return container_.size();
	  }
	  bool empty() const { return container_.empty(); }
	  /** \brief Check for trait value
       *
	   *  @param n trait index
	   *  @param value trait value
	   *  @return boolean
	  */
      bool is_equal(std::size_t n, trait value) const
      {
        if(set_all_flag == true) return (set_all_value == value);
        auto got = container_.find(n);
        if(got == container_.end()) return false;
        return (got->second == value);
      }
	  /** \brief Set value for trait at index
	   *
	   *  @param n trait index
	   *  @param value trait value
	  */
      void set_trait(std::size_t n, trait value)
      {
        set_all_flag = false;
        container_[n] = value;
      }
	  /** \brief Set all indices to value
	   *
	   * This function will alter the behavior of is_equal so that it will only
	   *  return true the "value" used in it is equal to the "set_all_value"
	   *
	   * @param value value for all traits
	  */
      void set_all(trait value) { set_all_flag = true; set_all_value = value;}
	  /** \brief Remove specialized behavior caused by set_all
	   *
	  */
      void unset_all() { set_all_flag = false; }
	  /** \brief Remove the trait entry for a given index
	   *
	  */
      void unset_trait(std::size_t n)
      {
        auto got = container_.find(n);
        if(got == container_.end()) return;
        set_all_flag = false;
        container_.erase(got);
      }
	  /** \brief Removes all traits from containter
	   *
	  */
      void unset_each()
      {
        set_all_flag = false;
        container_.clear();
      }

      auto begin() const -> decltype(container_.begin()) { return container_.begin(); }
      auto end() const -> decltype(container_.end()) { return container_.end(); }
  };

  /** \brief Trait Adapter
   *
   * The entity_iterator class expects the "range" to behave as if it were a
   *  sequential container, like an std::vector. Therefore, this class serves as
   *  an adapter for the trait class so that it can be used for iteration.
  */
  template
  <
    typename associative_container
  >
  class trait_adapter
  {
	class iterator_adapter {
      private:
        const associative_container & container_;
        decltype(container_.begin()) container_itr_;
        iterator_adapter() = delete;

		public:
         ~iterator_adapter() = default;
		iterator_adapter(const associative_container & ac, const decltype(container_.begin()) & iter) : container_(ac), container_itr_(iter) {}
		auto operator*() const -> decltype(container_itr_->first) { return container_itr_->first; }
		iterator_adapter & operator=(const iterator_adapter & rhs) {this->container_itr_ = rhs.container_itr_; return *this; }
		const iterator_adapter & operator++() { ++container_itr_; return *this; }
		const iterator_adapter & operator--() { --container_itr_; return *this; }
		bool operator==(const iterator_adapter &ia) const { return ia.container_itr_ == this->container_itr_;}
		bool operator!=(const iterator_adapter &ia) const { return ia.container_itr_ != this->container_itr_;}
	};

    private:
      const associative_container & container_;
	  trait_adapter() = delete;

	public:
	  ~trait_adapter() = default;
	  trait_adapter(const associative_container & ac) : container_(ac) {}
	  iterator_adapter begin() const { return iterator_adapter(container_,container_.begin()); }
	  iterator_adapter end() const { return iterator_adapter(container_,container_.end()); } 
	  bool empty() const { return container_.empty(); }
  };

}

#endif
