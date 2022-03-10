/**
 * @file
 * @brief NPDE homework TransformationOfGalerkinMatrices code
 * @author Erick Schulz
 * @date 01/03/2019
 * @copyright Developed at ETH Zurich
 */

#include "transformationofgalerkinmatrices.h"

#include <cassert>

namespace TransformationOfGalerkinMatrices {

/* SAM_LISTING_BEGIN_1 */
std::vector<Eigen::Triplet<double>> transformCOOmatrix(
    const std::vector<Eigen::Triplet<double>> &A) {
  std::vector<Eigen::Triplet<double>> A_t{};  // return value

  // First step: find the size of the matrix by searching the maximal
  // indices. Depends on the assumption that no zero rows/columns occur.
  int rows_max_idx = 0, cols_max_idx = 0;
  for (const Eigen::Triplet<double> &triplet : A) {
    rows_max_idx =
        (triplet.row() > rows_max_idx) ? triplet.row() : rows_max_idx;
    cols_max_idx =
        (triplet.col() > cols_max_idx) ? triplet.col() : cols_max_idx;
  }
  int n_rows = rows_max_idx + 1;
  int n_cols = cols_max_idx + 1;

  // Make sure we deal with a square matrix
  assert(n_rows == n_cols);
  // The matrix size must have even parity
  assert(n_cols % 2 == 0);

  int N = n_cols;      // Size of (square) matrix
  int M = n_cols / 2;  // Half the size
  //====================
  // Your code goes here
  //====================

  // loop over all triplets
  for(auto &triplet : A) {
    int k = triplet.row()+1, l = triplet.col()+1; // add 1 to get math indexing
    bool k_even = k % 2 == 0; // row index even?
    bool l_even = l % 2 == 0; // col index even?

    // case distinction, see solutions to 2-2.d); subtract 1 to get C++ indexing
    if(k_even && l_even) {
      A_t.emplace_back(Eigen::Triplet<double>(k/2-1,   l/2-1,    triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>(k/2+M-1, l/2+M-1,  triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>(k/2+M-1, l/2-1,   -triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>(k/2-1,   l/2+M-1, -triplet.value()));
    }
    else if(!k_even && !l_even) {
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2-1,   (l+1)/2-1,   triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2+M-1, (l+1)/2+M-1, triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2-1,   (l+1)/2+M-1, triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2+M-1, (l+1)/2-1,   triplet.value()));
    }
    else if(k_even && !l_even) {
      A_t.emplace_back(Eigen::Triplet<double>(k/2-1,   (l+1)/2-1,    triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>(k/2-1,   (l+1)/2+M-1,  triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>(k/2+M-1, (l+1)/2+M-1, -triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>(k/2+M-1, (l+1)/2-1,   -triplet.value()));
    }
    else if(!k_even && l_even) {
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2-1,   l/2-1,    triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2+M-1, l/2+M-1, -triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2+M-1, l/2-1,    triplet.value()));
      A_t.emplace_back(Eigen::Triplet<double>((k+1)/2-1,   l/2+M-1, -triplet.value()));
    }
  }
  
  return A_t;
}
/* SAM_LISTING_END_1 */

}  // namespace TransformationOfGalerkinMatrices
