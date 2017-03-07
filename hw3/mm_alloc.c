/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */
#define DEBUG
#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>

#ifdef DEBUG
 #include "stdio.h"
#endif

#define METADATA_SIZE 0x20
#define OFFSET 0x19
 typedef struct metadata *pMeta;
struct metadata{
	size_t size;
	pMeta next;
	pMeta prev;
	char free;
	char data[0];//zero-array for variable length.
};

pMeta extend_heap_for(pMeta last, size_t size);
pMeta find_block(pMeta *last, size_t size);
pMeta firstmeta = NULL;
// extend heap for specific size block and return the metadata if success or null
// "last": is the last metadata. We modify it by making it point to new metadata. 
pMeta extend_heap_for(pMeta last, size_t size){
	pMeta p;
	p = sbrk(METADATA_SIZE + size);
	if(p == (void*) -1)
		return NULL;
	p->size = size;
	p->next = NULL;
	p->free = 0;
	p->prev = last;
	if(last != NULL)
		last->next =p;
	return p; 
}
// find the first block which is free and its size is big for the request size
// The return value is the metadata of a block satisfy the above constraint or null if 
// doesn't exit. 
// "last" is the last metadata. It is exactly the last one only when doesn't find out.
pMeta find_block(pMeta *last, size_t  size){
	pMeta p = firstmeta;
	while(p != NULL ){
		if(p->free && p->size >= size){
			return p;
		}
		*last = p;
		p = p->next;
	}
	return NULL;
}
void *mm_malloc(size_t size) {
    /* YOUR CODE HERE */
    //TODO: concurrent situation.
	pMeta p, last;
	if (!size)
		return NULL;
	if(firstmeta){
	       p = find_block(&last, size);
		if(p){
		// success
			p->free = 0;
			// if there is enough size for two block, then split into two parts.
			// the first part point to second part and the second part point to where
			// the first part point to before.
			if(p->size > size + METADATA_SIZE){
				
				pMeta newmeta;
				newmeta =(pMeta) p->data + size;
				newmeta->size = p->size - size - METADATA_SIZE;
				newmeta->next = p->next;
				newmeta->prev = p;
				newmeta->free = 1;
				p->next = newmeta;
			}
			return p->data;	
		}else{
		// fail
			 p = extend_heap_for(last,size);
			if(p == NULL)
				return NULL;
			return p->data;
		}		
	}else{		
		p = extend_heap_for(NULL, size);
		// now, 'p' is the first metadata ever established.
		firstmeta = p;
		if(p == NULL)
			return NULL;
		return p->data;	
	}	
}

void *mm_realloc(void *ptr, size_t size) {
    /* YOUR CODE HERE */
   	if (ptr != NULL &&size == 0)
   	{
   		mm_free(ptr);
   		return NULL;
   	}
   	if (ptr == NULL )
   	{
   		return mm_malloc(size);
   	}
   	// if (ptr < firstmeta || ptr > sbrk(0))
   	// {
   	// 	return NULL;
   	// }
   	pMeta meta = (pMeta) (ptr - OFFSET);
   	int i;
   	char *newptr, *p;
   	size_t copybytes, zerobytes;
    mm_free(ptr);
    newptr = mm_malloc(size);
    p = ptr;
   	if (meta->size > size)
   	{
   		zerobytes = 0;
   		copybytes = size;
   	}else{
   		copybytes = meta->size;
   		zerobytes = size - copybytes;
   	}
   	for (i = 0; i < copybytes; ++i)
   		newptr[i] = p[i];
   	p = newptr + i ;
   	for(i = 0;i < zerobytes; i++)
   		p[i] = 0;
    return newptr;
}

void mm_free(void *ptr) {
    /* YOUR CODE HERE */
    if (ptr == NULL)
   		return;
   	// if (ptr < firstmeta || ptr > sbrk(0))
   	// {
   	// 	return;
   	// }
    pMeta p = (pMeta)(ptr - OFFSET);
    if (p->free)
    {
    	return;
    }
    p->free = 1;
    pMeta adjacentFirstFree = p;
    while(p!= NULL&& p->free){
  		adjacentFirstFree = p;
  		p = p->prev;
  	}
    p = adjacentFirstFree->next;
    while(p!=NULL && p->free){
    	adjacentFirstFree->size += p->size + METADATA_SIZE;
    	p = p->next;
    }
    adjacentFirstFree->next = p;
}
#ifdef DEBUG
void mm_info(){
	printf("Heap Information:\n");
	printf("Metadata size:%d\n", (int)METADATA_SIZE);
	pMeta p = firstmeta;
	printf("%d,%d\n%p\n%p\n",sizeof(size_t), sizeof(pMeta), &p->size, p->data);
	int i = 1;
	while(p != NULL){
		printf("%d\tsize:%d\tfree:%2d\n", i++, (int)p->size,p->free);
		p = p->next;
	}
	printf("start of heap:%p, brk:%p\n", firstmeta, sbrk(0) );
}
#endif