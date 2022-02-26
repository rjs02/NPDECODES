/**
 * @file nonlinschroedingerequation_main.cc
 * @brief NPDE homework NonLinSchroedingerEquation code
 * @author Oliver Rietmann
 * @date 22.04.2020
 * @copyright Developed at ETH Zurich
 */

#include <lf/assemble/assemble.h>
#include <lf/fe/fe.h>
#include <lf/io/io.h>
#include <lf/mesh/hybrid2d/hybrid2d.h>
#include <lf/uscalfe/uscalfe.h>

#include <Eigen/Core>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include "nonlinschroedingerequation.h"
#include "propagator.h"

int main() {
  /* SAM_LISTING_BEGIN_9 */
  // Load mesh and initalize FE space and DOF handler
  auto mesh_factory = std::make_unique<lf::mesh::hybrid2d::MeshFactory>(2);
  const lf::io::GmshReader reader(
      std::move(mesh_factory), CURRENT_SOURCE_DIR "/../meshes/square_64.msh");
  auto mesh_p = reader.mesh();
  auto fe_space =
      std::make_shared<lf::uscalfe::FeSpaceLagrangeO1<double>>(mesh_p);
  const lf::assemble::DofHandler &dofh{fe_space->LocGlobMap()};
  const lf::uscalfe::size_type N_dofs(dofh.NumDofs());

  // Mass matrix
  lf::assemble::COOMatrix<double> D_COO(N_dofs, N_dofs);
  NonLinSchroedingerEquation::MassElementMatrixProvider mass_emp;
  lf::assemble::AssembleMatrixLocally(0, dofh, dofh, mass_emp, D_COO);
  Eigen::SparseMatrix<double> D = D_COO.makeSparse();
  Eigen::SparseMatrix<std::complex<double>> M = std::complex<double>(0, 1) * D;

  // Stiffness matrix
  lf::assemble::COOMatrix<double> A_COO(N_dofs, N_dofs);
  lf::uscalfe::LinearFELaplaceElementMatrix stiffness_emp;
  lf::assemble::AssembleMatrixLocally(0, dofh, dofh, stiffness_emp, A_COO);
  Eigen::SparseMatrix<double> A = A_COO.makeSparse();

  // Prepare timestepping
  int timesteps = 100;
  double T = 1.0;
  double tau = T / timesteps;

// Prepare inital data
  //====================
  // Your code here
  // Initialize the vector mu with the basis expansion coefficients
  // of an interpolant of the function $u_0$.
  //====================
  Eigen::VectorXcd mu(N_dofs);

  // Prepare split-step propagator for full step $\tau$
  NonLinSchroedingerEquation::SplitStepPropagator splitStepPropagator(A, M,
                                                                      tau);

  // Arrays for storing "energies" contributing to the Hamiltonian
  Eigen::VectorXd norm(timesteps + 1);
  Eigen::VectorXd E_kin(timesteps + 1);
  Eigen::VectorXd E_int(timesteps + 1);
  //====================
  // Your code goes here
  // Implement timestepping based on Strang splitting
  // Record contributions to the Hamiltonian
  //====================

  // Timegrid
  Eigen::VectorXd t = Eigen::VectorXd::LinSpaced(timesteps + 1, 0.0, T);

  // Nice output format
  const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision,
                                         Eigen::DontAlignCols, ", ", "\n");

  // Write norm to file
  std::ofstream norm_csv;
  norm_csv.open("norm.csv");
  norm_csv << t.transpose().format(CSVFormat) << std::endl;
  norm_csv << norm.transpose().format(CSVFormat) << std::endl;
  norm_csv.close();
  /* SAM_LISTING_END_9 */

  // Call python script to plot norm
  std::cout << "Generated " CURRENT_BINARY_DIR "/norm.csv" << std::endl;
  std::system("python3 " CURRENT_SOURCE_DIR "/plot_norm.py " CURRENT_BINARY_DIR
              "/norm.csv " CURRENT_BINARY_DIR "/norm.eps");

  // Write energies to file
  std::ofstream energies_csv;
  energies_csv.open("energies.csv");
  energies_csv << t.transpose().format(CSVFormat) << std::endl;
  energies_csv << E_kin.transpose().format(CSVFormat) << std::endl;
  energies_csv << E_int.transpose().format(CSVFormat) << std::endl;
  energies_csv.close();

  // Call python script to plot energies
  std::cout << "Generated " CURRENT_BINARY_DIR "/energies.csv" << std::endl;
  std::system("python3 " CURRENT_SOURCE_DIR
              "/plot_energies.py " CURRENT_BINARY_DIR
              "/energies.csv " CURRENT_BINARY_DIR "/energies.eps");

  // Write entry-wise squared modulus of $\mu$ to .vtk file
  std::cout << "Generated " CURRENT_BINARY_DIR "/solution.vtk" << std::endl;
  lf::io::VtkWriter vtk_writer(mesh_p, "solution.vtk");
  Eigen::VectorXd mu_abs2 = mu.cwiseAbs2();
  lf::fe::MeshFunctionFE mu_abs2_mf(fe_space, mu_abs2);
  vtk_writer.WritePointData("mu_abs2", mu_abs2_mf);

  return 0;
}
