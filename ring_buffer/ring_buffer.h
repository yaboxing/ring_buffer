#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

//#include <driver/platform/generic.h>

typedef struct {
	unsigned char* buffer;
	unsigned int size;
	unsigned int in;
	unsigned int out;
} rb_t;

extern int rbInit(rb_t* pRb, unsigned char* pBuf, unsigned int length);

extern void rbDeInit(rb_t* pRb);

static inline bool rbIsEmpty(rb_t* pRb)
{
	return pRb->in == pRb->out;
}

static inline bool rbIsFull(rb_t* pRb)
{
	return (pRb->in - pRb->out) == pRb->size;
}

static inline unsigned int rbUsed(rb_t* pRb)
{
	return pRb->in - pRb->out;
}

static inline unsigned int rbUnUsed(rb_t* pRb)
{
	return pRb->size - pRb->in + pRb->out;
}

extern unsigned int rbGet(rb_t* pRb, unsigned char* pBuf, unsigned int size);

extern unsigned int rbPut(rb_t* pRb, unsigned char* pBuf, unsigned int size);

#endif
