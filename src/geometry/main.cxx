#include "node.hxx"
#include "node_list.hxx"
#include "face.hxx"
#include "face_list.hxx"
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
  std::string ofile_name = "outputfile.surf";
  std::ifstream file;
  std::ofstream ofile;
  file.open(argv[1],std::ios::in);
  ofile.open(ofile_name,std::ios::out);

  std::getline(file, file_line);
  std::istringstream ins_(file_line);
  ins_ >> ntria >> nquad >> nnode;
  ofile << ntria <<" "<<nquad<<" "<<nnode<<std::endl;
  std::cout <<"Reading "<<nnode<<" Nodes from file...\n";

  node_list nodes;
  double x, y ,z;

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
	ofile <<x<<" "<<y<<" "<<z<<" 0 0\n";
    end = std::chrono::system_clock::now();
	creation += end - start;
  }
  end = std::chrono::system_clock::now();

  total_time = end-start_t;
  std::cout <<"Node Reading: "<<nnode<<std::endl;
  std::cout <<"total elapsed time: "<<total_time.count()<<std::endl;
  std::cout <<"elapsed time for file read: "<<file_read.count()<<std::endl;
  std::cout <<"elapsed time for creation: "<<creation.count()<<std::endl;

  file_read = end - end;
  total_time = end - end;
  creation = end - end;

  size_t n1, n2, n3;
  face_list faces;

  start_t = std::chrono::system_clock::now();
  for(size_t i=0; i<ntria; ++i) {
    start = std::chrono::system_clock::now();
    std::getline(file,file_line);
	std::istringstream ins(file_line);
	ins >> n1 >> n2 >> n3;
    end = std::chrono::system_clock::now();
	file_read += end - start;
    
	start = std::chrono::system_clock::now();
	faces.add_face({n1,n2,n3});
	n1 = faces[i][0]; n2=faces[i][1]; n3=faces[i][2];
	ofile <<n1<<" "<<n2<<" "<<n3<<" 1 0 1\n";	
    end = std::chrono::system_clock::now();
	creation += end - start;
  }

  end = std::chrono::system_clock::now();

  total_time = end-start_t;
  std::cout <<"Tria Reading: "<<ntria<<std::endl;
  std::cout <<"total elapsed time: "<<total_time.count()<<std::endl;
  std::cout <<"elapsed time for file read: "<<file_read.count()<<std::endl;
  std::cout <<"elapsed time for creation: "<<creation.count()<<std::endl;
  file.close();

  return 0;
}
