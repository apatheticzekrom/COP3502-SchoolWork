#include <stdio.h>
#include <stdlib.h>

// Calculates slope
float slform(float x1, float x2, float y1, float y2)
{
  float m = ( y2 - y1 ) / ( x2 - x1 );
  return m;
}

// Calculates Y int for the ordered pairs
float intform(float m, float x2, float y2)
{
  // y = mx + b    b = -y/mx
  float b = (- y2 - ( x2 * m ) );
  return b;
}

// Calculates Y for the arrays
void calculate(float x_seti[], float m, float b, float n, float y_seti[])
{
  for(int i = 0; i < n; ++i)
  {
    y_seti[i] = ( ( x_seti[i] * m) + b );
  }
}

// Main function
int main(void)
{
  char title[255];
  char currentline[255];
  float x1, x2, y1, y2, m, b, n, line;
  
  float x_seti[255], y_seti[255];
  float x_set1[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  float x_set2[] = {0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0};
  float x_set3[] = {0.0, 1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0};
  float x_set4[] = {1.0, .1, .01, .001, .0001, .00001};
  float y_set1[11], y_set2[11], y_set3[8], y_set4[6];

  FILE * ifp = fopen("cop3223-as3input.txt", "r");
  FILE * ofp = fopen("cop3223-as3output-giang-benjamin.txt", "w");

  // Title + getting title
  fgets(title, 255, ifp);
  printf("%s\n", title);

  // Gets and prints the 2 ordered pairs
  fscanf(ifp, "%f,%f", &x1, &y1);
  fscanf(ifp, "%f,%f", &x2, &y2);
  printf("The ordered pair:\n");
  printf("(%f,%f) (%f,%f)\n", x1, y1, x2, y2);
  
  // Slope
  m = slform(x1, x2, y1, y2);
  printf("The slope is: %f\n", m);

  // Y intercept
  b = intform(m, x2, y2);
  printf("The Y intercept is: %f\n", b);
  
  // Gets & Prints n
  fscanf(ifp, "%f", &n);
  printf("Number of numbers in file: %f\n", n);

  printf("------------------------------------\n");

  // Loops and sticks the n amount of lines into an array
  // printf("%f\n", x_seti[i]); Example for me
  for(int i = 0; i < n; ++i)
  {
    fscanf(ifp, "%f", &x_seti[i]);
  }

  // Calculates Y values for arrays of input, 1, 2, 3, 4
  calculate(x_seti, m, b, n, y_seti);
  calculate(x_set1, m, b, 11, y_set1);
  calculate(x_set2, m, b, 11, y_set2);
  calculate(x_set3, m, b, 8, y_set3);
  calculate(x_set4, m, b, 6, y_set4);

  // Prints Title, ordered pair, slope, number of numbers in the file, and all 5 sets of results
  // Print n wasn't in the instructions, I thought it was nice to have if a different input file was used
  fprintf(ofp, "Title of file: %s\n", title);
  fprintf(ofp, "The first ordered pair is: (%f, %f) (%f, %f)\n", x1, y1, x2, y2);
  fprintf(ofp, "The slope is: %f\n", m);
  fprintf(ofp, "The y intercept is: %f\n", b);
  // fprintf(ofp, "The amount of numbers in the input file is: %f\n\n", n);
  fprintf(ofp, "---------------------------------------------------------------------\n");
  
  // Prints input array to file
  fprintf(ofp, "The input file set of coordinates:\n");
  for(int i = 0; i < n; ++i)
  {
    fprintf(ofp, "If x = %12.2f y = %12.2f\n", x_seti[i], y_seti[i]);
  }

  // Prints 1st array to file
  fprintf(ofp, "\n---------------------------------------------------------------------\n");
  fprintf(ofp, "The first array of coordinates:\n");
  for(int i = 0; i < 11; ++i)
  {
    fprintf(ofp, "If x = %12.2f y = %12.2f\n", x_set1[i], y_set1[i]);
  }

  // Prints 2nd array to file
  fprintf(ofp, "\n---------------------------------------------------------------------\n");
  fprintf(ofp, "The second array of coordinates:\n");
  for(int i = 0; i < 11; ++i)
  {
    fprintf(ofp, "If x = %12.2f y = %12.2f\n", x_set2[i], y_set2[i]);
  }

  // Prints 3rd array to file
  fprintf(ofp, "\n---------------------------------------------------------------------\n");
  fprintf(ofp, "The third array of coordinates:\n");
  for(int i = 0; i < 8; ++i)
  {
    fprintf(ofp, "If x = %12.2f y = %12.2f\n", x_set3[i], y_set3[i]);
  }

  // Prints 4th array to file
  fprintf(ofp, "\n---------------------------------------------------------------------\n");
  fprintf(ofp, "The fourth array of coordinates:\n");
  for(int i = 0; i < 6; ++i)
  {
    fprintf(ofp, "If x = %15.5f y = %12.5f\n", x_set4[i], y_set4[i]);
  }
  fprintf(ofp, "\n---------------------------------------------------------------------\n");


  fclose(ifp);
  fclose(ofp);
  return(0);
}

