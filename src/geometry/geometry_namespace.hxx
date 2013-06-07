#ifndef __PROTEUS_GEOMETRY_NAMESPACE__
#define __PROTEUS_GEOMETRY_NAMESPACE__

#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/topology.hxx>

#include <array>
#include <vector>

namespace Proteus
{
  /** \brief Geometry Namespace
   *
   * This namespace holds some common typdefs for used in creating geometry
  */
  namespace Geometry
  {
    typedef entity<std::array<double,2>> node2d;
    typedef entity<std::array<double,3>> node3d;
    typedef entity<std::vector<double>> node;

    typedef entity<std::array<size_t,2>> edge;
    typedef entity<std::vector<size_t>> curve;

    typedef entity<std::array<size_t,3>> tria;
    typedef entity<std::array<size_t,4>> quad;
    typedef entity<std::vector<size_t>> face;

    typedef entity<std::array<size_t,4>> tet;
    typedef entity<std::array<size_t,5>> pent5;
    typedef entity<std::array<size_t,6>> pent6;
    typedef entity<std::array<size_t,8>> hex;
    typedef entity<std::vector<size_t>> cell;

    typedef entity_list<node2d> node2d_list;
    typedef entity_list<node3d> node3d_list;
    typedef entity_list<node> node_list;

    typedef entity_list<edge> edge_list;
    typedef entity_list<curve> curve_list;

    typedef entity_list<tria> tria_list;
    typedef entity_list<quad> quad_list;
    typedef entity_list<face> face_list;

    typedef entity_list<tet> tet_list;
    typedef entity_list<pent5> pent5_list;
    typedef entity_list<pent6> pent6_list;
    typedef entity_list<hex> hex_list;
    typedef entity_list<cell> cell_list;

    typedef topology<std::vector<std::array<size_t,6>>> hex_topo_hex;
    typedef entity_list<size_t> surf_label;
  }

}

#endif
