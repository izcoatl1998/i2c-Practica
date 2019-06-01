#include<lib/18F4620.h>
#FUSES NOWDT,NOFCMEN,NOIESO,PUT,NOBROWNOUT,NOPBADEN,NOLPT1OSC,NOLVP,NOXINST
#use delay(crystal=16000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use i2c(MASTER,FAST=100000,SDA=PIN_C4,SCL=PIN_C3)
#define SLAVE_WRITE_ADDR 0xD0
#define SLAVE_READ_ADDR 0xD1
#use standard_io(c)

void main(){
    int i=0;
    while(TRUE){
        i2c_start();
        i2c_write(SLAVE_READ_ADDR);
        int segundos=0,minutos=0,hora=0;
        segundos=i2c_read(1);
        minutos=i2c_read(1);
        hora=i2c_read(0);
        printf("SEG:%d\n\rMIN:%d\n\rHORA:%d\n\r",segundos,minutos,hora);
        printf("algo");
        getch();
        i++;
        if(i>5){
            FALSE;
        }
    }
    i2c_stop();
}