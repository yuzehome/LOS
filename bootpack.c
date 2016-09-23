#include "bootpack.h"
#include <stdio.h>

extern struct KEY_BUFF key_buff; 
void HariMain(void){   
    //初始化gdt,idt
    init_gdtidt(); 
    //设定PIC
    init_pic();
    //开启CPU接收外来中断
    io_sti();
    //设置IMR，开启中断
    io_out8(PIC0_IMR,  0xf9  ); /* 11111001 启用键盘中断*/
    io_out8(PIC1_IMR,  0xef  ); /* 11101111 启用鼠标中断 */
    //设置缓冲区
    unsigned char key_data[32];
    init_fifo(&key_buff,32,key_data);
    // 设置鼠标控制电路可用
    set_mouse_control_circle_enable();
    //初始化调色板
    init_palette(); 
    //显示画面
    struct BOOTINFO *b_info = (struct BOOTINFO*)0x0ff0;
    init_screen(b_info);
   //显示鼠标指针
    char mouse[16*16];
    init_mouse_cursor8(mouse,COL8_008484);
    putblock8_8(b_info->vram,b_info->scrnx,16,16,60,60,mouse);
    //设置鼠标可用
    set_mouse_enable();
    for (;;) {
        io_cli();
        if(!has_next(&key_buff)){
            io_sti();
            io_hlt();
        }else{
            unsigned char s[4];
            sprintf(s,"%02X",pop(&key_buff));
            boxfill8(b_info->vram,b_info->scrnx,COL8_000000,0,0, 32*8-1, 15);
            put_string(b_info->vram,b_info->scrnx,0,0,COL8_FFFFFF,s);
        }
    }
}



