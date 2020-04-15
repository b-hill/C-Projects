
#include "ListyString.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv)
{
  processInputFile(argv[1]);
  return 0;
}

int processInputFile(char *filename)
{
  ListyString *listy;
  char command[1028], key[1], start[1024], str[1024];
  int len;

  // Read in file
  FILE *file;
  file = fopen(filename, "r");

  // Check args
  if (file == NULL)
    return 1;

  // Create listy string
  fscanf(file, "%s", start);
  listy = createListyString(start);

  // Iterate through file commands
  while (fscanf(file, "%s", command) != EOF)
  {
    // Switch board
    switch (command[0])
    {
      case '@':
        fscanf(file, "%s%s", key, str);
        replaceChar(listy, key[0], str);
        break;

      case '+':
        fscanf(file, "%s", str);
        listyCat(listy, str);
        break;

      case '-':
        fscanf(file, "%s", key);
        replaceChar(listy, key[0], "");
        break;

      case '~':
        reverseListyString(listy);
        break;

      case '?':
        len = listyLength(listy);
        printf("%d\n", len);
        break;

      case '!':
        printListyString(listy);
        break;
    }
  }

  fclose(file);

  return 0;
}

ListyString *createListyString(char *str)
{
  char c;
  int i, len;

  // Allocate space
  ListyString *listy = calloc(1, sizeof(ListyString));

  // Check args
  if (str == NULL || strcmp(str, "") == 0)
    return listy;

  // Add nodes 
  ListyNode *newNode = malloc(sizeof(ListyNode));
  ListyNode *head;
  newNode->data = str[0];
  newNode->next = NULL;
  listy->head = newNode; 
  head = listy->head;
  len = strlen(str);
  for(i=1; i<len; i++)
  {
    // Init node
    ListyNode *newNode = malloc(sizeof(ListyNode));
    newNode->data = str[i];
    newNode->next = NULL;
    // Adjust pointers
    head->next = newNode;
    head = head->next;
  }

  listy->length = len;

  return listy;
}

ListyString *destroyListyString(ListyString *listy)
{
  // Check args
  if (listy == NULL || listy->head == NULL)
    return NULL;

  ListyNode *current = listy->head;
  ListyNode *temp;

  // Free each node
  while (current != NULL)
  {
    temp = current->next;
    free(current);
    current = temp;
  }

  // Free the listy string
  free(listy);

  return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
  // Check args
  if (listy == NULL)
    return NULL;

  // Allocate space
  ListyString *listyClone = calloc(1, sizeof(ListyString));

  // Set up first node of clone and break if needed
  ListyNode *head = listy->head;
  if (head == NULL)
    return listyClone;
  ListyNode *headClone = malloc(sizeof(ListyNode));
  headClone->data = head->data;
  headClone->next = NULL;
  listyClone->head = headClone;
  head = head->next;

  // Copy listy to listyClone
  while (head != NULL)
  {
    // Init node
    ListyNode *newNode = malloc(sizeof(ListyNode));
    newNode->data = head->data;
    newNode->next = NULL;
    // Adjust pointers
    headClone->next = newNode;
    headClone = headClone->next;
    head = head->next;
  }
  return listyClone;
}

void replaceChar(ListyString *listy, char key, char *str)
{
  // Check args
  if (listy == NULL || listy->head == NULL)
    return;

  ListyNode *current = listy->head;
  ListyNode *prev = current;
  ListyNode *temp;
  int counter = 1, newFirst, deletion;

  // Remove nodes
  if (str[0] == '\0' || str == NULL)
  {    // Loop through list
    while (current != NULL)
    {
      newFirst = 0;
      deletion= 0;
      // On key == data
      if (current->data == key)
      {
        // Exception for key being head node
        if (listy->head == current)
        {
          // Assign new head pointer
          listy->head = listy->head->next;
          free(current);
          current = listy->head;
          newFirst = 1;
        }
        else
        {
          // "Skip" current node pointer
          prev->next = current->next;
          // Update total length
          listy->length--;
          deletion = 1;
        }
      }
    
      if (!deletion)
        // Adjust prev
        prev = current;
      
      if (!newFirst)
        // Move to next node
        current = current->next;

    }
  }
  // Replace nodes
  else
  {
    // Loop through list
    while (current != NULL)
    {
      // On key == data
      if (current->data == key)
      {
        // Save location of next node
        temp = current->next;

        // Replace data in starting node
        current->data = str[0];

        // Add nodes if neccesary
        while (str[counter] != '\0')
        {
          ListyNode *newNode = malloc(sizeof(ListyNode));
          current->next = newNode;
          newNode->data = str[counter];
          newNode->next = temp;
          current = newNode;
          counter++;
          listy->length++;
        }
      }

      counter = 1; 

      // Move to next node
      current = current->next;
    }
  }
}

void reverseListyString(ListyString *listy)
{
  // Check args
  if (listy == NULL || listy->head == NULL)
    return;

  ListyNode *current = listy->head;
  ListyNode *prev = NULL;
  ListyNode *next;

  // Iterate down list
  while (current != NULL)
  {
    // Swap linked list "direction" around curent node
    next = current->next;
    current->next = prev;
    // Swap pointers
    prev = current;
    current = next;
  }

  // Update listy head
  listy->head = prev;
}

ListyString *listyCat(ListyString *listy, char *str)
{
  int i, len, start=0;

  // Check args
  if (listy == NULL && str == NULL)
    return NULL;
  if(str == NULL)
    return listy;

  // Declare new listy string
  if (listy == NULL)
  {
    ListyString *listy = calloc(1, sizeof(ListyString));
  }

  // Return early if needed
  if (listy == NULL && strcmp(str, "") == 0)
    return listy;

  // Get to tail or make head 
  ListyNode *head = listy->head;
  if (head != NULL)
    while (head->next != NULL)
      head = head->next;
  else 
  {
    // Init node
    ListyNode *newNode = malloc(sizeof(ListyNode));
    newNode->data = str[0];
    newNode->next = NULL;
    // Adjust pointers
    listy->head = newNode;
    head = newNode;
    start++;
  }

  // Additional nodes
  len = strlen(str);
  for(i=start; i<len; i++)
  {
    // Init node
    ListyNode *newNode = malloc(sizeof(ListyNode));
    newNode->data = str[i];
    newNode->next = NULL;
    // Adjust pointers
    head->next = newNode;
    head = head->next;
  }

  // Update 
  listy->length += len; 

  return listy;
}

int listyCmp(ListyString *listy1, ListyString *listy2)
{
  // Edge cases
  if (listy1 == NULL && listy2 == NULL)
    return 0;
  if (listy1 == NULL || listy2 == NULL)
    return 1;
  if (listy1->length != listy2->length)
    return 1;
  if (listy1->head == NULL && listy2->head == NULL)
    return 0;

  // Iterate through both lists
  ListyNode *head1 = listy1->head;
  ListyNode *head2 = listy2->head;
  while (1)
  {
    // Break on deviation
    if (head1->data != head2->data)
      return 1;

    // Reach the end of the lists and still equal
    if (head1->next == NULL && head1->next == NULL)
      return 0;

    // Move to next nodes
    head1 = head1->next;
    head2 = head2->next;
  }
}

int listyLength(ListyString *listy)
{
  // Check args
  if (listy == NULL)
    return -1;

  return listy->length;
}

void printListyString(ListyString *listy)
{
  // Check args
  if (listy == NULL || listy->head == NULL)
  {
    printf("(empty string)\n");
    return;
  }

  // Iterate down list
  ListyNode *head = listy->head;
  while (head != NULL)
  {
    printf("%c", head->data);
    head = head->next;
  }
  printf("\n");
}