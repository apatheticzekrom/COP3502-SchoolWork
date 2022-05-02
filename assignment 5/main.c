/* COP 3502C Programming Assignment 5
This program is written by: Benjamin Giang */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	"leak_detector_c.h"

struct trie
{
  int isWord; //0/1
  int used;
  struct trie* next[26];
};

// Create the struct, not a word.
struct trie* init() 
{ 
  struct trie* myTree = malloc(sizeof(struct trie));    myTree->isWord = 0; // Set each pointer to NULLL.
  myTree->used = 0; // I think this works
  int i;   
  for (i=0; i<26; i++)       
  myTree->next[i] = NULL; // Return a pointer to the new root.
  return myTree;
}

void insert(struct trie* tree, char word[], int k)
{ 
// Down to the end, insert the word.

  tree->used = tree->used++; // I think this works

  if (k == strlen(word))
  {
    tree->isWord = 1;
    return;
  }
  // See if the next place to go exists, if not, create it.
  int nextIndex = word[k] - 'a';
  if (tree->next[nextIndex] == NULL)
  {
    tree->next[nextIndex] = init();
  }
  insert(tree->next[nextIndex], word, k+1);
}

// Prints all words stored in the trie in alphabetical order.
void printAll(struct trie* tree, char cur[]) {
  // Stop!
  if (tree == NULL) return;
  // Print this node, if it's a word.
  if (tree->isWord)
  {
  printf("%s", cur);// Safer if we store this.
  printf("\n");
  }
  int len = strlen(cur);
  // Recursively print all words in each subtree,
  // in alpha order.
  int i;
  for (i=0; i<26; i++)
  {
    cur[len] = (char)('a'+i);
    cur[len+1] = '\0';
    printAll(tree->next[i], cur);
    
  }
}


// Recursive search from level k of the trie for word.
int searchRec(struct trie* tree, char word[], int k, int wordlen) {

    // Down to the end, insert the word.
    if (k == wordlen)
        return tree->isWord;

    // If the next place doesn't exist, word is not a word.
    int nextIndex = word[k] - 'a';
    if (tree->next[nextIndex] == NULL)
        return 0;

    return searchRec(tree->next[nextIndex], word, k+1, wordlen);
}

// Wrapper function returns 1 iff word is in the trie pointed to by tree.
int search(struct trie* tree, char word[]) {
    return searchRec(tree, word, 0, strlen(word));
}


// Just frees all the memory pointed to by tree (directly and indirectly)
void freeDictionary(struct trie* tree) {

    int i;
    for (i=0; i<26; i++)
        if (tree->next[i] != NULL)
            freeDictionary(tree->next[i]);

    free(tree);
}


int main(void) {

  struct trie* myDictionary = init();

  FILE *infile;
  infile = fopen("in.txt", "r");

  int n;
  fscanf(infile, "%d", &n);

  // Reads file
  for (int i=0; i<n; i++)
  {
    int query;
    fscanf(infile, "%d", &query);

    // Read in each word and insert it.
    if(query == 1)
    {
      char word[100];
      int count;
      fscanf(infile, "%s %d", word, &count);
      for(int j=0; j<count; j++)
      {
        insert(myDictionary, word, 0);
      }
    }

    if(query == 2)
    {
      char p[100];
      fscanf(infile, "%s", p);

      // // Write function if next = null then print unknown word, 
      // if(search(myDictionary, p))
      // {
      //   printf("unknown word\n");
      // }
    
    }
  }

  
  // //Printing all the words in alphabetical order
  // printf("Printing all the words: \n");
  // char word[100];
  // word[0] = '\0';
  // printAll(myDictionary, word);

  freeDictionary(myDictionary);
  fclose(infile);
}
