void init_descriptor_tables();

struct idt_entry_struct
{
    uint16_t offset_low;
    uint16_t sel;                 // Kernel segment selector.
    uint8_t zero0;             // This must always be zero.
    uint8_t  typeAttr;               // More flags. See documentation.
    uint16_t offset_mid;             // The upper 16 bits of the address to jump to.
    uint32_t offset_high;
    uint32_t zero1;
} __attribute__((packed));

//typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

//typedef struct idt_ptr_struct idt_ptr_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr32();
extern void isr33();

