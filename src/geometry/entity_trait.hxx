#ifndef __PROTEUS_ENTITY_TRAIT_CLASS__
#define __PROTEUS_ENTITY_TRAIT_CLASS__

#include <iostream>
#include <unordered_map>

namespace Proteus
{

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
      entity_trait() {set_all_flag = false;}
      ~entity_trait() = default;

      auto size() const -> decltype(container_.size())
	  {
		return container_.size();
	  }
      bool is_equal(std::size_t n, trait value) const
      {
        if(set_all_flag == true) return (set_all_value == value);
        auto got = container_.find(n);
        if(got == container_.end()) return false;
        return (got->second == value);
      }
      void set_trait(std::size_t n, trait value)
      {
        set_all_flag = false;
        container_[n] = value;
      }
      void set_all(trait value) { set_all_flag = true; set_all_value = value;}
      void unset_all() { set_all_flag = false; }
      void unset_trait(std::size_t n)
      {
        auto got = container_.find(n);
        if(got == container_.end()) return;
        set_all_flag = false;
        container_.erase(got);
      }
      void unset_each()
      {
        set_all_flag = false;
        container_.clear();
      }

      auto begin() const -> decltype(container_.begin()) { return container_.begin(); }
      auto end() const -> decltype(container_.end()) { return container_.end(); }
  };

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
  };
}

#endif
