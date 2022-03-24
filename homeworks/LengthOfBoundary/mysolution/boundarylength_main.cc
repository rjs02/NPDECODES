/**
 * @ file boundarylength_main.cc
 * @ brief NPDE homework LengthOfBoundary code
 * @ author Christian Mitsch
 * @ date 03.03.2019
 * @ copyright Developed at ETH Zurich
 */

#include <iostream>
#include <utility>

#include "boundarylength.h"

using namespace LengthOfBoundary;

/* SAM_LISTING_BEGIN_1 */
int main(int argc, char *argv[]) {
  //====================
  // Your code goes here
  //====================

  std::string filename = argv[1];
  std::cout << "reading file " << filename << std::endl;
  auto data = measureDomain(filename);
  std::cout << "Volume of domain:   " << data.first
            << "\nLength of boundary: " << data.second << std::endl;

}
/* SAM_LISTING_END_1 */
