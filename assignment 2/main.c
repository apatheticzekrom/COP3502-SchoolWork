/* COP 3502C Assignment 2 This program is written by: Benjamin Giang */
// Kinda wished we got premade structs again ngl (Main function is all the way at the bottom)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

#define MAX_STR_LEN 50
#define MAX_GROUND_NUM 10

// Singly linked list for sorting input
typedef struct node
{
  int info;
  struct node* next;
  struct queue* queue_array;
}node;

// Circular doubly linked that will be used to fill each queue
typedef struct soldier
{
  int sequenceNumber;
  struct soldier* next;
  struct soldier* prev;
}soldier;

// Queue with grounds info
typedef struct queue
{
  int gNumber;
  int nodeCount;
  int origNodeCount;
  int k;
  int th;
  char* name;
  soldier* head;
  soldier* tail;
}queue;


// -----------------------------------------------------------

// Initializes queue
void initQueue(queue* qPtr)
{
  qPtr->head = NULL;
  qPtr->tail = NULL;
}

// Sorts input into a singly linked list
node* sortInsert(node* head, int nGrouds, char* groundName, int nSoldiers, int k, int th)
{
  node* temp;
  node* temp2;
  
  // Puts info from singly linked list to queue
  temp = (node*) malloc(sizeof(node));
  temp->info = nGrouds;
  temp->next = NULL;
  temp->queue_array = (queue*) malloc(sizeof(queue));
  temp->queue_array->name = (char*) malloc(MAX_STR_LEN * sizeof(char));
  temp->queue_array->name = strcpy(temp->queue_array->name, groundName);
  temp->queue_array->nodeCount = nSoldiers;
  temp->queue_array->origNodeCount = nSoldiers;
  temp->queue_array->k = k;
  temp->queue_array->th = th;
  temp->queue_array->gNumber = temp->info;

  // Sorts from low to higher ground number
  if (head == NULL || head->info >= nGrouds) 
  {
    temp->next = head;
    head = temp;
  } else 
  {
    temp2 = head;
    while (temp2->next != NULL && temp2->next->info < nGrouds) 
    {
      temp2 = temp2->next;
    }
    temp->next = temp2->next;
    temp2->next = temp;
  }
  return head;
}

// Allocates soldier node and returns it
soldier* createSoldier(int sequence)
{
  soldier* newSoldier = (soldier*) malloc(sizeof(soldier));
  if (newSoldier != NULL) 
  {
    newSoldier->sequenceNumber = sequence;
    newSoldier->next = NULL;
    newSoldier->prev = NULL;
    return newSoldier;
  }
  return 0;
}

void enqueue(queue* qPtr, int sequenceN)
{ 
  struct soldier* temp = createSoldier(sequenceN);
    
  // Makes linked list circular
  if (!qPtr->head) 
  {
    qPtr->head = temp;
    qPtr->head->next = qPtr->head;
    qPtr->head->prev = qPtr->head;
    qPtr->tail = qPtr->head->prev;
    return;
  }
    
    temp->next = qPtr->head;
    temp->prev = qPtr->head->prev;
    temp->prev->next = temp;
    qPtr->head->prev = temp;
    qPtr->tail = temp;
}

// Creates the queues (inversed)
void createReverseCircle(queue* q)
{
  int counter = q->nodeCount;
  for (int j = 0; j < MAX_GROUND_NUM; j++) 
  {
    if (counter > 0) 
    {
      enqueue(q, counter);
    }
    counter--;
  }
}

// Reverses the queues
void rearrangeCircle(queue* q)
{
  if (q->head == NULL) 
  {
    printf("List is empty\n");
    return;
  }

  soldier* current;
  soldier* t;
  current = q->head;

  do 
  {
    soldier* temp;
    temp = current->prev;
    current->prev = current->next;
    current->next = temp;
    current = current->next;
  } while (current != q->head);

  t = q->head;
  q->head = q->tail;
  q->tail = t;

}

// From the lecture
int empty(queue* qPtr)
{
    return qPtr->nodeCount == 0;
}

// Prints to console
void display(queue q)
{
  if (q.head == NULL) 
  {
  printf("Queue is empty\n");
  return;
  }
    
  soldier* t = q.head;
  printf("%d %s ", q.gNumber, q.name);
  printf("%d ", q.head->sequenceNumber);
  q.head = q.head->next;
    
  while (t != q.head) 
  {
    printf("%d ", q.head->sequenceNumber);
    q.head = q.head->next;
  }
  printf("\n");
}

// Prints to output file
void printQueue(queue q, FILE* output)
{
  if (q.head == NULL) 
  {
    fprintf(output,"Queue is empty\n");
    return;
  }
    
  soldier* t = q.head;
  fprintf(output ,"%d %s ", q.gNumber, q.name);
  fprintf(output ,"%d ", q.head->sequenceNumber);
  q.head = q.head->next;
    
  while (t != q.head) 
  {
    fprintf(output ,"%d ", q.head->sequenceNumber);
    q.head = q.head->next;
  }
  fprintf(output, "\n");
}

// Puts info from singly linked list to queue
void fillQueueInfo(queue* q, node* head)
{
  q->gNumber = head->queue_array->gNumber;
  q->nodeCount = head->queue_array->nodeCount;
  q->origNodeCount = head->queue_array->origNodeCount;
  q->k = head->queue_array->k;
  q->th = head->queue_array->th;
  q->name = head->queue_array->name;
}

// Frees singly linked list
void freeList(node* head)
{
  while (head != NULL) 
  {
    free(head->queue_array->name);
    head = head->next;
  }
  free(head);
}

void prePhase1(FILE* input, FILE* output, int G, queue** qPtr)
{
  node* head = NULL;
  int nGrouds, nSoldiers, k ,th;
  char groundName[MAX_STR_LEN];

  // Sorts from low to high ground number
  for (int i = 0; i < G; i++) 
  {
    fscanf(input, "%d %s %d %d %d", &nGrouds, groundName, &nSoldiers, &k, &th);
    head = sortInsert(head, nGrouds, groundName, nSoldiers, k, th);
  }

  // Fills list
  fprintf(output, "Initial nonempty lists status\n");
  printf("Initial nonempty lists status\n");
  for (int i = 0; i < G; i++) 
  {
    fillQueueInfo(qPtr[i], head);
    createReverseCircle(qPtr[i]);
    display(*qPtr[i]);
    printQueue(*qPtr[i] , output);
    head = head->next;
  }

  //Reversing part
  fprintf(output, "\nAfter ordering nonempty lists status\n");
  printf("\nAfter ordering nonempty lists status\n");
  for (int i = 0; i < G; i++) 
  {
    rearrangeCircle(qPtr[i]);
    display(*qPtr[i]);
    printQueue(*qPtr[i] , output);
  }
  freeList(head);
}

int dequeue(queue* q)
{
  soldier* temp;
  int retsequenceN;
    
  if (q->head == NULL) 
  {
    return -1;
  }
  retsequenceN = q->head->sequenceNumber;
  temp = q->head;
  temp->prev->next = temp->next;
  temp->next->prev = temp->prev;
  q->head = q->head->next;
  q->tail = q->head->prev;
  if (q->head == NULL)
  {
    q->tail = NULL;
  }

  free(temp);
  return retsequenceN;
}

void rotateQueue(queue* q, int k)
{
  if (k == 0) 
  {
    return;
  }

  soldier* current = q->head;
  int count = 0;
  while (count < k - 1) 
  {
    current = current->next;
    count++;
  }
  
  soldier* t = current;
  while (current->next != q->head) 
  {
    current = current->next;
  }

  current->next = q->head;
  q->head = t->next;
  t->next = q->head;
}

void phase1(queue** q, FILE* output)
{
  fprintf(output, "\nPhase1 execution\n");
  printf("\nPhase1 execution\n");
  fprintf(output, "\n");
  printf("\n");

  for (int i = 0; i < MAX_GROUND_NUM; i++) 
  {
    if (empty(q[i])) 
    {
      return;
    }

    int executed;
    rotateQueue(q[i], q[i]->k - 1);
    executed = dequeue(q[i]);
    q[i]->nodeCount--;
    fprintf(output, "Line# %d %s\n", q[i]->gNumber, q[i]->name);
    printf("Line# %d %s\n", q[i]->gNumber, q[i]->name);
    fprintf(output, "Soldier# %d executed\n", executed);
    printf("Soldier# %d executed\n", executed);

    for (int j = q[i]->nodeCount; j > q[i]->th ; j--) 
    {
      rotateQueue(q[i], q[i]->k - 1);
      executed = dequeue(q[i]);
      q[i]->nodeCount--;
      fprintf(output, "Soldier# %d executed\n", executed);
      printf("Soldier# %d executed\n", executed);
    }
    fprintf(output, "\n");
    printf("\n");
  }
}

// Sorts remaining members after phase 1
void sortNewLists(queue* q)
{
  while (q->head->sequenceNumber > q->head->next->sequenceNumber) 
  {
    q->head = q->head->next;
    if (q->head->prev->sequenceNumber < q->head->sequenceNumber) 
    {
      q->head = q->head->prev;
    }
  }
  q->tail = q->head->prev;
}

void prePhase2(queue** q)
{
  for (int i = 0; i < MAX_GROUND_NUM; i++) 
  {
    if (empty(q[i])) 
    {
      return;
    }
    sortNewLists(q[i]);
  }
}

void phase2(queue** q, FILE* output, int G)
{
  fprintf(output, "Phase 2 execution\n");
  printf("Phase 2 execution\n");
  int count = 0;
  
  // Stops executing process when only 1 remaining (th)
  for (int i = 0; i < G; i++) 
  {
    count += q[i]->th;
  }

  // checks if queue is empty
  for (int i = 0; i < G; i++) 
  {
    if (empty(q[i]))
    {
      break;
    }

    int max = 0;
    for (int j = 0; j < G; j++) 
    {
      if (empty(q[j])) 
    {
    break;
    }
            
    // Runs if 2 grounds have the same orig nodecount
    if (q[j]->origNodeCount == q[i]->origNodeCount && i != j && q[j]->origNodeCount != 0 && q[i]->origNodeCount != 0) 
    {
      if (q[j]->gNumber < q[i]->gNumber) 
      {
        for (int k = 0; k < q[j]->nodeCount; k++) 
        {
          if (count == 1) 
          {
            int soldierChosen = dequeue(q[j]);
            fprintf(output, "\nSoldier %d from line %d will survive\n", soldierChosen, q[j]->gNumber);
            printf("\nSoldier %d from line %d will survive\n", soldierChosen, q[j]->gNumber);
            break;
          }
            int soldierExecuted = dequeue(q[j]);
            fprintf(output, "Executed soldier %d from line %d\n", soldierExecuted, q[j]->gNumber);
            printf("Executed soldier %d from line %d\n", soldierExecuted, q[j]->gNumber);
            count--;
            q[j]->origNodeCount = 0;
        }

        for (int k = 0; k < q[i]->nodeCount; k++) 
        {
          if (count == 1) 
          {
            int soldierChosen = dequeue(q[i]);
            fprintf(output, "\nSoldier %d from line %d will survive\n", soldierChosen, q[i]->gNumber);
            printf("\nSoldier %d from line %d will survive\n", soldierChosen, q[i]->gNumber);
            break;
          }

          int soldierExecuted = dequeue(q[i]);
          fprintf(output, "Executed soldier %d from line %d\n", soldierExecuted, q[i]->gNumber);
          printf("Executed soldier %d from line %d\n", soldierExecuted, q[i]->gNumber);
          count--;
          q[i]->origNodeCount = 0;
        }
      } 

      else 
      {
        for (int k = 0; k < q[i]->nodeCount; k++) 
        {
          if (count == 1) 
          {
            int soldierChosen = dequeue(q[i]);
            fprintf(output, "\nSoldier %d from line %d will survive\n", soldierChosen, q[i]->gNumber);
            printf("\nSoldier %d from line %d will survive\n", soldierChosen, q[i]->gNumber);
            break;
          }

          int soldierExecuted = dequeue(q[i]);
          fprintf(output, "Executed soldier %d from line %d", soldierExecuted, q[i]->gNumber);
          printf("Executed soldier %d from line %d", soldierExecuted, q[i]->gNumber);
          count--;
          q[i]->origNodeCount = 0;
        }
        for (int k = 0; k < q[j]->nodeCount; k++) 
        {
          if (count == 1) 
          {
            int soldierChosen = dequeue(q[j]);
            fprintf(output, "\nSoldier %d from line %d will survive\n", soldierChosen, q[j]->gNumber);
            printf("\nSoldier %d from line %d will survive\n", soldierChosen, q[j]->gNumber);
            break;
          }
          int soldierExecuted = dequeue(q[j]);
          fprintf(output, "Executed soldier %d from line %d\n", soldierExecuted, q[j]->gNumber);
          printf("Executed soldier %d from line %d\n", soldierExecuted, q[j]->gNumber);
          count--;
          q[j]->origNodeCount = 0;
        }
      }
    } 
    else if (q[i]->origNodeCount > q[j]->origNodeCount)
    {
      if (q[i]->origNodeCount > q[max]->origNodeCount) 
      {
        max = i;
      }
    } 
    else
      if (q[j]->origNodeCount > q[max]->origNodeCount)
      {
        max = j;
      }
    }

    // Survivors
    for (int k = 0; k < q[max]->nodeCount; k++) 
    {
      if (count == 1) 
      {
        int soldierChosen = dequeue(q[max]);
        fprintf(output, "\nSoldier %d from line %d will survive\n", soldierChosen, q[max]->gNumber);
        printf("\nSoldier %d from line %d will survive\n", soldierChosen, q[max]->gNumber);
        break;
      }

      int soldierExecuted = dequeue(q[max]);
      fprintf(output, "Executed soldier %d from line %d\n", soldierExecuted, q[max]->gNumber);
      printf("Executed soldier %d from line %d\n", soldierExecuted, q[max]->gNumber);
      count--;
      q[max]->origNodeCount = 0;
    }
  }
}

// Frees memory
void freeMemory(queue** q)
{
  for (int i = 0; i < MAX_GROUND_NUM; i++) 
  {
    free(q[i]);
  }
  free(q);
}

// -------------------------------------------------------------------------------

int main()
{
  atexit(report_mem_leak);
  FILE* input;
  FILE* output;

  input = fopen("in.txt", "r");
  output = fopen("out.txt", "w");
  
  // scans number of grounds
  int G;
  fscanf(input,"%d", &G);
  
  // DMA array of queues
  queue** qPtr = (struct queue**) malloc(MAX_GROUND_NUM * sizeof(queue*));
  for (int i = 0; i < MAX_GROUND_NUM; i++) 
  {
    qPtr[i] = (struct queue*) malloc(sizeof(queue));
    initQueue(qPtr[i]);
  }
  
  // Phase 1
  prePhase1(input, output, G, qPtr);
  phase1(qPtr, output);

  // Phase 2
  prePhase2(qPtr);
  phase2(qPtr, output, G);

  //Free memory
  freeMemory(qPtr);

  //Close files
  fclose(input);
  fclose(output);
  return 0;
}

