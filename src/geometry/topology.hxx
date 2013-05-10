#ifndef __PROTEUS_TOPOLOGY_CLASS__
#define __PROTEUS_TOPOLOGY_CLASS__

#include <memory>
#include <vector>
#include <iostream>

namespace Proteus
{
  template
  <
    typename container=std::vector<std::vector<std::size_t>>
  >
  class topology
  {
    private:
	  container _container;

    public:
	  topology() = default;
	  auto size() -> decltype(_container.size()) { return _container.size(); }
	  auto size(std::size_t n) -> decltype(_container[n].size()) { return _container[n].size(); }
	  void resize(std::size_t n) { _container.resize(n); }
	  void resize(std::size_t i, std::size_t n) { _container[i].resize(n); }
	  void push(std::size_t i, std::size_t n) { _container[i].emplace_back(n); }
	  void pull(std::size_t i, std::size_t n) { remove(_container[i].begin(), _container[i].end(), n); }
      auto operator[](std::size_t n) const -> decltype(_container[n]) { return _container[n]; }
  };

}

#endif
