#ifndef __PROTEUS_ENTITY_LIST_CLASS__
#define __PROTEUS_ENTITY_LIST_CLASS__

#include <vector>

namespace Proteus
{
  /** \brief Entity List Class
   *
   * Entity List is to be a simple container for entities that offers only push
   *  operations and entity access via the [] operator.
  */

  template
  <
    typename T,
	typename container = std::vector<T>
  >
  class entity_list
  {
	private:
	  container container_;

	public:
	  /// Default Constructor for empty list
	  entity_list() = default;
	  /// Constructor for list with space reserved to avoid reallocation
	  entity_list(std::size_t n) { container_.reserve(n); }
	  ~entity_list() = default;
      
	  /// Push entity onto list
	  template<typename ...Args>
	  void push(Args&& ...args) { container_.emplace_back( std::forward<Args>(args)... ); }
	  /// Returns size of container
	  auto size() const -> decltype(container_.size()) { return container_.size(); }
	  /// Element access
	  auto operator[] (std::size_t n) const -> decltype(container_[n]) { return container_[n]; }
  };

}

#endif
