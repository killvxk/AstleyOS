#include <kernel.h>
#include <system.h>
#include <modules.h>
#include <memory.h>
#include <video.h>
#include <output.h>
#include <idt.h>
#include <interrupts.h>
#include <exceptions.h>
#include <pic.h>
#include <pit.h>
#include <serial.h>
#include <sound.h>
#include <kalloc.h>
#include <heap.h>
#include <paging.h>
#include <process.h>
#include <scheduler.h>
#include <video_mode.h>
#include <ipc.h>

#define PAGE_SIZE 0x1000

#define MODULE_SHELL_INDEX 0
#define MODULE_SHELL_ADDRESS 0x0000000020440358

extern uint8_t kernel_text;
extern uint8_t kernel_rodata;
extern uint8_t kernel_data;
extern uint8_t kernel_bss;
extern uint8_t kernel_binary;
extern uint8_t kernel_end;

typedef int (* EntryPoint)(void);

void * kernel_init(void);
int kernel_main(void);

static void loadIDT(void);

static void * module_addresses[] = {
	(void *) MODULE_SHELL_ADDRESS // Shell address
};

void * kernel_init(void) {
	module_load(&kernel_binary, module_addresses);

	// Clear BSS
	memset(&kernel_bss, 0, &kernel_end - &kernel_bss);

	// Return stack's base
	return (void *) (
		(uint64_t) &kernel_end			// End of kernel address
		+ ((uint64_t) PAGE_SIZE) * 8	// The size of the stack itself, 32KiB
		- sizeof(uint64_t)				// Begin at the top of the stack
	);
}

int kernel_main(void) {
	video_init();
	out_init();
	out_printf("Initializing video driver... [Done]\n");
	out_printf("Loading modules... [Done]\n");

	out_printf("Initializing serial port... ");
	serial_init();
	out_printf("[Done]\n");

	log_init();
	log("<KERNEL> text: %h\n", (uint64_t) &kernel_text);
	log("<KERNEL> rodata: %h\n", (uint64_t) &kernel_rodata);
	log("<KERNEL> data: %h\n", (uint64_t) &kernel_data);
	log("<KERNEL> bss: %h\n", (uint64_t) &kernel_bss);

	out_printf("Initializing heap... ");
	heap_init();
	out_printf("[Done]\n");

	out_printf("Enabling paging... ");
	assert(paging_init());
	out_printf("[Done]\n");

	out_printf("Initializing & configuring PIC... ");
	pic_init();
	out_printf("[Done]\n");

	out_printf("Disabling interrupts... ");
	interrupt_clear();
	pic_mask_all();
	out_printf("[Done]\n");

	out_printf("Initializing PIT... ");
	pit_init();
	out_printf("[Done]\n");

	out_printf("Initializing IDT... ");
	assert(idt_init());
	out_printf("[Done]\n");

	loadIDT();

	out_printf("Enabling interrupts... ");
	interrupt_set();
	pic_mask(0xFC); // TODO:
	out_printf("[Done]\n");

	out_clear();

	schedulerInit();
	mqInit();
		


	EntryPoint shell = module_addresses[MODULE_SHELL_INDEX];

	shell();
	while(1);

	return 0;
}

void kernel_panic(const char * code, const char * desc, const char * source, const int halt) {
	log("\nKERNEL EXCEPTION!!!\n\n");
	out_box_top();
	out_box_line("OOPS! Something went wrong");
	out_box_line("EXCEPTION CODE: #%s", code);
	out_box_line("DESCRIPTION: %s", desc);
	out_box_line("SOURCE: %s", source);
	out_box_line("HALT SYSTEM? %s", STRINGNIFY_BOOL_YESNO(halt));
	out_box_bottom();
	out_printf("\n");

	if(halt) {
		out_printf("Disabling interrupts... ");
		interrupt_clear();
		pic_mask_all();
		out_printf("[Done]\n");

		out_printf("System halted!");
		system_halt();
	}
}

static void loadIDT(void) {
	// Exceptions
	out_printf("Loading exceptions... ");
	_IDT_ENTRY_EXCEPTION(00, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(02, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(03, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(04, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(05, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(06, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(07, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(08, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(09, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(0A, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(0B, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(0C, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(0D, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(0E, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(10, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(11, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(12, _IDT_ACCESS_INT);
	_IDT_ENTRY_EXCEPTION(13, _IDT_ACCESS_INT);
	out_printf("[Done]\n");
	// ^^ Exceptions ^^

	// Interrupts
	out_printf("Loading interrupts... ");
	_IDT_ENTRY_INTERRUPT(20, _IDT_ACCESS_INT);
	_IDT_ENTRY_INTERRUPT(21, _IDT_ACCESS_INT);
	_IDT_ENTRY_INTERRUPT(80, _IDT_ACCESS_INT);
	out_printf("[Done]\n");
	// ^^ Interrupts ^^
}
