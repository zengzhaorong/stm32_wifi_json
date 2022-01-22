#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ringbuffer.h"


/* retturn: 1-empty, 0-not empty */
int ringbuf_empty(struct ringbuffer *ringbuf)
{
	if(ringbuf == NULL)
		return -1;
		
	return (ringbuf->len == 0 ? 1 : 0);
}

/* retturn: 1-full, 0-not full */
int ringbuf_full(struct ringbuffer *ringbuf)
{
	if(ringbuf == NULL)
		return -1;
	
	return (ringbuf->len == ringbuf->size ? 1 : 0);
}

/* return: read byte conut */
int ringbuf_read(struct ringbuffer *ringbuf, unsigned char *buf, int len)
{
	int tmplen = 0;
	int retlen = 0;
	
	if(ringbuf == NULL || buf == 0)
		return -1;

	if(ringbuf->len == 0)
		return 0;
	
	if(ringbuf->head > ringbuf->tail)
	{
		tmplen = (len <= ringbuf->head - ringbuf->tail ? len : ringbuf->head - ringbuf->tail);
		memcpy(buf, ringbuf->buf +ringbuf->tail, tmplen);
		ringbuf->tail += tmplen;
		retlen = tmplen;
	}
	else
	{
		if(ringbuf->size -ringbuf->tail >= len)
		{
			memcpy(buf, ringbuf->buf +ringbuf->tail, len);
			ringbuf->tail = (ringbuf->tail +len) % ringbuf->size;
			retlen = len;
		}
		else
		{
			tmplen = ringbuf->size -ringbuf->tail;
			memcpy(buf, ringbuf->buf +ringbuf->tail, tmplen);
			ringbuf->tail = 0;
			retlen = tmplen;
			tmplen = (len -retlen <= ringbuf->head ? len -retlen : ringbuf->head);
			memcpy(buf +retlen, ringbuf->buf +0, tmplen);
			ringbuf->tail += tmplen;
			retlen += tmplen;
		}
	}

	ringbuf->len -= retlen;

	return retlen;
}

/* return: write byte conut */
int ringbuf_write(struct ringbuffer *ringbuf, unsigned char *buf, int len)
{
	int tmplen = 0;
	int retlen = 0;

	if(ringbuf == NULL || buf == 0 || len<0)
		return -1;
	
	if(ringbuf->len == ringbuf->size || len == 0)
		return 0;
	
	if(ringbuf->head >= ringbuf->tail)
	{
		if(ringbuf->size - ringbuf->head >= len)
		{
			memcpy(ringbuf->buf +ringbuf->head, buf, len);
			ringbuf->head = (ringbuf->head +len) % ringbuf->size;
			retlen = len;
		}
		else
		{
			tmplen = ringbuf->size -ringbuf->head;
			memcpy(ringbuf->buf +ringbuf->head, buf, tmplen);
			ringbuf->head = 0;
			retlen += tmplen;
			tmplen = (len -retlen <= ringbuf->tail ? len -retlen : ringbuf->tail);
			memcpy(ringbuf->buf +0, buf +retlen, tmplen);
			ringbuf->head += tmplen;
			retlen += tmplen;
		}
	}
	else
	{
		tmplen = (len <= ringbuf->tail -ringbuf->head -1 ? len : ringbuf->tail -ringbuf->head);
		memcpy(ringbuf->buf +ringbuf->head, buf, tmplen);
		ringbuf->head += tmplen;
		retlen = tmplen;
	}

	ringbuf->len += retlen;

	return retlen;
}

int ringbuf_datalen(struct ringbuffer *ringbuf)
{
	if(ringbuf == NULL)
		return -1;
	
	return ringbuf->len;
}

int ringbuf_space(struct ringbuffer *ringbuf)
{
	return (ringbuf->size - ringbuf->len);
}

int ringbuf_size(struct ringbuffer *ringbuf)
{
	if(ringbuf == NULL)
		return -1;
	
	return ringbuf->size;
}

int ringbuf_reset(struct ringbuffer *ringbuf)
{
	if(ringbuf == NULL)
		return -1;

	ringbuf->head = 0;
	ringbuf->tail = 0;
	ringbuf->len = 0;
	
	return 0;
}

int ringbuf_init(struct ringbuffer *ringbuf, unsigned char *buf, int size)
{

	if(ringbuf==NULL || buf==NULL || size<=0)
		return -1;

	memset(ringbuf, 0, sizeof(struct ringbuffer));
	ringbuf->buf = buf;
	ringbuf->size = size;

	return 0;
}

void ringbuf_deinit(struct ringbuffer *ringbuf)
{
	if(ringbuf == NULL)
		return ;

	memset(ringbuf, 0, sizeof(struct ringbuffer));
	
}

