#include <stdio.h>
#include <string.h>
//#include <pthread.h>
//#include <unistd.h>
#include <stdlib.h>
#include "packer.h"
#include "linkedList.h"
PData_t *gPData;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

void *mallocAndCheck(size_t size)
{
    void *ret = malloc(size);
    if(ret == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return ret;
}

// iterating over th elinked list
int searchFunc(char *funcName)
{
    PData_t *head = gPData;
    PData_t *current =  head;
    while(current != NULL)
    {
        printf("SEARCH FUNC NAMES %s %s\n", funcName, current->funcName);
        if(strcmp(current->funcName, funcName) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void pushSim(PData_t **head, char *data)
{
    if(*head == NULL)
    {
        printf("PUSHING TO EMPTY\n");
    }
    while(*head != NULL)
    {
        head = &(*head)->next;
    }

    *head = (PData_t*) mallocAndCheck(sizeof (PData_t));

    /*if(*head == NULL)
    {
        fprintf(stderr, "malloc error\n");
        return;
    }*/

    size_t size;
    memcpy(&size, data, sizeof (size));
    (*head)->size = size;
    printf("size from push %ld\n", size);
    //(*head)->funcName = funcName;
    //printf("func name from push %s\n", funcName);
    (*head)->in = (char*) mallocAndCheck(size);
    memcpy((*head)->in, data, size);
    //
    //(*head)->nIns = noIn;
    //(*current)->nInsFilled = nov;
    (*head)->next = NULL;
    printf("Data pushed to SIM\n");
}

// Adding item to the end of the list
void pushToEnd(char *funcName, char noIn, char nov, char *data)
{
    PData_t **current =  &gPData;
    if(*current == NULL)
    {
        printf("PUSHING TO EMPTY\n");
    }
    while(*current != NULL)
    {
        current = &(*current)->next;
    }

    *current = (PData_t*) mallocAndCheck(sizeof (PData_t));

    /*if(*current == NULL)
    {
        fprintf(stderr, "malloc error\n");
        return;
    }*/

    size_t size;
    memcpy(&size, data, sizeof (size));
    (*current)->size = size;
    printf("size from push %ld\n", size);
    (*current)->funcName = (char*)mallocAndCheck(strlen(funcName) + 1);
    memcpy((*current)->funcName, funcName, strlen(funcName) + 1);
    printf("func name from push %s\n", funcName);
    (*current)->in = (char*) mallocAndCheck(size);
    memcpy((*current)->in, data, size);
    //
    (*current)->nIns = noIn;
    (*current)->nInsFilled = nov;
    (*current)->next = NULL;
    printf("Data pushed\n");
}

void appendData(char *funcName, char noIn, char nov, char *data)
{
    PData_t **current = &gPData;
    while(*current != NULL)
    {
        if(strcmp((*current)->funcName, funcName) == 0)
        {
            break;
        }
        current = &(*current)->next;
    }

    if(*current == NULL)
    {
        printf("UNKNOWN CONDITION IN APPEND DATA\n");
        exit(0);
    }


    size_t tot_size_init;
    size_t tot_size_final;
    size_t tot_size_data;
    size_t add_size = 0;

    deleteData(-1, &data);

    memcpy(&tot_size_data, data + add_size, sizeof (tot_size_data));
    memcpy(&tot_size_init, (*current)->in + add_size, sizeof (tot_size_init));

    printf("TOTAL SIZE BEFORE APPEND %ld\n", tot_size_data);
    printf("TOTAL SIZE of NEW DATA %ld\n", tot_size_init);

    tot_size_data -= (sizeof (tot_size_data) + sizeof (char));

    tot_size_final =  tot_size_init + tot_size_data;

    printf("TOTAL SIZE FINAL %ld\n", tot_size_final);

    char *newC = realloc((*current)->in, tot_size_final);
    //*current = realloc(*current, tot_size_final);
    if(!newC)
    {
        printf("ERROR REALLOC\n");
        exit(0);
    }
    (*current)->in = newC;

    (*current)->size = tot_size_final;

    memcpy((*current)->in + add_size, &tot_size_final, sizeof (tot_size_final));

    add_size += sizeof (tot_size_final);

    char nv;
    memcpy(&nv, (*current)->in + add_size, sizeof (nv));

    char nv_in;
    memcpy(&nv_in, data + add_size, sizeof (nv_in));

    printf("NV old %d NV NEW %d\n", nv, nv_in);

    nv += nv_in;

    printf("NV Totl %d\n", nv);
    memcpy((*current)->in + add_size, &nv, sizeof (nv));
    //add_size += sizeof (nv);
    //add_size += tot_size_init;

    memcpy((*current)->in + tot_size_init, data + (sizeof (tot_size_data) + sizeof (char)), tot_size_data);
    (*current)->nInsFilled += nov;

}

PData_t **findPosOfLastFilled()
{
    PData_t **current =  &gPData;
    while((*current) != NULL)
    {
        if((*current)->nInsFilled < (*current)->nIns)
        {
            break;
        }
        (*current) = (*current)->next;
    }

    return current;
}

void localProcedureCall(char *funcName, char noIn, char nov, char *data)
{
    printf("\nTRYING TO LOCK MUTEX %s\n", __FUNCTION__);
    while(pthread_mutex_trylock(&lock))
    {
        printf("\rTRYING TO LOCK MUTEX %s", __FUNCTION__);
    }
    printf("\nLOCKED MUTEX %s\n", __FUNCTION__);

    int res = searchFunc(funcName);

    if(res)
    {
        printf ("FOUND THE FUNC ENTRY %s IN LINK LIST\n", funcName);
        appendData(funcName, noIn, nov, data);
    }
    else
    {
        printf ("COULD NOT FIND THE FUNC ENTRY %s IN LINK LIST\n", funcName);
        pushToEnd(funcName, noIn, nov, data);
    }

    pthread_mutex_unlock(&lock);
}

// Adding item to the beginning of the list
void pushToBegin(PData_t **head, char *data)
{
    //PData_t **head = &gPData;

    PData_t *new_node;
    new_node = mallocAndCheck(sizeof (PData_t));
    /*if(new_node == NULL)
    {
        fprintf(stderr, "Malloc error!\n");
        return;
    }*/
    new_node->next = NULL;
    new_node->next = (*head);
    size_t size;
    memcpy(&size, data, sizeof (size));
    new_node->size = size;
    printf("size from push %ld\n", size);
    new_node->in = (char *) mallocAndCheck(size);
    /*if(new_node->in == NULL)
    {
        fprintf(stderr, "Malloc error!\n");
        return;
    }*/
    memcpy(new_node->in, data, size);

    new_node->nIns = new_node->nInsFilled = 1;
    (*head) = new_node;
    //printf("HHHHHHHHHHHH %s\n", gPData->funcName);
    printf("Data pushed\n");
}

void pushAfter(PData_t *prev_node)
{
    if(prev_node == NULL)
    {
        fprintf(stderr, "Previous node must not be NULL\n");
        return;
    }

    PData_t *new_node = mallocAndCheck(sizeof (PData_t));
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

int isFirstListFilled(PData_t *head)
{

    if(head == NULL)
    {
        return 0;
    }

    if(head->nIns > 0 && head->nInsFilled > 0 && head->nIns == head->nInsFilled)
    {
        return 1;
    }
    else {
        return 0;
    }
}

size_t sizeOfFirstList(void)
{
    if(gPData == NULL)
    {
        return 0;
    }

    return gPData->size;
}

int popFirst(char **data)
{
    PData_t **head = &gPData;

    if (*head == NULL) {
        //fprintf(stderr, "head is NULL\n");
        *data = NULL;
        return -1;
    }
    *data = (char *) mallocAndCheck((*head)->size);
    printf("sizeee %ld\n", (*head)->size);
    /*if(*data == NULL)
    {
        fprintf(stderr, "malloc error\n");
        return -1;
    }*/
    memcpy(*data, (*head)->in, (*head)->size);
    PData_t * next_node = (*head)->next;

    if((*head)->funcName != NULL)
    {
        free((*head)->funcName);
    }

    if((*head)->in != NULL)
    {
        free((*head)->in);
    }

    if(*head != NULL)
    {
        free (*head);
    }

    *head = next_node;
    return 1;
}

int popFilled(char **data)
{
    while(pthread_mutex_trylock(&lock))
    {
        printf("\rTRYING TO LOCK MUTEX %s", __FUNCTION__);
    }

    PData_t **current =  &gPData;
    if (*current == NULL)
    {
        *data = NULL;

        pthread_mutex_unlock(&lock);
        return -1;
    }
    else
    {
        printf("Head has data %s\n\n\n\n", (*current)->funcName);
    }

    PData_t **prev = NULL;
    int flagFilled = 0;

    while((*current) != NULL)
    {
        if((*current)->nInsFilled == (*current)->nIns)
        {
            printf("Filled %d %d\n", (*current)->nInsFilled, (*current)->nIns);
            printf("DATA IS FILLED in %s\n", (*current)->funcName);
            flagFilled = 1;
            break;
        }
        printf("DATA IS NOT FILLED in %s\n", (*current)->funcName);
        prev = current;
        current = &(*current)->next;
    }

    if(!flagFilled)
    {
        *data = NULL;

        pthread_mutex_unlock(&lock);
        return -1;
    }

    if ((*current) == NULL) {
        printf("SOMETHING WENT WRONG\n");
        *data = NULL;

        pthread_mutex_unlock(&lock);
        return -1;
    }

    if(prev == NULL)
    {
        printf("POPING FIRST for %s\n", (*current)->funcName);
        popFirst(data);
    }
    else
    {
        printf("POPING FILLED for %s\n", (*current)->funcName);
        //printf("ADDRESS SHOULD BE SAME %ld %ld\n", prev->next, (*current));
        (*prev)->next = (*current)->next;
        *data = (char *) mallocAndCheck((*current)->size);
        /*if(*data == NULL)
        {
            fprintf(stderr, "malloc error\n");
            return -1;
        }*/
        memcpy(*data, (*current)->in, (*current)->size);
        if((*current)->funcName != NULL)
        {
            free((*current)->funcName);
        }
        if((*current)->in != NULL)
        {
            free((*current)->in);
        }
        if(*current != NULL)
        {
            free(*current);
            *current = NULL;
        }
    }

    pthread_mutex_unlock(&lock);
    return 0;
}
