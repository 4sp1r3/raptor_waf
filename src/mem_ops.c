#include "mem_ops.h"
#include "utils.h"


// based in OpenBSD reallocarray() function http://man.openbsd.org/reallocarray.3
void *xallocaarray (size_t nmemb, size_t size) 
{
	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && nmemb > 0 && SIZE_MAX / nmemb < size) 
	{
		DEBUG("integer overflow block");
		return NULL;
	}

	void *ptr = alloca (nmemb*size);

	if (ptr == NULL) 
		return NULL;

	return ptr;
}



// based in OpenBSD reallocarray() function http://man.openbsd.org/reallocarray.3
void *xmallocarray (size_t nmemb, size_t size) 
{
	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && nmemb > 0 && SIZE_MAX / nmemb < size) 
	{
		DEBUG("integer overflow block");
		return NULL;
	}

	void *ptr = malloc (nmemb*size);

	if (ptr == NULL) 
		return NULL;

	return ptr;
}

static void *xmalloc_fatal(size_t size) 
{
	if ( size == 0 ) 
		return NULL;

	DEBUG("\n Memory FAILURE...\n");

	exit(1);
}

void *xmalloc (size_t size) 
{
	void *ptr = malloc (size);

	if (ptr == NULL) 
		return xmalloc_fatal(size);

	return ptr;
}

void *xcalloc (size_t mem, size_t size) 
{
	void *ptr = calloc (mem, size);

	if (ptr == NULL) 
		return xmalloc_fatal(mem*size);

	return ptr;
}

void *xrealloc (void *ptr, size_t size) 
{
	void *p = realloc (ptr, size);

	if (p == NULL) 
		return xmalloc_fatal(size);

	return p;
}

void xfree(void **ptr) 
{
	assert(ptr);
	if( ptr != NULL )
        {
		free(*ptr);
		*ptr=NULL;	
        }
	
}

   
volatile void *burn_mem(volatile void *dst, int c, size_t len) 
{
	volatile char *buf;
   
	for(buf = (volatile char *)dst;  len;  buf[--len] = c);

	return dst;
}
   
volatile void *burn_memcpy(volatile void *dst, volatile void *src, size_t len) 
{
	volatile char *cdst, *csrc;
   
	cdst=(volatile char *)dst;
	csrc=(volatile char *)src;

	while(len--) 
		cdst[len] = csrc[len];

  	return dst;
}
   
volatile void *burn_memmove(volatile void *dst, volatile void *src, size_t len) 
{
	size_t i=0;
	volatile char *cdst, *csrc;
   
	cdst=(volatile char *)dst;
	csrc=(volatile char *)src;

	if(csrc > cdst && csrc < cdst + len)
		while(i < len)
		{ 
			cdst[i] = csrc[i];
			i++;
		}
  	else
   		while(len--) 
			cdst[len] = csrc[len];

  	return dst;
}
