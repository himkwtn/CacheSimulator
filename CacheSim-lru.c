/**
 * CacheSim.c 
 * This program is designed for class exercise only.
 * It is provided as is. There is no warranty or support of any kind.
 *
 * Krerk Piromsopa, Ph.D.
 * Department of Computer Engineering
 * Chulalongkorn University
 * Bangkok, Thailand.
 * 
 * History
 * 2013 - Initial design
 * 2015 - Refactor/Clean code
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CacheSim.h"
#include <math.h>

void init(struct LRUCache cache[], int index_size)
{
  int i;
  for (i = 0; i < index_size; i++)
  {
    for (int j = 0; j < ENTRIES; j++)
    {
      (cache)[i].entry[j].valid = 0;
      (cache)[i].entry[j].tag = 0;
      (cache)[i].entry[j].dirty = 0;
      (cache)[i].lastused[j] = 0;
    }
  }
}

void calAddr(unsigned long addr, unsigned long *tag,
             unsigned long *idx, unsigned long *offset,
             int offsetLen, int indexLen)
{
  unsigned long tmp;
  int i;
  *tag = addr >> (offsetLen + indexLen);
  tmp = 0;
  for (i = 0; i < indexLen; i++)
  {
    tmp <<= 1;
    tmp |= 0x01;
  }
  *idx = addr >> offsetLen & tmp;
  tmp = 0;
  for (i = 0; i < offsetLen; i++)
  {
    tmp <<= 1;
    tmp |= 0x01;
  }
  *offset = addr & tmp;
}

void inc(int arr[], int size)
{
  for (int i = 0; i < size; i++)
  {
    arr[i]++;
  }
}

int find_maximum(int a[], int n)
{
  int c, max, index;

  max = a[0];
  index = 0;

  for (c = 1; c < n; c++)
  {
    if (a[c] > max)
    {
      index = c;
      max = a[c];
    }
  }

  return index;
}

void access(unsigned long addr, struct LRUCache cache[], long *hit, long *miss, int offsetLen, int indexLen)
{
  unsigned long offset;
  unsigned long tag;
  unsigned long idx;
  int valid = 0;
  calAddr(addr, &tag, &idx, &offset, offsetLen, indexLen);
  inc(cache[idx].lastused, ENTRIES);
  for (int i = 0; i < ENTRIES; i++)
  {
    if (cache[idx].entry[i].tag == tag && cache[idx].entry[i].valid)
    {
      (*hit)++;
      cache[idx].lastused[i] = 0;
      return;
    }
  }
  struct LRUCache *c = &cache[idx];
  (*miss)++;
  int index = find_maximum(c -> lastused, ENTRIES);
  c -> lastused[index] = 0;
  c -> entry[index].valid = 1;
  c -> entry[index].tag = tag;
}
int main(int argc, char *argv[])
{
  printf("CacheSim v.2015\n");
  printf("This program is designed for class exercise only.\n");
  printf("By Krerk Piromsopa, Ph.D.\n");
  printf("LRU %d-way\n", ENTRIES);
  FILE *input;
  char buff[1025];
  unsigned long myaddr;
  if (argc < 2)
  {
    fprintf(stderr, "Usage:\n\t%s address_file\n", argv[0]);
    exit(-1);
  }

  int size[] = {1, 4, 8, 32, 512, 1024};
  int block_size = size[3];

  for (int j = 0; j < sizeof(size) / 4; j++)
  {
    input = fopen(argv[1], "r");
    int cache_size = size[j] * 1024;
    int index_size = cache_size / block_size;
    int offset_len = log2(block_size);
    int index_len = log2(index_size);
    int tag_len = 32 - offset_len - index_len;
    long miss = 0;
    long hit = 0;
    // printf("Cache Size: %d, Block Size: %d, Index Size: %d, Offset Length: %d, Index Length:%d, Tag Length: %d\n",
    //  cache_size, block_size, index_size, offset_len, index_len, tag_len);
    struct LRUCache *cache = malloc(index_size*sizeof(struct LRUCache));
    init(cache, index_size);

    //read file
    while (fgets(&buff[0], 1024, input))
    {
      sscanf(buff, "0x%lx", &myaddr);
      access(myaddr, cache, &hit, &miss, offset_len, index_len);
    }
    printf("(%7ld,%7ld) ", hit, miss);
    printf("%f%%\n", (miss / (double)(hit + miss) * 100));
    free(cache);
  }
  // printf("\n");
  // }

  return 0;
}