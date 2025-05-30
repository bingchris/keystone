#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "framebuffer/framebuffer.h"
#include "limine_setup.h"
#include "libc/memory.h"
#include "vfs/vfs.h"
#include "memory/kmalloc.h"
#include "drivers/keyboard.h"

/*
	Keystone kernel
	bingchris 2025
*/

// base revision 3 (latest)
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// framebuffer request
__attribute__((used, section(".limine_requests")))

// root node for the in-tmp filesystem
static vfs_node_t tmpfs_root = {
    .name = "tmpfs",
    .type = VFS_TYPE_DIR,
    .size = 0,
    .ops = NULL,
    .parent = NULL,
    .children = NULL,
    .next = NULL,
    .data = NULL
};

// define heap size
#define HEAP_SIZE (64 * 1024)
static uint8_t kernel_heap[HEAP_SIZE];

#include "libc/memory.h"

// halt the system
static void halt(void) {
	for (;;) {
		asm ("hlt");
	}
}

void kernel_main(void) {
    // check if base revision is supported
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        halt();
    }

    // setup limine
    setup_limine();

    // get framebuffer
    /*struct framebuffer *fb = get_framebuffer();
    if (!fb) {
        halt(); 
    }
    */
    

    // print boot message
    kprint("Keystone 0.1\n", 0x00ff00);

    // init heap
    kprint("keystone: init heap\n", 0xffffff);
    kmalloc_init(kernel_heap, HEAP_SIZE);

    // mount root
    kprint("keystone: mount root\n", 0xffffff);
    vfs_mount("/", &tmpfs_root);

    // create file
    vfs_node_t *file = vfs_create_file("test.txt", &tmpfs_root);
    if (!file) {
        halt();
    }

    // write to file
    const char *data = "vfs test";
    if (vfs_write_to_file(file, data, 12) != 0) {
        halt();
    }

    // read from file
    char buffer[13] = {0};
    if (vfs_read_from_file(file, buffer, sizeof(buffer)) > 0) {
        kprint("ok so heres the data: ", 0xffffff);
        kprint(buffer, 0xffffff);
        kprint("\n", 0xffffff);
    }


    //shit (shell is this (shit))
    //using keybord.c
    keyboard_init();
    kprint("test> ", 0xffffff); //ok done TODO: fix keyboard.c or something to make the user not be able to eat the fucking prompt
    while (true) { //remind me tomorrow to implement this somehwere else
        keyboard_read();
        uint8_t key = keyboard_get_ascii();
        if (key) {
            if (key == '\b') {
                kprint_user("\b \b", 0xffffff);
            } else if (key) {
                kprint_user((char[]){key, '\0'}, 0xffffff);
            }

        }
        if (key == 0x1c) {
            break;
        }
    }


    // halt
    halt();
}