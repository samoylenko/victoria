#ifndef NULL
  #define NULL 0
#endif

extern int memlock(void *, short);
extern int memunlock(void *, short);
extern void copymem(void *, void *, short);

void *lockedmalloc(int);
void lockedfree(void *);

void *lockedmalloc(int size)
{
  short *block = (short *)malloc(size + 4);
  if (!block) return NULL;
  *block = size;
  if (!memlock(block + 1, size))
  {
    free(block);
    return NULL;
  }
  return block + 1;
}

void lockedfree(void *address)
{
  short *block;

  if (!address) return;
  block = (short *)address - 1;
  memunlock(block + 1, *block);
  free(block);
}
