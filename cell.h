#include <stdio.h>
#include <math.h>

struct Vec3 {
  double x;
  double y;
  double z;
};

double potential_LennardJones (struct Vec3 atom1Pos, struct Vec3 atom2Pos, double epsilon, double r_m) {
  double r = sqrt( (atom2Pos.x-atom1Pos.x)*(atom2Pos.x-atom1Pos.x)
		  +(atom2Pos.y-atom1Pos.y)*(atom2Pos.y-atom1Pos.y)
		  +(atom2Pos.z-atom1Pos.z)*(atom2Pos.z-atom1Pos.z));
  double ratio = INFINITY;
  if (r != 0.0) {
    ratio = r_m/r;
  }
  return epsilon*( pow(ratio, 12)-2*pow(ratio, 6) );
}

double calculate_internal(int cell_rad, )
