#ifndef __LISTY_STRING_H
#define __LISTY_STRING_H

// Struct Definitions

typedef struct ListyNode
{
	// Each node holds a single character.
	char data;

	// Pointer to next node in linked list.
	struct ListyNode *next;
} ListyNode;


typedef struct ListyString
{
	// Head of the linked list representing this string.
	ListyNode *head;

	// Length of this linked list.
	int length;
} ListyString;


// Functional Prototypes

int processInputFile(char *filename);

ListyString *createListyString(char *str);

ListyString *destroyListyString(ListyString *listy);

ListyString *cloneListyString(ListyString *listy);

void replaceChar(ListyString *listy, char key, char *str);

void reverseListyString(ListyString *listy);

ListyString *listyCat(ListyString *listy, char *str);

int listyCmp(ListyString *listy1, ListyString *listy2);

int listyLength(ListyString *listy);

void printListyString(ListyString *listy);


#endif
