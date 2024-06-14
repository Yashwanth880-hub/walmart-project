#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define HEAP_SIZE 1024
char heap[HEAP_SIZE];
typedef struct meta_block
{
    int size_of_memory ;
    int free ; 
    struct meta_block * next ;

}meta_data;

meta_data* hdr = (meta_data *)heap ;

void Initializie()
{
    hdr->size_of_memory =  HEAP_SIZE - sizeof(struct meta_block);
    hdr->free=1;
    hdr->next=NULL;
    printf("initialized \n");
}


void* alloc(int size_of_data)
{
    meta_data* curr ;
    void *result;
    if((hdr->size_of_memory)==0)
    {
        Initializie();
    }
    curr = hdr ;
    while (((curr->size_of_memory < size_of_data) || (curr->free == 0)) && (curr->next != NULL))
    {
        curr = curr->next;
    }
    if(curr->size_of_memory == size_of_data)
    {
        curr->free = 0;
        result= (void*)(++curr);
        printf(" \n free_block found \n");
    }
    else if(curr->size_of_memory > (size_of_data + sizeof(meta_data)))
    {
       meta_data*  Next = (meta_data*)(((char*)(curr) + size_of_data + sizeof(meta_data)));
       Next->size_of_memory = curr->size_of_memory - size_of_data -sizeof(meta_data) ;
       Next->free = 1;
       Next->next = curr->next;
       curr->size_of_memory = size_of_data;
       curr->free = 0;
       curr->next = Next ;
       result = (void *)(++curr);
       printf("free block found \n");
    }
    else
    {
        result = NULL;
        printf("out of memory\n");
    }
    return result;
}
void merge()
{
    meta_data *curr,* Next ;
    int flag = 0;
    curr = hdr;
    Next = curr->next ;
    while(flag == 0 && Next != NULL)
    {
        if(curr->free == 1 && Next->free == 1)
        {
            if(Next->next != NULL  && Next->next->free == 1)
            {
                curr->size_of_memory = curr->size_of_memory + Next->size_of_memory+ Next->next->size_of_memory + 2*sizeof(meta_data) ;
                curr->next = Next->next->next ;
            }
            else
            { 
               curr->size_of_memory = curr->size_of_memory + Next->size_of_memory + sizeof(meta_data);
               curr->next =Next->next ;
            }
            flag = 1;
            
        }
        else
        {
            curr = curr->next ;
            Next =Next->next ;
        }
    }

}

void freee(void * ptr)
{    
    if (((void *)heap <= ptr) && (ptr <= (void *)(heap + HEAP_SIZE)))
    {
        meta_data *curr =ptr ;   
       --curr;
        curr->free = 1;
        merge();
        printf("memroy freeed \n");
    }
    else
    {
        printf("freee called on unallocated memory\n ");
    }

}

void show_heap()
{

	meta_data *ptr;
    ptr = hdr;
	while(ptr != NULL)
	{
        if(ptr->free == 0)
        {
            printf("%12s\n", "ALLOCATED");
            printf("----------------------\n");
            printf("%10s %7d\n","Size: ",ptr->size_of_memory);
            printf("----------------------\n");	
        }
        else
        {
            printf("%12s\n", "FREE");
            printf("----------------------\n");
            printf("%10s %7d\n","Size: ",ptr->size_of_memory);
            printf("----------------------\n");
        }
        ptr = ptr->next;
	}
}

int main()
{
    int  * p ,*q;
    p = (int*)alloc(sizeof(int)*10);
    show_heap();
    q = (int*) alloc(sizeof(int)*5);
    *p =10;
    printf(" \n p contains c %d  \n",*p);
    freee(p);
    show_heap();
    printf("\n \n");
    freee(q);
    show_heap();
  
    return 0;
}