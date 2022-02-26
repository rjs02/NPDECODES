#ifndef UPWIND_QUADRATURE_H
#define UPWIND_QUADRATURE_H

/**
 * @file upwindquadrature.h
 * @brief NPDE homework template
 * @author Philippe Peter
 * @date June 2020
 * @copyright Developed at SAM, ETH Zurich
 */
#include <lf/base/base.h>
#include <lf/geometry/geometry.h>
#include <lf/mesh/mesh.h>
#include <lf/mesh/utils/utils.h>

#include <Eigen/Core>
#include <Eigen/LU>
#include <memory>
#include <vector>

namespace UpwindQuadrature {

// At any vertex a^j of a triangle, the vector field -v(a^j) may either point
// into the triangle, along an edge of the triangle or outside the triangle.
enum class Direction { INWARDS, OUTWARDS, ALONG_EDGE };

/**
 * @brief Computes for all corners a^j of a triangle the direction of the vector
 * field -v(a^j)
 * @param geo Geometry object describing the triangle
 * @param velocities values of the vector field v, evaluated at the corners of
 * the triangle described by geo, stored in a 2x3 matrix.
 * @return A vector containing for all 3 corners the corresponding direction of
 * -v(a^j)
 */
std::vector<Direction> opposite_velocity_directions(
    const lf::geometry::Geometry &geo, const Eigen::MatrixXd &velocities);

/**
 * @headerfile upwindquadrature.h
 * @brief Computes the local matrices for the convection term based on the
 * upwind quadrature scheme introced in the exercise.
 * @tparam FUNCTOR function that defines the vector valued velocity
 * coefficient v.
 */
template <typename FUNCTOR>
class UpwindConvectionElementMatrixProvider {
 public:
  /**
   * @brief
   * @param v functor for the velocity field
   * @param masses data structure storing the masses m(a^j) for all vertices of
   * the mesh.
   */
  explicit UpwindConvectionElementMatrixProvider(
      FUNCTOR v, lf::mesh::utils::CodimMeshDataSet<double> masses)
      : v_(v), masses_(masses) {}

  /**
   * @brief main routine for the computation of element matrices.
   * @param entity reference to the TRIANGULAR cell for which the element
   * matrix should be computed.
   * @return a 3x3 matrix containing the element matrix.
   */
  Eigen::Matrix3d Eval(const lf::mesh::Entity &entity);

  /** @brief Default implementation: all cells are active */
  bool isActive(const lf::mesh::Entity & /*entity*/) const { return true; }

 private:
  FUNCTOR v_;  // velocity field
  lf::mesh::utils::CodimMeshDataSet<double>
      masses_;  // masses of all vertices of the mesh.
};

template <typename FUNCTOR>
Eigen::Matrix3d UpwindConvectionElementMatrixProvider<FUNCTOR>::Eval(
    const lf::mesh::Entity &entity) {
  LF_ASSERT_MSG(lf::base::RefEl::kTria() == entity.RefEl(),
                "Function only defined for triangular cells");

  const lf::geometry::Geometry *geo_ptr = entity.Geometry();
  const Eigen::MatrixXd corners = lf::geometry::Corners(*geo_ptr);
  const double area = lf::geometry::Volume(*geo_ptr);
  Eigen::Matrix3d loc_mat;

  //====================
  // Your code goes here
  //====================

  return loc_mat;
}

/**
 * @brief Computes the masses m(p) of all vertices of the mesh
 * @param mesh_p pointer to the mesh.
 * @return data structure containing the masses m(p) for all vertices p of the
 * mesh represented by mesh_p.
 */
lf::mesh::utils::CodimMeshDataSet<double> initializeMasses(
    std::shared_ptr<const lf::mesh::Mesh> mesh_p);

}  // namespace UpwindQuadrature

#endif  // UPWIND_QUADRATURE_H
