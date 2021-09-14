#include "ring_buffer.h"
#include <string.h>

// 判断 x 是否是 2 的次方
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

// 取 a 和 b 中的最小值
#define min(a, b)   (((a)<(b))? (a) : (b))

int rbInit(rb_t* pRb, unsigned char* pBuf, unsigned int length)
{
	if(!is_power_of_2(length) || !pRb || !pBuf)
	{
		return -1;
	}

	pRb->size = length;
	pRb->buffer = pBuf;
	pRb->in = 0;
	pRb->out = 0;

	return 0;
}

void rbDeInit(rb_t* pRb)
{
	if(pRb)
	{
		pRb->in = 0;
		pRb->out = 0;
		pRb->buffer = NULL;
	}
}

unsigned int rbGet(rb_t* pRb, unsigned char* pBuf, unsigned int size)
{
	if(!pRb || !pBuf)
	{
		return 0;
	}

	unsigned int len = 0;

	size = min(size, rbUsed(pRb));
	len = min(size, pRb->size - (pRb->out & (pRb->size - 1)));
	memcpy(pBuf, pRb->buffer + (pRb->out & (pRb->size - 1)), len);
	memcpy(pBuf + len, pRb->buffer, size - len);

	pRb->out += size;

	if(pRb->in == pRb->out)
	{
		pRb->in = pRb->out = 0;
	}

	return size;
}

unsigned int rbPut(rb_t* pRb, unsigned char* pBuf, unsigned int size)
{
	if(!pRb || !pBuf)
	{
		return 0;
	}

	unsigned int len = 0;
	size = min(size, rbUnUsed(pRb));
	len = min(size, pRb->size - (pRb->in & (pRb->size - 1)));
	memcpy(pRb->buffer + (pRb->in & (pRb->size - 1)), pBuf, len);
	memcpy(pRb->buffer, pBuf + len, size - len);
	pRb->in += size;

	return size;
}
