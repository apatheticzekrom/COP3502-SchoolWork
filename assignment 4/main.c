/* COP 3502C Programming Assignment 4
This program is written by: Benjamin Giang */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 30
#include	"leak_detector_c.h"

FILE* outfile;

typedef struct itemNode
{
  char name[MAXLEN];
  int count;
  struct itemNode *left, *right;
  }itemNode;

typedef struct treeNameNode
{
  char treeName[MAXLEN];
  struct treeNameNode *left, *right;
  itemNode *theTree;
}treeNameNode;

// -------------------------------------------------------
// Build name tree
treeNameNode* createTreeNameNode(char* treeN)
{
  // Allocate space for the node, set the fields.
  treeNameNode* temp = (struct treeNameNode*)malloc(sizeof(struct treeNameNode));
  temp->theTree = NULL;
  temp->left = NULL;
  temp->right = NULL;
  strcpy(temp->treeName, treeN);
  return temp; // Return a pointer to the created node.
}

treeNameNode* insertNameNode(treeNameNode* root, treeNameNode* newNode)
{
  if (root == NULL)
  {
    return newNode;
  }
  else
  { // root name > newNode name
    if (strcmp(root->treeName, newNode->treeName) > 0)
    {
  // if an element exists to the left of 'root' then call again
      if (root->left != NULL)
      {
        root->left = insertNameNode(root->left, newNode);
      }
      else
      {
        root->left = newNode;
      }
    } // root name < newNode name OR they are equal strings
    else
    {
  // if an element exists to the right of 'root' then call again
      if (root->right != NULL)
      {
        root->right = insertNameNode(root->right, newNode);
      }
      else
      {
        root->right = newNode;
      }
    }
  }
  return root;
}

treeNameNode* buildNameTree(FILE* infile, int N)
{
  if (infile == NULL)
  {
    return NULL;
  }
  treeNameNode* nameRoot = NULL;
  char name[MAXLEN];
  for (int x = 0; x < N; x++)
  {
    fscanf(infile, "%s", name);
    treeNameNode* newNameNode = createTreeNameNode(name);
    nameRoot = insertNameNode(nameRoot, newNameNode);
  }
  return nameRoot;
}

// -------------------------------------------------------
// Populate trees

itemNode* createItemNode(char* name, int count)
{
  itemNode* temp = (itemNode*)malloc(sizeof(itemNode));
  strcpy(temp->name, name);
  temp->count = count;
  temp->left = NULL;
  temp->right = NULL;
  return temp;
}

itemNode* insertItemNode(itemNode* treeRoot, itemNode* newNode)
{
  if (treeRoot == NULL)
  {
    return newNode;
  }
  else
  {
  // if treeRoot name > newNode name
    if (strcmp(treeRoot->name, newNode->name) > 0)
    {
      // if an element exists to the left of it's 'root' then call again
      if (treeRoot->left != NULL)
      {
        treeRoot->left = insertItemNode(treeRoot->left, newNode);
      }
      else
      {
        treeRoot->left = newNode;
      }
    }
    // treeRoot name < newNode name OR they are equal length
    else
    {
      // if an element exists to the right of it's 'root' then call again
      if (treeRoot->right != NULL)
      {
        treeRoot->right = insertItemNode(treeRoot->right, newNode);
      }
      else
      {
        treeRoot->right = newNode;
      }
    }
  }
  return treeRoot;
}

treeNameNode* searchNameNode(treeNameNode* root, char treeName[50])
{
  if (root == NULL)
  {
    return NULL;
  }
  treeNameNode* nameNode;
  if (strcmp(root->treeName, treeName) == 0)
  {
    return root;
  }
  else if (strcmp(root->treeName, treeName) > 0)
  {
  nameNode = searchNameNode(root->left, treeName);
  }
  else
  {
    nameNode = searchNameNode(root->right, treeName);
  }
  return nameNode;
}

void populateTrees(FILE* infile, treeNameNode* nameRoot, int I)
{
  if (infile == NULL)
  {
    return;
  }
  else
  {
    int count;
    char treeName[MAXLEN];
    char itemName[MAXLEN];
    for (int x = 0; x < I; x++)
    {
      fscanf(infile, "%s %s %d", treeName, itemName, &count);
      itemNode* newItemNode = createItemNode(itemName, count);
      treeNameNode* treeNode = searchNameNode(nameRoot, treeName);
      treeNode->theTree = insertItemNode(treeNode->theTree, newItemNode);
    }
  }
}

// -------------------------------------------------------

void displayInOrderNameTree(treeNameNode* root)
{
  if(root != NULL)
  {
    displayInOrderNameTree(root->left);
    printf("%s ", root->treeName);
    displayInOrderNameTree(root->right);
  }
}

void traverseSubTree(itemNode* root)
{ 
  if(root != NULL)
  {
    traverseSubTree(root->left);
    printf("%s ", root->name);
    traverseSubTree(root->right);
  }
}

void traverse_in_traverse(treeNameNode* root)
{
  if(root != NULL)
  {
    traverse_in_traverse(root->left);
    printf("\n===%s===\n", root->treeName);
    traverseSubTree(root->theTree);
    traverse_in_traverse(root->right);
  }
}

// -------------------------------------------------------
// Free Stuff
void freeTree(itemNode* root)
{
  if(root != NULL)
  {
    freeTree(root->left);
    freeTree(root->right);
    free(root);
  }
}

void freeAll(treeNameNode* root)
{
  if(root != NULL)
  {
    freeAll(root->left);
    freeAll(root->right);
    freeTree(root->theTree);
    free(root);
  }
}

// -------------------------------------------------------
// Query stuff

void queries (FILE* infile, treeNameNode* root, int Q)
{
  char query[MAXLEN], w1[MAXLEN], w2[MAXLEN];
  int reduceby;
  for (int x = 0; x < Q; x++)
    {
      fscanf(infile, "%s", query);
      
      if(strcmp(query, "search") == 0)
      {
        fscanf(infile, "%s %s", w1, w2);
        
      }
    }
}



// -------------------------------------------------------
int main(void)
{
  FILE *infile;
  infile = fopen("in.txt", "r");
  outfile = fopen("out.txt", "w");
  // N = number of Tree Names
  // I = total number of items in the list to be inserted to all the trees
  // Q = number of queries listed in the input file
  int N, I, Q;
  
  fscanf(infile, "%d %d %d", &N, &I, &Q);
  //printf("%d %d %d", N, I, Q);

  treeNameNode* nameRoot = buildNameTree(infile, N);
  
  populateTrees(infile, nameRoot, I);
  displayInOrderNameTree(nameRoot);
  traverse_in_traverse(nameRoot);
  printf("\n");

  // queries(infile, nameRoot, Q);


  freeAll(nameRoot);

  fclose(infile);
  fclose(outfile);
}