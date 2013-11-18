/********************************************************************************
 * Name:
 * Purpose:
 *
 * Author:      crazylion
 *
 * Created:
 * Copyright:   (c) crazylion 2013
 * Licence:     <The MIT License>
 *******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "list.h"

/* list_init */
void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return;
}

/* list_destroy */
void list_destroy(List *list)
{
    void *data;

    while(list_size(list) > 0)
    {
        if(list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL)
        {
            list->destroy(data);
        }
    }

    memset(list, 0, sizeof(List));
    return;
}

/* list_ins_next */
int list_ins_next(List *list, ListElmt *element, const void *data)
{
    ListElmt *new_element;

    // Allocate storage for the element.
    if((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
    {
        return -1;
    }

    // Insert the element into the list.
    new_element->data = (void *)data;
    if(element == NULL)
    {
        // Handle insertion at the4 head of the list.
        if(list_size(list) == 0)
        {
            list->tail = new_element;
        }
        new_element->next = list->head;
        list->head = new_element;
    }
    else
    {
        // Handle insertion somewhere other than at the head.
        if(element->next == NULL)
        {
            list->tail = new_element;
        }

        new_element->next = element->next;
        element->ne = new_element;
    }

    list->size++;

    return 0;
}

/* list_rem_next */
int list_rem_next(List *list, ListElmt *element, void **data)
{
    ListElmt *old_element;

    // Do not allow removal from an empty list.
    if(list_size(list) == 0)
    {
        return -1;
    }

    // Remove the element from the list.
    if(element == NULL)
    {
        // Handle removal from the head of the list.
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if(list_size(list) == 1)
        {
            list->tail = NULL;
        }
    }
    else
    {
        // Handle removal from somewhere other than the head.
        if(element->next == NULL)
        {
            return -1;
        }

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if(element->next == NULL)
        {
            list->tail = element;
        }
    }

    // Free the sotrage allocated by the abstract datatype.
    free(old_element);

    // Adjust the size of the list to account for the removed element.
    list->size--;
    return 0;
}


