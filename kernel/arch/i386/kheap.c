/**
 * Author: Isaac Assegai
 * Date  : 10/1/2017
 * The kernels heap functions
 * Provides a type of malloc (kmalloc) for use before the heap is initialized.
 */

#include <stdlib.h>
#include <stddef.h>
#include <kernel/kheap.h>

//memLoc is defined in mm.c
extern u32int* memLoc;
extern u32int placement_address;// = (u32int)memLoc;

//u32int placement_address = memLoc;

void kheap_test(){
  printf("\nkheap test");
  printf("\nmemLoc: 0x%x", memLoc);
  printf("\nplacement_address: 0x%x", placement_address);
  u32int someAddress = kmalloc_a(4096);
  printf("\nSomeAddress: 0x%x", someAddress);
  printf("\nplacement_address: 0x%x", placement_address);
  u32int someOtherAddress = kmalloc_a(4096);
  printf("\nSomeOtherAddress: 0x%x", someOtherAddress);
  printf("\nplacement_address: 0x%x", placement_address);

}

u32int kmalloc_int(u32int sz, int align, u32int *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if (align == 1 && (placement_address & 0xFFFFF000) )
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys)
    {
        *phys = placement_address;
    }
    u32int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

u32int kmalloc_a(u32int sz)
{
    return kmalloc_int(sz, 1, 0);
}

u32int kmalloc_p(u32int sz, u32int *phys)
{
    return kmalloc_int(sz, 0, phys);
}

u32int kmalloc_ap(u32int sz, u32int *phys)
{
    return kmalloc_int(sz, 1, phys);
}

u32int kmalloc(u32int sz)
{
    return kmalloc_int(sz, 0, 0);
}
