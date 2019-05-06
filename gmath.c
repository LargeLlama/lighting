#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  //variables to calculate light
  color ambient, diffuse, specular;

  //variable to return
  color i;

  //normalize everything
  normalize(normal);
  normalize(view);
  normalize(light[LOCATION]);

  //calculate the ambient, diffuse, and specular. store in their respective variables
  ambient = calculate_ambient(alight, areflect);
  diffuse = calculate_diffuse(light, dreflect, normal);
  specular = calculate_specular(light, sreflect, view, normal);

  //modify RGB of i to fit the 
  i.red = ambient.red + diffuse.red + specular.red;
  i.green = ambient.green + diffuse.green + specular.green;
  i.blue = ambient.blue + diffuse.blue + specular.blue;

  //make sure i doesn't exceed 255 for any of its RGB values
  limit_color(&i);

  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color ambient;

  ambient.red = alight.red * areflect[RED];
  ambient.green = alight.green * areflect[GREEN];
  ambient.blue = alight.blue * areflect[BLUE];

  return ambient;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color diffuse;
  //double to store the cosine of the angle
  double cos;

  //set cos to dot product of the normal and the light on the location
  cos = dot_product(normal, light[LOCATION]);
  if (cos < 0)	//make sure its positive
    cos = 0;

  //set the RGB values
  diffuse.red = light[COLOR][RED] * dreflect[RED] * cos;
  diffuse.green = light[COLOR][GREEN] * dreflect[GREEN] * cos;
  diffuse.blue = light[COLOR][BLUE] * dreflect[BLUE] * cos;

  return diffuse;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color specular;
  
  //store the cos and an array of doubles
  double cos;
  double r[3];

  //set cosine to dot product of the normal and the location of light source
  cos = dot_product(normal, light[LOCATION]);

  //loop thru, setting value of each element in r
  for (int i = 0; i < 3; i++)
    r[i] = 2 * normal[i] * cos - light[LOCATION][i];
  
  //re-assign value to cos as dotproduct of the new array and the view
  cos = dot_product(r, view);
  if (cos < 0) {
    cos = 0;
  }

  cos = pow(cos, SPECULAR_EXP);

  //set the RGB values
  specular.red = light[COLOR][RED] * sreflect[RED] * cos;
  specular.green = light[COLOR][GREEN] * sreflect[GREEN] * cos;
  specular.blue = light[COLOR][BLUE] * sreflect[BLUE] * cos;

  return specular;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if ((*c).red > 255) {
    (*c).red = 255;
  }
  if ((*c).green > 255) {
    (*c).green = 255;
  }
  if ((*c).blue > 255) {
    (*c).blue = 255;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
