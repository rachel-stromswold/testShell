#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "cell.h"

extern int execute(char* input);

int main(int argc, char *argv[]) {
  if (argc < 9) {
    printf("too few arguments supplied\n");
    exit(0);
  }
  double x1, y1, z1, x2, y2, z2, epsilon, r_m;
  sscanf(argv[1], "%lf", &x1);
  sscanf(argv[2], "%lf", &y1);
  sscanf(argv[3], "%lf", &z1);
  sscanf(argv[4], "%lf", &x2);
  sscanf(argv[5], "%lf", &y2);
  sscanf(argv[6], "%lf", &z2);
  sscanf(argv[7], "%lf", &epsilon);
  sscanf(argv[8], "%lf", &r_m);

  struct Vec3 at1Pos, at2Pos;
  at1Pos.x = x1;at1Pos.y = y1;at1Pos.z = z1;
  at2Pos.x = x2;at2Pos.y = y2;at2Pos.z = z2;
  double result = potential_LennardJones(at1Pos, at2Pos, epsilon, r_m);
  printf("Position 1: (%lf,%lf,%lf)\nPosition 2: (%lf,%lf,%lf)\nepsilon: %lf, rm: %lf\n",
  x1, y1, z1, x2, y2, z2, epsilon, r_m);
  printf("Lennard-Jones Potential energy: %lf\n", result);

  return 0;
}
