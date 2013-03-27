#include "node.hxx"
#include "node_list.hxx"
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>

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
  file.open(argv[1],std::ios::in);

  std::getline(file, file_line);
  std::istringstream ins_(file_line);
  ins_ >> ntria >> nquad >> nnode;

  std::cout <<"Reading "<<nnode<<" Nodes from file...\n";

  node_list nodes;
  double x, y ,z;
  int l=0, m=1, n=2;

  std::chrono::duration<float> file_read, creation, total_time;
  std::chrono::time_point<std::chrono::system_clock> start, end, start_t;

  start_t = std::chrono::system_clock::now();
  for(size_t i=0; i<nnode; ++i) {
    start = std::chrono::system_clock::now();
    std::getline(file,file_line);
	std::istringstream ins(file_line);
	ins >> x >> y >> z;
    end = std::chrono::system_clock::now();
	file_read += end - start;
    
	start = std::chrono::system_clock::now();
	nodes.add_node({x,y,z});
	x = nodes[i][0]; y=nodes[i][1]; z=nodes[i][2];
	if(i < 10) std::cout <<x<<","<<y<<","<<z<<std::endl;
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
