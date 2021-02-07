/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "boot/stivale2.h"
#include "system/GDT.h"
#include <ascii.h>
#include <boot/boot.h>
#include <devices/keyboard/keyboard.h>
#include <devices/pci/PCI.h>
#include <devices/serial/serial.h>
#include <devices/video/vbe.h>
#include <libk/logging.h>
#include <libk/module.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <stddef.h>
#include <stdint.h>
#include <system/interrupts/IDT.h>
#include <system/interrupts/PIT.h>

color_t white = {255, 255, 255}, green = {0, 148, 99}, gray = {94, 94, 94};

void kmain(struct stivale2_struct *info)
{

    module("main");

    GDT_init();
    IDT_init();

    Serial_init();
    Keyboard_init();

    VBE_init(info);
    VBE_clear_screen();

    info = (void *)info + MEM_OFFSET;
    
    PCI_init();
    
    BootInfo *boot_info = Boot_get_info(info);

    PMM_init((void *)boot_info->memory_map, boot_info->memory_map->entries);

    /*VMM_init();*/

    VBE_puts("\nWelcome to ", white);
    VBE_puts("EmeraldOS!\n", green);

    VBE_display_circle(200,500,120);
    set_ascii();
    while (1);
}
