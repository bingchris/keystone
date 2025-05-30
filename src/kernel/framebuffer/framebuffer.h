#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
/*
	Keystone kernel
	bingchris 2025
*/
#include <stdint.h>

void framebuffer_clear(uint32_t color);
void framebuffer_set_pixel(uint16_t x, uint16_t y, uint32_t color);
void scroll_framebuffer();  
void kprint(const char *str, uint32_t color);
uint32_t framebuffer_get_pixel(uint16_t x, uint16_t y); 
void framebuffer_move_cursor_back();
void draw_cursor(uint32_t color);
void handle_backspace();
void update_cursor();
void set_min_cursor();
static uint16_t min_cursor_x;
static uint16_t min_cursor_y;
void kprint_user(const char *str, uint32_t color);
#endif
