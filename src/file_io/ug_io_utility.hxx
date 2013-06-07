#ifndef __PROTEUS_UGIO_UTILITY__
#define __PROTEUS_UGIO_UTILITY__

#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/geometry.hxx>
#include <string>

void
create_entity_lists_from_file_ug_io
(std::string file_name,
 Proteus::Geometry::node_list &,
 Proteus::Geometry::tria_list &,
 Proteus::Geometry::quad_list &,
 Proteus::Geometry::surf_label &,
 Proteus::Geometry::tet_list &,
 Proteus::Geometry::pent5_list&,
 Proteus::Geometry::pent6_list&,
 Proteus::Geometry::hex_list &);

void
write_entity_lists_to_file_ug_io
(std::string file_name,
 Proteus::Geometry::node_list &,
 Proteus::Geometry::tria_list &,
 Proteus::Geometry::quad_list &,
 Proteus::Geometry::surf_label &,
 Proteus::Geometry::tet_list &,
 Proteus::Geometry::pent5_list&,
 Proteus::Geometry::pent6_list&,
 Proteus::Geometry::hex_list &);

#endif
