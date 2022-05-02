/* COP 3502C Assignment 1 This program is written by: Benjamin Giang */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h" //immediately after including all the header files

#define MAX_STR_LEN 51   // Max string length
#define MAX_POP 1000000  // Max population

typedef struct monster
{
    char *name;
    char *element;
    int population;
} monster;

typedef struct region
{
    char *name;
    int nmonsters;
    int total_population;
    monster **monsters;
} region;
    
typedef struct itinerary
{
    int nregions;
    region **regions;
    int captures;
} itinerary;
    
typedef struct trainer
{
    char *name;
    itinerary *visits;
} trainer;


monster* createMonster(char *name, char *element, int population)
{
  // Struct of temp *
  struct monster* temp;

  // malloc struct temp *
  temp = (struct monster*)malloc(sizeof(struct monster));

  // malloc'd temp name and element
  temp->name       = malloc(sizeof(char)     * MAX_STR_LEN);
  temp->element    = malloc(sizeof(char)     * MAX_STR_LEN);

  // Insert to struct
  strcpy(temp->name, name);
  strcpy(temp->element, element);
  temp->population = population;

  return temp;
}

monster** readMonsters(FILE* infile, int *monsterCount)
{
  // Temp variables
  char *item1 = malloc(sizeof(char) * MAX_STR_LEN);
  char *item2 = malloc(sizeof(char) * MAX_STR_LEN);
  int  *item3 = malloc(sizeof(int ) * MAX_POP);
  int i;

  // Make struct of **
  struct monster** monsterList;

  // malloc struct of **
  monsterList = (struct monster**)malloc(sizeof(struct monster*) * *monsterCount);

  // For loop that fscanf's name element population and sends to createMonster
  for(i=0; i<*monsterCount;++i)
  {
    fscanf(infile, "%s %s %d", item1, item2, item3);
    monsterList[i] = createMonster(item1, item2, *item3);
  }

  return monsterList;
}

region** readRegions(FILE* infile, int *countRegions, monster** monsterList, int monsterCount)
{
  // Variables
  int i, j, k;
  char mon[MAX_STR_LEN];

  struct region** regionList;
  regionList = (struct region**)malloc(sizeof(struct region*) * *countRegions);
  
  for(i=0;i<*countRegions;++i)
  {
    char *rname = malloc(sizeof(char) * MAX_STR_LEN);
    int  *rmcount = malloc(sizeof(int ) * MAX_POP);
    char *mname = malloc(sizeof(char) * MAX_STR_LEN);

    regionList[i] = (struct region*)malloc(sizeof(struct region));

    // Puts region name into regionList
    fscanf(infile, "%s", rname);
    regionList[i]->name = rname;

    // Puts region mcount into regionList
    fscanf(infile, "%d", rmcount);
    regionList[i]->nmonsters = *rmcount;

    // Scans word monster and holds
    fscanf(infile, "%s", mon);

    regionList[i]->monsters = malloc(sizeof(struct monster*) * regionList[i]->nmonsters);

    // Compares name with monsterlist name, and if same, set regionlist to pointer of monsterlist

    
    for(j=0; j<*rmcount;j++)
    {
      fscanf(infile, "%s", mname);
      
      for(k=0;k<monsterCount;++k)
      {
        
        if(strcmp(monsterList[k]->name, mname) == 0)
        {
          regionList[i]->monsters[j] = monsterList[k];
          regionList[i]->total_population += monsterList[k]->population;
          
        }
      }
    }
    // for(int h=0; h<=i;++h){
    // printf("%d : %s\n", h, regionList[h]->name);
    // printf("%d rmcount: %d\n", h, regionList[h]->nmonsters);
    // }
  }

  return regionList;
}


trainer* readTrainers(FILE* infile, int *trainerCount, region** regionList, int countRegions)
{

  
  char cap[MAX_STR_LEN], reg[MAX_STR_LEN];
  int i, j, k;

  // Trainer struct
  struct trainer* trainerList;
  trainerList = (struct trainer*)malloc(sizeof(struct trainer) * *trainerCount);
  
  
  for(i=0;i<*trainerCount;i++)
  {
    // Itinerary variables
    int  *rcount = malloc(sizeof(int ) * MAX_POP);
    char *rname =  malloc(sizeof(char) * MAX_STR_LEN);
    int  *ccount = malloc(sizeof(int ) * MAX_POP);

    // Trainer variables
    char *tname = malloc(sizeof(char) * MAX_STR_LEN);

    trainerList[i] = *(struct trainer*)malloc(sizeof(struct trainer));
    trainerList[i].visits = (itinerary*) malloc(sizeof(itinerary));

    // Puts trainer name into trainerList
    fscanf(infile, "%s", tname);
    trainerList[i].name = tname;
  
    // scans ccount
    fscanf(infile, "%d", ccount);
    trainerList[i].visits->captures = *ccount;

    // Scans word captures and holds
    fscanf(infile, "%s", cap);

    // scans rcount
    fscanf(infile, "%d", rcount);
    trainerList[i].visits->nregions = *rcount;

    // Scans word captures and holds
    fscanf(infile, "%s", reg);

    trainerList[i].visits->regions = malloc(sizeof(struct region*) * *rcount);

    for(j=0;j<*rcount;j++)
    {
      fscanf(infile, "%s", rname);

      for(k=0; k<countRegions; ++k)
      {
        if(strcmp(regionList[k]->name, rname) == 0)
        {
          trainerList[i].visits->regions[j] = regionList[k];
          
        }
      }
    }

  }
  
  return trainerList;
}


void process_inputs(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount)
{
// Opens output file
  FILE *outfile;
  outfile = fopen("out.txt", "w");

  for(int i=0; i<trainerCount; i++) // i prints trainer name
  {
    printf("%s\n", trainerList[i].name);
    fprintf(outfile, "%s\n", trainerList[i].name);

    for(int j=0; j<trainerList[i].visits->nregions; j++) // x prints region name
    {
      printf("%s\n", trainerList[i].visits->regions[j]->name);
      fprintf(outfile, "%s\n", trainerList[i].visits->regions[j]->name);

      for(int k = 0; k<trainerList[i].visits->regions[j]->nmonsters; k++) // y
      {
        float numcaptures = (float) trainerList[i].visits->regions[j]->monsters[k]->population / trainerList[i].visits->regions[j]->total_population;
        numcaptures*=trainerList[i].visits->captures;
        numcaptures = round(numcaptures);
        int ncaptures = numcaptures;     
        if(ncaptures>0)
        {
          printf("%d %s\n", ncaptures, trainerList[i].visits->regions[j]->monsters[k]->name);
          fprintf(outfile, "%d %s\n", ncaptures, trainerList[i].visits->regions[j]->monsters[k]->name);
        }
    
      }
    
    }
  printf("\n");
  fprintf(outfile, "\n");
}
fclose(outfile);
}


void release_memory(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount)
{
    for(int i = 0; i<trainerCount; i++)
    {
      free(trainerList[i].visits);
    }  
    free(trainerList);

    for(int i = 0; i<regionCount; i++)
    {
      free(regionList[i]->monsters);
    }   
    free(regionList);
    
    for(int i = 0; i<monsterCount; i++)
    {
      
      free(monsterList[i]->name);
      free(monsterList[i]->element);
    }
    free(monsterList);
  
}

int main(void)
{
  atexit(report_mem_leak); // Memory leak detector

  //// Monster stuff
  int monsterCount;
  char mons[MAX_STR_LEN]; // Just to hold words

  // Opens File
  FILE *infile;
  infile = fopen("in.txt", "r");

  // Gets monsterCount (Number of monsters)
  fscanf(infile, "%d", &monsterCount);

  // Stores the word monster
  fscanf(infile, "%s", mons);

  // The struct and readmonster function
  struct monster** monsterList;
  monsterList = readMonsters(infile, &monsterCount);


  //// Region stuff
  int countRegions;
  char region[MAX_STR_LEN];

  // scanf's countRegion and word region
  fscanf(infile, "%d", &countRegions);
  fscanf(infile, "%s", region);

  // The struct and readregion function
  struct region** regionList;
  regionList = readRegions(infile, &countRegions, monsterList, monsterCount);


  //// Trainer stuff
  int trainerCount;
  char trainer[MAX_STR_LEN];

  // scanf's trainercount and word trainer
  fscanf(infile, "%d", &trainerCount); // This is causing a seg fault
  fscanf(infile, "%s", trainer);

  // The struct and readtrainer function
  struct trainer* trainerList;
  trainerList = readTrainers(infile, &trainerCount, regionList, countRegions);

  //// Process process inputs 
  process_inputs(monsterList, monsterCount, regionList, countRegions, trainerList, trainerCount);

  // Release memory
  release_memory(monsterList, monsterCount, regionList, countRegions, trainerList, trainerCount);
  
  fclose(infile);
  return 0;
}



