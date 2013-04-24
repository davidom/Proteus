#ifndef __PROTEUS_ENTITY_TRAIT_CLASS__
#define __PROTEUS_ENTITY_TRAIT_CLASS__

#include <unordered_map>

namespace Proteus
{

  template
  <
    typename trait
  >
  class entity_trait
  {
    private:
      std::unordered_map<std::size_t, trait> container_;
      bool set_all_flag;
      trait set_all_value;

    public:
      entity_trait() {set_all_flag = false;}
      ~entity_trait() = default;

      std::size_t size() { return container_.size(); }
      bool is_equal(std::size_t n, trait value)
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
  };

}

#endif
