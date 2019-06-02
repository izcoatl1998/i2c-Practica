#include<lib/18F4620.h>
#FUSES NOWDT,NOFCMEN,NOIESO,PUT,NOBROWNOUT,NOPBADEN,NOLPT1OSC,NOLVP,NOXINST
#use delay(crystal=16000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use i2c(MASTER,FAST,SDA=PIN_C4,SCL=PIN_C3)
#define SLAVE_WRITE_ADDR 0xD0
#define SLAVE_READ_ADDR 0xD1
#use standard_io(c)

int BCDaBIN(BYTE bcd);
void rtc_get_time(BYTE& hr, BYTE& min, BYTE& sec);

int segundos,minutos,horas;

void main(){
    int i=0;
    rtc_get_time(horas,minutos,segundos);
    printf("horas:%d minutos:%d segundos:%d\n\r",horas,minutos,segundos);
}

int convertidorBcdABin(BYTE bcd){
    return (((bcd)&15) + ((bcd)>>4)*10);
}
void rtc_get_time(BYTE& hora, BYTE& minutos, BYTE& segundos) {
    i2c_start(); //Escritura
    i2c_write(0xD0); //Codigo de escritura
    i2c_write(0x00); //Puntero de la primera dirección
    i2c_start(); //Lectura
    i2c_write(0xD1); //Codigo de lectura
    segundos = convertidorBcdABin(i2c_read()&0x7f); //Lectura de los 7 bit de los segundos
    minutos = convertidorBcdABin(i2c_read()&0x7f); //Lectura de los 7 bit de los minutos
    horas = convertidorBcdABin(i2c_read(0)&0x3f); //Lectura de los 6 bit de las horas
    i2c_stop(); //Finaliza comunicación
}