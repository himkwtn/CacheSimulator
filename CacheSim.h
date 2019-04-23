/**
 * CacheSim.h
 * This program is designed for class exercise only.
 * It is provided as is. There is no warranty or support of any kind.
 *
 * Krerk Piromsopa, Ph.D.
 * Department of Computer Engineering
 * Chulalongkorn University
 * Bangkok, Thailand.
**/
#ifndef CACHESIM_H_INCLUDED
#define CACHESIM_H_INCLUDED

/* Please adjust parameters here */
#define CACHE_SIZE 4096
#define INDEX_SIZE 256
#define BLOCK_SIZE 16
#define TAGLEN 20
#define INDEXLEN 8
#define OFFSETLEN 4
#define ENTRIES 2
typedef unsigned char Byte;
typedef unsigned char bool;
typedef struct Cache
{
  bool valid;
  bool dirty;
  unsigned long tag;
  Byte data[BLOCK_SIZE];
};

typedef struct RoundRobinCache
{
  struct Cache entry[ENTRIES];
  int index;
};

typedef struct LRUCache
{
  struct Cache entry[ENTRIES];
  int lastused[ENTRIES];
};
#endif // CACHESIM_H_INCLUDED