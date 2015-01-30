#ifndef _e820_
#define _e820_
typedef struct SMAP_entry {
 
	unsigned int BaseL; // base address uint64_t
	unsigned int BaseH;
	unsigned int LengthL; // length uint64_t
	unsigned int LengthH;
	unsigned int Type; // entry Type
	//unsigned int gg;
}__attribute__((packed)) SMAP_entry_t;

SMAP_entry_t s[8];

#endif