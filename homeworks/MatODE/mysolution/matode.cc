/**
 * @file matode.cc
 * @brief NPDE homework MatODE code
 * @copyright Developed at ETH Zurich
 */

#include <Eigen/Dense>

namespace MatODE {

/* SAM_LISTING_BEGIN_3 */
Eigen::MatrixXd eeulstep(const Eigen::MatrixXd& A, const Eigen::MatrixXd& Y0,
                         double h) {
  //====================
  // Your code goes here

  // explicit Euler step
  return Y0 + h * A * Y0;

  //====================
  //return Y0;
}
/* SAM_LISTING_END_3 */

/* SAM_LISTING_BEGIN_4 */
Eigen::MatrixXd ieulstep(const Eigen::MatrixXd& A, const Eigen::MatrixXd& Y0,
                         double h) {
  //====================
  // Your code goes here

  // implicit Euler step
  
  return (Eigen::MatrixXd::Identity(Y0.rows(), Y0.cols()) - h*A).lu().solve(Y0);

  //====================
  //return Y0;
}
/* SAM_LISTING_END_4 */

/* SAM_LISTING_BEGIN_5 */
Eigen::MatrixXd impstep(const Eigen::MatrixXd& A, const Eigen::MatrixXd& Y0,
                        double h) {
  //====================
  // Your code goes here

  return (Eigen::MatrixXd::Identity(Y0.rows(), Y0.cols()) - h/2. * A).lu().solve(Y0 + h/2. * A * Y0);

  //====================
  //return Y0;
}
/* SAM_LISTING_END_5 */

}  // namespace MatODE
