/**
 * @ file boundarylength.cc
 * @ brief NPDE homework LengthOfBoundary code
 * @ author Christian Mitsch
 * @ date 03.03.2019
 * @ copyright Developed at ETH Zurich
 */

#include "boundarylength.h"

#include <lf/geometry/geometry.h>
#include <lf/io/io.h>
#include <lf/mesh/hybrid2d/hybrid2d.h>
#include <lf/mesh/utils/utils.h>

namespace LengthOfBoundary {

/* SAM_LISTING_BEGIN_1 */
double volumeOfDomain(const std::shared_ptr<lf::mesh::Mesh> mesh_p) {
  double volume = 0.0;
  //====================
  // Your code goes here
  //====================
  int codim = 0; // 0 = triangles (2D)
  for(const auto *cell : mesh_p->Entities(codim)) {
    volume += lf::geometry::Volume(*(cell->Geometry()));
  }
  return volume;
}
/* SAM_LISTING_END_1 */

/* SAM_LISTING_BEGIN_2 */
double lengthOfBoundary(const std::shared_ptr<lf::mesh::Mesh> mesh_p) {
  double length = 0.0;
  //====================
  // Your code goes here
  //====================

  auto flagList = lf::mesh::utils::flagEntitiesOnBoundary(mesh_p, 1); // flag edges on the boundary

  for(const auto *edge : mesh_p->Entities(1)) { // iterate over all edges
    if(flagList(*edge)) length += lf::geometry::Volume(*(edge->Geometry())); // add length (volume of edge) if edge is on boundary
  }

  return length;
}
/* SAM_LISTING_END_2 */

/* SAM_LISTING_BEGIN_3 */
std::pair<double, double> measureDomain(std::string filename) {
  double volume, length;

  //====================
  // Your code goes here
  //====================

  // from Code 2.7.1.12
  auto factory = std::make_unique<lf::mesh::hybrid2d::MeshFactory>(2);
  lf::io::GmshReader reader(std::move(factory), filename);
  std::shared_ptr<lf::mesh::Mesh> mesh_ptr = reader.mesh();

  volume = volumeOfDomain(mesh_ptr);
  length = lengthOfBoundary(mesh_ptr);

  return {volume, length};
}
/* SAM_LISTING_END_3 */

}  // namespace LengthOfBoundary
