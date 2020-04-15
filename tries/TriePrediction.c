
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"

TrieNode *insertString(TrieNode *root, char words[30][1024], int index, int last);
void cleanString(char *str);
void textPrediction(TrieNode *root, char *str, int n);
void handlePrintSingle(TrieNode *root, char *str);
int prefixCountHelper(TrieNode *root);
void getMostFrequentWordHelper(TrieNode *root, char *word, char *tmp, int *maxCount, int depth, int index);
void printTrieHelper(TrieNode *root, char *buffer, int k);
void printTrie(TrieNode *root, int useSubtrieFormatting);
TrieNode *createNode(void);
TrieNode *getNodeModified(TrieNode *root, char *str);

// Derived from Dr S notes
TrieNode *createNode(void)
{
	return calloc(1, sizeof(TrieNode));
} 

// Derived from Dr S notes
TrieNode *insertString(TrieNode *root, char words[30][1024], int index, int last)
{
	int i, letter, length1, length2;
	char *str, next[1024];
	char orig[1024];
	TrieNode *tmp, *sub;

	// Init root if it doesnt exist 
	if (root == NULL)
		root = createNode(); 

	// Init current
	str = words[index];

	// Check if word ends the sentence w/ period
	if (str[strlen(str)-1] == '.')
		// If so, set last to true to avoid false co-occurance 
		last = 1;

	// Remove punc and alter to for main string
	cleanString(str);
	length1 = strlen(str);

	// Move to terminal node
	tmp = root;
	for (i = 0; i < length1; i++)
	{
		// Get letter
		letter = str[i] - 'a';

		// Check for/Create upcoming node
		if (tmp->children[letter] == NULL)
			tmp->children[letter] = createNode();
			
		// Move forward 
		tmp = tmp->children[letter]; 
	}

	// Increment string count @node
	tmp->count++;

	// Insert Subtrie if applicable 
	if (!last)
	{
		// Get string (next is a char[] not char * bc we dont want to clean the next word before we 
		// know if it is the end of the sentence, this bug took over an hour to catch :( )
		strcpy(next, words[index+1]);

		// Format string
		cleanString(next);
		length2 = strlen(next);

		// Init subtrie if needed
		if (tmp->subtrie == NULL)
			tmp->subtrie = createNode();
		
		// Move to terminal node
		sub = tmp->subtrie;
		for (i = 0; i < length2; i++)
		{
			// Get letter
			letter = next[i] - 'a';
			
			// Check for/Create upcoming node
			if (sub->children[letter] == NULL)
				sub->children[letter] = createNode();
			
			// Move forward if possible
			sub = sub->children[letter]; 		
		}
		
		// Increment string count @node
		sub->count++;
	} 

	return root;
}

// Removes punctuation and uppercase letters
void cleanString(char *str)
{
	char *tmp = calloc(1,sizeof(str));
	int i;
	int length = strlen(str);
	int j = 0;

	// Loop through input string (j is index of tmp string)
	for (i = 0; i < length; i++)
	{
		// Skip if the char is punctuation/number/special 
		if (!isalpha(str[i]))
			continue; 

		// Lower if upper
		else if (isupper(str[i]))
			tmp[j] = tolower(str[i]);

		// Otherwise copy over 
		else 
			tmp[j] = str[i];
			
		j++;
	}
	// Replace str w/ tmp
	strcpy(str, tmp);

	// Free tmp
	free(tmp);
}

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;
		
	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

// this is beautiful, pretty proud ngl
void textPrediction(TrieNode *root, char *str, int n)
{
	TrieNode *termNode, *subRoot;
	char subStr[1024];
	int i;
	
	if (root == NULL)
		return;

	// Print first word always
	printf("%s", str);

	// Prep word for search in trie (every word after will already be formatted)
	cleanString(str);

	// Prediction 
	for(i = 0; i < n; i++)
	{
		// Check terminal node and cancel if required then check if the terminal node has a subtrie 
		if(((termNode = getNode(root, str)) == NULL) || ((subRoot = termNode->subtrie) == NULL))
			return;
		
		// Get most frequent word in str's subtrie for prediction then print 
		getMostFrequentWord(subRoot, subStr);
		printf(" %s", subStr); // leading space, important 		

		// Set subtrie word to main word
		strcpy(str, subStr);
	}
}

// Print word and get subtrie words
void handlePrintSingle(TrieNode *root, char *str)
{
	TrieNode *termNode;
	char orig[1024];

	// Save unformatted string 
	strcpy(orig, str);

	// Format string
	cleanString(str);

	// Str not in trie
	if ((termNode = getNode(root, str)) == NULL)
	{
		printf("%s\n(INVALID STRING)\n", str);
		return;
	}

	// Just no subtrie
	else if ((termNode->subtrie) == NULL)
	{
		printf("%s\n(EMPTY)\n", str);
		return;
	}		

	// Default
	printf("%s\n", orig);
	printTrie(termNode->subtrie, 1);
}

// Derived from Dr. S notes
TrieNode *buildTrie(char *filename)
{
	TrieNode *root = NULL;
	FILE *file; 
	char buffer[1024];
	char words[1024][1024]; //lol
	int i;
	int numWords = 0;

	// Check for failed file open
	if ((file = fopen(filename, "r")) == NULL) 
	{
		fprintf(stderr, "Failed to open \"%s\" in buildTrie().\n", filename);
		return NULL;
	}

	// Init 
	root = createNode();

	// Get words
	while(fscanf(file, "%s", buffer) != EOF)
	{
		strcpy(words[numWords], buffer);
		numWords++;
	}

	// Build Trie + subTries
	for (i = 0; i < numWords; i++)
		// Insert string to trie 
		if ((i+1) != numWords)
			root = insertString(root, words, i, 0);
			
		// Edge case for last word in array
		else 
			root = insertString(root, words, i, 1);			

	fclose(file);
	return root;
}

// This is based off my listy string processInputFile function 
int processInputFile(TrieNode *root, char *filename)
{
	char command[1024], str[1024];
	int num; 

	// Check args
	FILE *file;
	if ((file = fopen(filename, "r")) == NULL)
		return 1;

	// Iterate through file commands 
	while (fscanf(file, "%s", command) != EOF)
	{
		// Switch board
		switch (command[0])
		{
			// Text Prediction 
			case '@':
				fscanf(file, "%s%d", str, &num);
				textPrediction(root, str, num);
				printf("\n");
				break;

			// Print trie
			case '!':
				printTrie(root, 0);
				break;

			// Find word and subtrie
			default: 
				handlePrintSingle(root, command);
				break;
		}
	}
	fclose(file);
	return 0;
}

// Kill it with fire
TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	// brackets are ugly
	for(i = 0; i < 26; i++)
		if(root->children[i] != NULL)
			destroyTrie(root->children[i]);
	if (root->subtrie != NULL)
		destroyTrie(root->subtrie);

	// Avada Kedavra	
	free(root);

	return NULL;
}

// Recursive
TrieNode *getNode(TrieNode *root, char *str) 
{
	int letter;

	// Base case
	if(str[0] == '\0') 
		if (root->count >= 1)
			return root;
		else
			return NULL;
			
	// Update letter
	letter = str[0] - 'a';

	// Recursive call 
	if (root->children[letter] == NULL)
		return NULL;	
	else
		return getNode(root->children[letter], str+1);

}

// This pair of fucntions is similar to the printTries functions except 
// rather than printing the words when a node with a count is found,
// this function only keeps track of one word
void getMostFrequentWord(TrieNode *root, char *str)
{
	int depth = 0;         // Index of current letter of temp char array
	int maxCount = 0; 	  // Current highest frequency
	char tmp[1024];      // Updated with each recursive call
	char word[1024];    // Only updated when new maxCount is found
	int i;
	// im so sorry
	char alphabet[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	// Edge case
	if (root == NULL)
	{
		strcpy(str, "");
		return;
	}

	for (i = 0; i < 26; i++)
		getMostFrequentWordHelper(root->children[i], word, tmp, &maxCount, depth, alphabet[i]);			

	// Update most frequent word
	strcpy(str, word);
}

// This thing is wild
void getMostFrequentWordHelper(TrieNode *root, char *word, char *tmp, int *maxCount, int depth, int index)
{
	int i; 
	char alphabet[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	// Base Case
	if (root == NULL)
		return;

	// Update tmp char array
	// I HAVE NO IDEA WHY THIS IS LIKE THIS BUT IT IS
	if(depth == 0)
		tmp[depth] = alphabet[index-1];
	else
		tmp[depth] = alphabet[index];
	tmp[depth+1] = '\0';

	// Check count, replace only if new maxCount is found
	if (root->count > *maxCount)
	{
		strcpy(word, tmp);
		*maxCount = root->count; 
	}

	// If count and maxcount are equal, check alphabetical order
	if (root->count == *maxCount)
		// If tmp comes before word alphabetically, update word
		if (strcmp(word, tmp) > 0)
			strcpy(word, tmp);

	// Check children 
	for (i = 0; i < 26; i++)
		getMostFrequentWordHelper(root->children[i], word, tmp, maxCount, depth+1, i);
}

// EZPZ
int containsWord(TrieNode *root, char *str)
{
	if(getNode(root, str) == NULL)
		return 0;
	else 
		return 1; 
}

int prefixCount(TrieNode *root, char *str)
{
	int count; 
	TrieNode *prefix;

	// Edge case where prefix is empty string i.e. count all words in trie
	if(strcmp(str, "") == 0)
		return prefixCountHelper(root);
	
	// Check if prefix exists
	if ((prefix = getNodeModified(root, str)) == NULL)
		return 0; 
		
	// Search children (root included in recursive call)
	return prefixCountHelper(prefix);
}

int prefixCountHelper(TrieNode *root)
{
	int count = 0;
	int i; 

	// Base case
	if (root == NULL)
		return 0; 

	count += root->count;

	for(i = 0; i < 26; i++)
		count += prefixCountHelper(root->children[i]);

	return count;	
}

// Literally the same thing as getNode but doesnt return null if count is zero at terminal node
// This is used for prefixCount
TrieNode *getNodeModified(TrieNode *root, char *str)
{
	int letter;

	// Base case
	if(str[0] == '\0') 
		return root;
			
	// Update letter
	letter = str[0] - 'a';

	// Recursive call 
	if (root->children[letter] == NULL)
		return NULL;	
	else
		return getNodeModified(root->children[letter], str+1);
}

int main(int argc, char **argv)
{
	TrieNode *trie = NULL;

	// Build the trie for the corpus
	trie = buildTrie(argv[1]);

	// Process input file
	processInputFile(trie, argv[2]);

	// Destroy the trie 
	trie = destroyTrie(trie);

	return 0;
}
