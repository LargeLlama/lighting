#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"
#include "stack.h"
#include "gmath.h"

int main(int argc, char **argv) {

  // lighting values here for easy access
  color ambient;
  double light[2][3];
  double view[3];
  double areflect[3];
  double dreflect[3];
  double sreflect[3];

  // ambient light
  ambient.red = 0;
  ambient.green = 0;
  ambient.blue = 0;

  // one point light source
  light[LOCATION][0] = 0.2;
  light[LOCATION][1] = 0.1;
  light[LOCATION][2] = 0.8;

  light[COLOR][RED] = 200;
  light[COLOR][GREEN] = 0;
  light[COLOR][BLUE] = 255;

  // view vector
  view[0] = 1;
  view[1] = 1;
  view[2] = 0;

  // reflective constants
  areflect[RED] = 0;
  areflect[GREEN] = 0;
  areflect[BLUE] = 0;

  dreflect[RED] = 0.5;
  dreflect[GREEN] = 0.5;
  dreflect[BLUE] = 0.5;

  sreflect[RED] = 0.5;
  sreflect[GREEN] = 0.5;
  sreflect[BLUE] = 0.5;

  screen s;
  zbuffer zb;
  struct matrix * edges;
  struct matrix * polygons;
  struct stack * csystems;

  edges = new_matrix(4, 4);
  polygons = new_matrix(4, 4);
  csystems = new_stack();

  if ( argc == 2 )
    parse_file( argv[1], csystems, edges, polygons, s, zb,
                view, ambient, light, areflect, sreflect, dreflect);
  else
    parse_file( "stdin", csystems, edges, polygons, s, zb,
                view, ambient, light, areflect, sreflect, dreflect);

  free_matrix( edges );
  free_matrix( polygons );
  free_stack(csystems);
}
