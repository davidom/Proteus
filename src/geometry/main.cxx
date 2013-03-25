#include "node.hxx"
#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

using namespace Proteus;

int
main(int argc, char *argv[])
{
  if(argc < 2) {
	std::cout <<"Not enough arguments...\n";
	return 1;
  }

  size_t ntria, nquad, nnode;
  std::string file_line;
  std::ifstream file;
  file.open(argv[1]);

  file >> ntria >> nquad >> nnode;

  std::cout <<"Reading "<<nnode<<" Nodes from file...\n";

  std::vector<node> nodes;
  nodes.reserve(nnode);
  double x, y ,z;

  std::chrono::duration<float> file_read, creation, total_time;
  std::chrono::time_point<std::chrono::system_clock> start, end, start_t;

  start_t = std::chrono::system_clock::now();
  for(int i=0; i<nnode; ++i) {
    start = std::chrono::system_clock::now();
    std::getline(file,file_line);
	std::stringstream ins(file_line);
	ins >> x >> y >> z;
    end = std::chrono::system_clock::now();
	file_read += end - start;

    start = std::chrono::system_clock::now();
	nodes.emplace_back(node({x,y,z}));
    end = std::chrono::system_clock::now();
	creation += end - start;
  }
  end = std::chrono::system_clock::now();

  total_time = end-start_t;
  std::cout <<"total elapsed time: "<<total_time.count()<<std::endl;
  std::cout <<"elapsed time for file read: "<<file_read.count()<<std::endl;
  std::cout <<"elapsed time for creation: "<<creation.count()<<std::endl;

  file.close();

  return 0;
}
