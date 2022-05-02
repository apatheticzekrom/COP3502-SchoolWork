/* COP 3502C Programming Assignment 3
This program is written by: Benjamin Giang */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include	"leak_detector_c.h"

#define xMAX 10000
#define yMAX 10000
#define nMAX 1000000
#define sMAX 200000
#define tMAX 30

int xme, yme;

// Originally did the assignment with arrays of *x *y but then realized I wasnt supposed to do that halfway through and I was told to check the discussion for clarification but it didn't clarify anything but that's ok cuz I got 3 hours left

typedef struct monster
{
  int x;
  int y;
  int d;
}monster;

// Reads first line
void readFirst(FILE* infile, int *n, int *s, int *t)
{
  fscanf(infile, "%d %d %d %d %d", &xme, &yme, n, s, t);
}

// Finds distance between the 2 points
int distance(int p1, int p2, int n)
{
  int dist;
  for(int i=0; i<n; i++)
  {
    dist = (sqrt(pow((xme - p1), 2) + pow((yme - p2), 2)));
  }
  return dist;
}

// Reads the coords based on n
monster* readData(FILE* infile, int *n)
{
  int  item1;
  int  item2;

  struct monster* monsterList;
  monsterList = (struct monster*)malloc(sizeof(struct monster) * *n);

  for(int i=0; i<*n;++i)
  {
    fscanf(infile, "%d %d", &item1, &item2);
    monsterList[i].x = item1;
    monsterList[i].y = item2;

    // printf("%d %d\n", monsterList->x, monsterList->y); // Prints coords to console

    int dist = distance(item1, item2, *n);
    monsterList[i].d = dist;
  }
  return monsterList;
}

// Returns 0 if distance is =, -1 if p1 closer then p2, 1 if p1 is further then p2
int compareTo(monster *ptrPt1, monster *ptrPt2)
{
  if(ptrPt1->d > ptrPt2->d)
  {
    return 1;
  }
  else if(ptrPt1->d < ptrPt2->d)
  {
    return -1;
  } 
  else // (ptrPt1->d == ptrPt2->d)
  {
    if(ptrPt1->x < ptrPt2->x)
    {
      return -1;
       
    } else if(ptrPt1->x > ptrPt2->x) 
    {
      return 1;
    } else // (ptrPt1->x == ptrPt2->x)
    {
      if(ptrPt1->y < ptrPt2->y) 
      {
        return -1;
      } else // (ptrPt1->y > ptrPt2)
      {
        return 1;
      }
    }
  }
}


// Sort stuff
// l is left index, m is midpoint, r is right index
void merge(monster* arr, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1; // size of L
  int n2 = r - m; // size of R

  // Create temp arrays
  monster *L = (struct monster*)malloc(sizeof(struct monster) * n1);
  monster *R = (struct monster*)malloc(sizeof(struct monster) * n2);

  // Copies data to temp arrays L[] & R[]
  for(i=0; i<n1;i++)
  {
    L[i] = arr[l + i];
  }

  for(j=0; j<n2;j++)
  {
    R[j] = arr[m + 1 + j];
    // printf("%d %d Filling R[]\n", R[j].x, R[j].y);
  }

  // Merge temp arrays back into arr[l...r]
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  
  while (i < n1 && j < n2)
  {
    if(L[i].d < R[j].d)
    {
      arr[k] = L[i];
      i++;
      // printf("(L[i].d < R[j].d) k: %d x: %d y: %d\n", k, arr[k].x, arr[k].y);
    }
    else if (L[i].d > R[j].d)
    {
      arr[k] = R[j];
      j++;
      // printf("(L[i].d > R[j].d) k:  %d x: %d y: %d\n", k, arr[k].x, arr[k].y);
    }

    else // if(L[i].d == R[j].d)
    {
      if (compareTo(&L[i], &R[j]) < 0)
      {
        arr[k] = L[i];
        i++;
        // printf("(compareTo(&L[i], &R[j]) < 0) k: %d x: %d y: %d\n", k, arr[k].x, arr[k].y);
      } else // (compareTo(L, R) > 0)
      {
        arr[k] = R[j];
        j++;
        // printf("(compareTo(&L[i], &R[j]) > 0) k: %d x: %d y: %d\n", k, arr[k].x, arr[k].y);
      }
    }
    k++;
  }

  // Copies the remaining elements of R[], if any
  while(i<n1)
  {
    arr[k] = L[i];
    i++;
    k++;
  } 

  while(j<n2)
  {
    arr[k] = R[j];
    j++;
    k++;
  } 
  
  free(L);
  free(R);
  
}


// l is left index, r is right index
void mergeSort(monster monster[], int l, int r)
{

  if(l < r)
  {

    // gets the midpoint
    int m = (l+r)/2;

    // Sort first and second halves
    mergeSort(monster, l, m);
    mergeSort(monster, m+1, r);

    // printf("Testing l=%d r=%d m=%d\n", l, r, m);

    merge(monster, l, m, r);

  }
}

void insertionSort(monster arr[], int n)
{
  int i, j;

  monster item;

  for(i = 1; i<n; i++)
  {
    item = arr[i];

    for(j=i-1; j>=0; j--)
    {
      if(arr[j].d>item.d)
      {
        arr[j+1] = arr[j];
      }
      else
      {
        break;
      }
    }
    arr[j+1] = item;
  }
}

// Wrapper function
void sort(monster monster[], int *n, int *t)
{
  int arrn;
  arrn = *n;

  if
  (arrn <= *t)
  {
    insertionSort(monster, arrn);
  } else
  {
    mergeSort(monster, 0, arrn-1);
  }
}

int binarySearch(monster arr1[], int l, int r, monster* search) 
{ 
  if (r >= l)
  { 
    int mid = l + (r - l) / 2; 


    // If the element is present at the middle, then it's itself
        if(arr1[mid].x == search->x && arr1[mid].y == search->y) 
        {
          return mid;
        }

        // printf("ax: %d, ay: %d, sx: %d, sy: %d\n", arr1[mid].x,arr1[mid].y,search->x,search->y);

    // If element is smaller than mid, then it can only be present in left subarray 
        if (compareTo(&arr1[mid], search) > 0)
        {
          return binarySearch(arr1, l, mid - 1, search); 
        }

        // Else the element can only be present in right subarray 
        return binarySearch(arr1, mid + 1, r, search); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return -1; 
} 




int main(void) {
  FILE* infile;
  FILE* outfile;
  infile = fopen("in.txt", "r");

  // Reads and stores firstline
  int n, s, t, b, rank;
  readFirst(infile, &n, &s, &t);
  b = n + s; // n + s
  // printf("%d %d %d %d %d\n", xme, yme, n, s, t); // Prints first line

  // Puts coords into a struct array
  struct monster* monsterList;
  monsterList = readData(infile, &b);
  sort(monsterList, &n, &t);

  // Prints sorted array
  for(int i=0; i<n;++i)
  {
    printf("%d %d\n", monsterList[i].x, monsterList[i].y);
  }

  for(int i=n; i<b;++i)
  {
    rank = binarySearch(monsterList, 0, n-1, &monsterList[i]);
    
    if (rank < 0)
    {
      printf("%d %d not found\n", monsterList[i].x, monsterList[i].y);
    }
    else
    {
      rank = rank + 1;
      printf("%d %d found at rank %d\n", monsterList[i].x, monsterList[i].y, rank);
    }
  }
  

  return 0;
}
