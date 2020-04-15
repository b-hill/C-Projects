// Note: for use of a regualr linked list, node->next is defaulted to to node->left

#include "KindredSpirits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node *new_node(int data);
node *clone(node *root);
node *reflect(node *root);
node *LL_insert(node *root, int data);
node *LL_reverse(node *str);
int LL_cmp(node *a, node *b);
node *inorder(node *root, node *str);
node *preorder(node *root, node *str);
node *postorder(node *root, node *str);

// Primary Functions:
// One tree is a reflection of the other if one inorder transversal in the reverse of the other
int isReflection(node *a, node *b)
{
  // Check args
  if (a == NULL && b == NULL)
    return 1;
  if (a == NULL || b == NULL)
    return 0;

  // Init transversals
  node *transversal_a = calloc(1, sizeof(node));
  node *transversal_b = calloc(1, sizeof(node));

  // Append transversal strings + skip first node bc its always 0 and idk how to work around that
  transversal_a = inorder(a, transversal_a);
  transversal_a = transversal_a->left;
  transversal_b = inorder(b, transversal_b);
  transversal_b = transversal_b->left;

  // Reverse one transversal
  transversal_b = LL_reverse(transversal_b);

  // Compare transversals, if in(a) == in(reversed b) -> reflections
  return LL_cmp(transversal_a, transversal_b);
}

// Clone on tree and then mirror the clone, both recursive 
node *makeReflection(node *root)
{
  // Check args
  if (root == NULL)
    return root;

  node *new_root = NULL;

  // Clone tree
  new_root = clone(root);

  // Mirror the clone
  new_root = reflect(new_root);

  // Mirrored clone ezpz
  return new_root;
}

// If pre(a) == post(b) or vice versa
int kindredSpirits(node *a, node *b)
{
  // Check args
  if (a == NULL && b == NULL)
    return 1;
  if (a == NULL || b == NULL)
    return 0;

  // Init transversal LLs:
  node *transversal_a = calloc(1, sizeof(node));
  node *transversal_b = calloc(1, sizeof(node));
  node *transversal_c = calloc(1, sizeof(node));
  node *transversal_d = calloc(1, sizeof(node));

  // Append transversal strings + skip first node bc its always 0 and idk how to work around that
  transversal_a = preorder(a, transversal_a);
  transversal_a = transversal_a->left;            // Preorder of a
  transversal_b = postorder(b, transversal_b);
  transversal_b = transversal_b->left;            // Post order of b
  transversal_c = postorder(a, transversal_c);
  transversal_c = transversal_c->left;            // Post order of a
  transversal_d = preorder(b, transversal_d);
  transversal_d = transversal_d->left;            // Pre order of b

  // Compare transversals 
  // pre(a) == post(b) => kindred spirits (or vice versa)
  return (LL_cmp(transversal_a, transversal_b) || LL_cmp(transversal_c, transversal_d));
}

// Secondary functions:
// Creates new node, left and right pointers default to null via calloc
node *new_node(int data)
{
  node *new = calloc(1, sizeof(node));
  new->data = data;
  return new;
}

// Create copy of binary tree, recursive
node *clone(node *root)
{
  if (root == NULL)
    return root;

  node *new = calloc(1, sizeof(node));

  // Copy all info + work down trees
  new->data = root->data;
  new->left = clone(root->left);
  new->right = clone(root->right);

  return new;
}

// Makes reflection, Similar to a swap function for array elements but with pointers + recursion
node *reflect(node *root)
{
  if (root == NULL)
    return NULL;

  node *tmp;

  // Call down subtrees 
  reflect(root->left);
  reflect(root->right);
  // Copy data
  tmp = root->left;
  root->left = root->right;
  root->right = tmp;
}

// Insert node into basic LL for transversals, recursive
node *LL_insert(node *root, int data)
{
  if (root == NULL)
    return new_node(data);

  root->left = LL_insert(root->left, data);
  return root;
}

// Reverse basic LL for transversals
node *LL_reverse(node *str)
{
  // Check args
  if (str == NULL)
    return NULL;

  node *current = str;
  node *prev = NULL;
  node *next;

  while (current != NULL)
  {
    next = current->left;
    current->left = prev;
    prev = current;
    current = next;
  }

  // Change head location
  return prev;
}

// Comapre two basic LLs for transversals
// return 1 if they are the same, 0  otherwise
int LL_cmp(node *a, node *b)
{
  // Check args
  if (a == NULL && b == NULL)
    return 1;
  if (a == NULL || b == NULL)
    return 0;

  // Loop until loop breaks
  while (1)
  {
    // Check for deviation
    if (a->data != b->data)
      return 0;
    if (a->left == NULL && b->left == NULL)
      return 1;
    if (a->left == NULL || b->left == NULL)
      return 0;

    // Move on
    a = a->left;
    b = b->left;
  }
}

// Inorder transversal, recursive
node *inorder(node *root, node *str)
{
  if (root == NULL)
    return NULL;

  // Left => Middle => Right
  inorder(root->left, str);
  str = LL_insert(str, root->data);
  inorder(root->right, str);

  return str;
}

// Preorder transversal, recursive
node *preorder(node *root, node *str)
{
  if (root == NULL)
    return NULL;

  // Middle => Left => Right
  str = LL_insert(str, root->data);
  preorder(root->left, str);
  preorder(root->right, str);

  return str;
}

// Postorder transversal, recursive
node *postorder(node *root, node *str)
{
  if (root == NULL)
    return NULL;

  // Left => Right => Middle
  postorder(root->left, str);
  postorder(root->right, str);
  str = LL_insert(str, root->data);

  return str;
}
