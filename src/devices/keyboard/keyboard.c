#include "kbutils.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
//#include "keyboard_map.h"
unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned int current_loc = 0;
extern char read_port(unsigned short port);
char *vidptr = (char*)0xb8000;
void EmeraldDevices_keyboard_Keyboard_init(void)
{

	outb(0x21 , 0xFD);
}
void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}
void EmeraldDevices_keyboard_Keyboard_handler_main(void *nothing)
{
	unsigned char status;
	char keycode;

	outb(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	keycode = read_port(KEYBOARD_DATA_PORT);
	if(keycode < 0){
		PIC_sendEOI(0);
		return;
		}

	if(keycode == ENTER_KEY_CODE) {
		kprint_newline();
		return;
		}
	if(keycode == DELETE_KEY_CODE) {
		kprint_newline();
		return;
		}

	vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
	vidptr[current_loc++] = 0x07;
}
void kprint(const char *str,int color)
{
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = color;
	}
}
void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}
void kprint_load(char *str,bool error){
	if(error == false){

	kprint("[ ",7);
	kprint("OK ",10);
	kprint("] ",7);
	kprint(str,0x07);
	kprint(" loaded",0x07);	
	kprint_newline();
}
	else if (error == true)
	{
	kprint("[ ",7);
	kprint("ERROR ",12);
	kprint("] ",7);
	kprint(str,0x07);
	kprint(" failed to load",0x07);	
	kprint_newline();
		}
	}

void clear_screen(void)
{
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
}