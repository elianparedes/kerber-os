#ifndef _DEFS_H_
#define _DEFS_H_

/* segment access rights flags */
#define ACS_PRESENT 0x80 // present segment
#define ACS_CSEG    0x18 // code    segment
#define ACS_DSEG    0x10 // data    segment
#define ACS_READ    0x02 // read    segment
#define ACS_WRITE   0x02 // write   segment
#define ACS_IDT     ACS_DSEG
#define ACS_INT_386 0x0E // interrupt gate 32 bits
#define ACS_INT     (ACS_PRESENT | ACS_INT_386)

#define ACS_CODE    (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA    (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK   (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#define EOF -1


#endif /* _DEFS_H_ */