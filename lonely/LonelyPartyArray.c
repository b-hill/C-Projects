
#include "LonelyPartyArray.h"
#include <stdio.h>
#include <stdlib.h>

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
  int capacity;

  // Fail cases
  if (num_fragments <= 0 || fragment_length <= 0)
    return NULL;

  // Allocate Space for party
  LonelyPartyArray *party = malloc(sizeof(LonelyPartyArray));
  if (party == NULL)
    return NULL;

  // Init struct data
  party->num_fragments = num_fragments;
  party->fragment_length = fragment_length;
  party->size = 0;
  party->num_active_fragments = 0;

  // Allocate NULL space for fragments
  party->fragments = calloc(num_fragments, sizeof(int *));
  if (party->fragments == NULL)
  {
    free(party);
    return NULL;
  }

  // Allocate NULL space for fragment_sizes
  party->fragment_sizes = calloc(num_fragments, sizeof(int));
  if (party->fragment_sizes == NULL)
  {
    free(party->fragments);
    free(party);
    return NULL;
  }

  // On success
  capacity = num_fragments * fragment_length;
  printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n",
         capacity, num_fragments);
  return party;
}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
  int i;

  // Check args
  if (party == NULL)
    return NULL;

  // Free party memory
  for (i = 0; i < party->num_fragments; i++)
    free(party->fragments[i]);
  free(party->fragments);
  free(party->fragment_sizes);
  free(party);

  // On success
  printf("-> The LonelyPartyArray has returned to the void.\n");
  return NULL;
}

LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
  int capacity, i, j;

  // Check args
  if (party == NULL)
    return NULL;

  // Allocate Space for party
  LonelyPartyArray *clone = malloc(sizeof(LonelyPartyArray));
  if (clone == NULL)
    return NULL;

  // Init clone data
  clone->num_fragments = party->num_fragments;
  clone->fragment_length = party->fragment_length;
  clone->size = party->size;
  clone->num_active_fragments = party->num_active_fragments;

  // Allocate NULL space for fragments
  clone->fragments = calloc(clone->num_fragments, sizeof(int *));
  if (clone->fragments == NULL)
  {
    free(clone);
    return NULL;
  }

  // Allocate NULL space for fragment_sizes
  clone->fragment_sizes = calloc(clone->num_fragments, sizeof(int));
  if (clone->fragment_sizes == NULL)
  {
    free(clone->fragments);
    free(clone);
    return NULL;
  }

  // Allocate space for fragment cells and set values
  for (i = 0; i < clone->num_fragments; i++)
  {
    if (party->fragments[i] != NULL)
    {
      clone->fragments[i] = malloc(clone->fragment_length * sizeof(int));
      if (clone->fragments[i] == NULL)
        return NULL;
      for (j = 0; j < clone->fragment_length; j++)
      {
        if (clone->fragments[i][j] != UNUSED)
          clone->fragments[i][j] = party->fragments[i][j];
        else
          clone->fragments[i][j] = UNUSED;
      }
    }
  }

  // On success
  capacity = party->num_fragments * party->fragment_length;
  printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n",
         capacity, clone->num_fragments);
  return clone;
}

int set(LonelyPartyArray *party, int index, int key)
{
  int indexLim, isNULL = 0, indexFrag, indexCell, indexStart, indexEnd, i;

  // Check args
  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in set().\n");
    return LPA_FAILURE;
  }

  // Configure indices
  indexLim = party->fragment_length * party->num_fragments - 1;
  indexFrag = index / party->fragment_length;
  indexCell = index % party->fragment_length;

  if (index > indexLim || index < 0)
  {
    printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n",
           index, indexFrag, indexCell);
    return LPA_FAILURE;
  }

  // Determine if the fragment is NULL
  if (party->fragments[indexFrag] == NULL)
    isNULL = 1;

  // Modify Cell Value
  if (isNULL)
  {
    // Allocate space + update active frag counter
    party->fragments[indexFrag] = malloc(party->fragment_length * sizeof(int));
    if (party->fragments[indexFrag] == NULL)
      return LPA_FAILURE;
    for (i = 0; i < party->fragment_length; i++)
      party->fragments[indexFrag][i] = UNUSED;
    party->num_active_fragments++;

    // Store key in cell + update counters
    party->fragments[indexFrag][indexCell] = key;
    party->fragment_sizes[indexFrag]++;
    party->size++;

    // On success
    indexStart = party->fragment_length * indexFrag;
    indexEnd = indexStart + party->fragment_length - 1;
    printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n",
           indexFrag, party->fragment_length, indexStart, indexEnd);
    return LPA_SUCCESS;
  }
  else
  {
    // Determine if the cell is UNUSED + update cell counter
    if (party->fragments[indexFrag][indexCell] == UNUSED)
    {
      party->fragment_sizes[indexFrag]++;
      party->size++;
    }

    // Store key in cell
    party->fragments[indexFrag][indexCell] = key;
    return LPA_SUCCESS;
  }
}

int get(LonelyPartyArray *party, int index)
{
  int indexLim, indexFrag, indexCell;

  // Check args
  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in get().\n");
    return LPA_FAILURE;
  }

  // Configure indices
  indexLim = party->fragment_length * party->num_fragments - 1;
  indexFrag = index / party->fragment_length;
  indexCell = index % party->fragment_length;

  if (index > indexLim || index < 0)
  {
    printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n",
           index, indexFrag, indexCell);
    return LPA_FAILURE;
  }

  // Determine if the fragment is NULL
  if (party->fragments[indexFrag] == NULL)
    return UNUSED;

  // Retreive cell value
  return party->fragments[indexFrag][indexCell];
}

int delete (LonelyPartyArray *party, int index)
{
  int indexLim, mustDeallocate = 0, indexFrag, indexCell, indexStart, indexEnd, i;

  // Check args
  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in delete().\n");
    return LPA_FAILURE;
  }

  // Configure indices
  indexLim = party->fragment_length * party->num_fragments - 1;
  indexFrag = index / party->fragment_length;
  indexCell = index % party->fragment_length;

  if (index > indexLim || index < 0)
  {
    printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n",
           index, indexFrag, indexCell);
    return LPA_FAILURE;
  }

  // Fail cases
  if (party->fragments[indexFrag] == NULL)
    return LPA_FAILURE;
  if (party->fragments[indexFrag][indexCell] == UNUSED)
    return LPA_FAILURE;

  // Change cell to UNUSED
  party->fragments[indexFrag][indexCell] = UNUSED;
  party->size--;
  party->fragment_sizes[indexFrag]--;

  // Deallocate if fragment is empty
  if (party->fragment_sizes[indexFrag] == 0)
  {
    free(party->fragments[indexFrag]);
    party->fragments[indexFrag] = NULL;
    party->num_active_fragments--;
    indexStart = party->fragment_length * indexFrag;
    indexEnd = indexStart + party->fragment_length - 1;
    printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n",
           indexFrag, party->fragment_length, indexStart, indexEnd);
  }

  return LPA_SUCCESS;
}

int containsKey(LonelyPartyArray *party, int key)
{
  int i, j;

  // Check args
  if (party == NULL)
    return 0;

  // Linear Search
  for (i = 0; i < party->num_fragments; i++)
  {
    if (party->fragments[i] == NULL)
      continue;
    else
      for (j = 0; j < party->fragment_length; j++)
        if (party->fragments[i][j] == key)
          return 1;
  }
  return 0;
}

int isSet(LonelyPartyArray *party, int index)
{
  int indexLim, indexFrag, indexCell, i;

  // Check args
  if (party == NULL)
    return 0;

  // Configure indices
  indexFrag = index / party->fragment_length;
  indexCell = index % party->fragment_length;
  indexLim = party->fragment_length * party->num_fragments - 1;

  // Cases
  if (index > indexLim || index < 0)
    return 0;
  else if (party->fragments[indexFrag] == NULL)
    return 0;
  else if (party->fragments[indexFrag][indexCell] == UNUSED)
    return 0;
  else
    return 1;
}

int printIfValid(LonelyPartyArray *party, int index)
{
  int indexLim, indexFrag, indexCell, value;

  // Check args
  if (party == NULL)
    return LPA_FAILURE;

  // Configure indices
  indexLim = party->fragment_length * party->num_fragments - 1;
  indexFrag = index / party->fragment_length;
  indexCell = index % party->fragment_length;

  // Fail cases
  if (index > indexLim || index < 0)
    return LPA_FAILURE;
  if (party->fragments[indexFrag] == NULL)
    return LPA_FAILURE;
  if (party->fragments[indexFrag][indexCell] == UNUSED)
    return LPA_FAILURE;

  // Print cell value
  value = party->fragments[indexFrag][indexCell];
  printf("%d\n", value);

  return LPA_SUCCESS;
}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
  int i;

  // Check args
  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
    return party;
  }

  // Clear party
  for (i = 0; i < party->num_fragments; i++)
  {
    if (party->fragments[i] != NULL)
      free(party->fragments[i]);
    party->fragments[i] = NULL;
  }
  party->size = 0;
  party->num_active_fragments = 0;

  // On success
  printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n",
         party->num_fragments * party->fragment_length, party->num_fragments);
  return party;
}

int getSize(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;
  else
    return party->size;
}

int getCapacity(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;
  else
    return party->num_fragments * party->fragment_length;
}

int getAllocatedCellCount(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;
  else
    return party->num_active_fragments * party->fragment_length;
}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int bytes;

  if (party == NULL)
    return 0;

  bytes = (long long unsigned int)(party->fragment_length * party->num_fragments) * sizeof(int);
  return bytes;
}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int bytes = 0;
  int i, j;

  // Check args
  if (party == NULL)
    return 0;

  // Add up bytes by case
  bytes += sizeof(*party);
  bytes += sizeof(party);
  for (i = 0; i < party->num_fragments; i++)
  {
    bytes += sizeof(party->fragments[i]);
    bytes += sizeof(party->fragment_sizes[i]);

    if (party->fragments[i] != NULL)
      for (j = 0; j < party->fragment_length; j++)
        bytes += sizeof(party->fragments[i][j]);
  }

  return bytes;
}
