/**
 * @file matode_main.cc
 * @brief NPDE homework MatODE code
 * @copyright Developed at ETH Zurich
 */

#include <Eigen/Dense>
#include <iomanip>
#include <iostream>
#include <vector>

#include "matode.h"

int main(int /*argc*/, char** /*argv*/) {
  /* SAM_LISTING_BEGIN_6 */
  double h = 0.01;  // stepsize
  //Eigen::Vector3d norms;
  // Build M
  Eigen::Matrix3d M;
  M << 8, 1, 6, 3, 5, 7, 9, 9, 2;
  // Build A
  Eigen::Matrix3d A;
  A << 0, 1, 1, -1, 0, 1, -1, -1, 0;
  Eigen::MatrixXd I = Eigen::Matrix3d::Identity();
  //====================
  // Your code goes here
  //====================
  
  using namespace MatODE;

  // y0 is Q in M = QR
  Eigen::MatrixXd Y01, Y02, Y03;
  Eigen::HouseholderQR<Eigen::MatrixXd> qr(M);
  Y01 = Y02 = Y03 = qr.householderQ();

  Eigen::MatrixXd norms = Eigen::MatrixXd::Zero(20, 3); // stores frobenius norms using 3 different methods
  // perform 20 steps using the implemented methods
  for(int i = 0; i < 20; ++i) {
    // explicit Euler step
    Eigen::MatrixXd Y1 = eeulstep(A, Y01, h);
    norms(i, 0) = (Y1.transpose()*Y1 - I).norm();
    Y01 = Y1;
    // implicit Euler step
    Y1 = ieulstep(A, Y02, h);
    norms(i, 1) = (Y1.transpose()*Y1 - I).norm();
    Y02 = Y1;
    // implicit Euler step
    Y1 = impstep(A, Y03, h);
    norms(i, 2) = (Y1.transpose()*Y1 - I).norm();
    Y03 = Y1;
  }

  std::cout << "Norms:" << std::endl;
  std::cout << norms << std::endl;
  /* SAM_LISTING_END_6 */
  return 0;
}
