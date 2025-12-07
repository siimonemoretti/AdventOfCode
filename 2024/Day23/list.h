#ifndef _MY_LIST_H_
#define _MY_LIST_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
   int data;
   struct node *next;
} node_t;

void print_list(node_t *head)
{
   node_t *current = head;
   while (current != NULL)
   {
      printf("%d ", current->data);
      current = current->next;
   }
}

void push(node_t *head, int data)
{
   if (head == NULL)
   {
      printf("Error: head is NULL\n");
      return;
   }

   node_t *current = head;
   while (current->next != NULL)
   {
      current = current->next;
   }
   current->next = (node_t *)malloc(sizeof(node_t));
   current->next->data = data;
   current->next->next = NULL;
}

int search(node_t *head, int data)
{
   node_t *current = head;
   while (current != NULL)
   {
      if (current->data == data)
      {
         return 1;
      }
      current = current->next;
   }
   return 0;
}

#endif