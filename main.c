#include<lib/18F4620.h>
#FUSES NOWDT,NOFCMEN,NOIESO,PUT,NOBROWNOUT,NOPBADEN,NOLPT1OSC,NOLVP,NOXINST
#use delay(crystal=16000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use i2c(MASTER,FAST,SDA=PIN_C4,SCL=PIN_C3)
#define SLAVE_WRITE_ADDR 0xD0
#define SLAVE_READ_ADDR 0xD1
#use standard_io(c)

int convertidorBcdABin(BYTE bcd);
void obtenerFecha(BYTE& dia, BYTE& mes, BYTE& year);
void obtenerTiempo(BYTE& hora, BYTE& minutos, BYTE& segundos);

int segundos,minutos,horas,dia,mes,year;

void main(){
    obtenerTiempo(horas,minutos,segundos);
    printf("horas:%d minutos:%d segundos:%d\n\r",horas,minutos,segundos);
    obtenerFecha(dia,mes,year);
    printf("dia:%d mes:%d year:%d",dia,mes,year);
}

int convertidorBcdABin(BYTE bcd){
    return (((bcd)&15) + ((bcd)>>4)*10);
}
void obtenerFecha(BYTE& dia, BYTE& mes, BYTE& year){
    i2c_start(); //Iniciar comunicación
    i2c_write(0xD0); //Direccion de escritura
    i2c_write(0x04); //Puntero de la cuarta dirección para la fecha
    i2c_start(); //Lectura
    i2c_write(0xD1); //Direccion de lectura
    dia = convertidorBcdABin(i2c_read()&0x1f); //Leer y asignar los 5 bit del dia
    mes = convertidorBcdABin(i2c_read()&0xf); //4 bits de los meses
    year = convertidorBcdABin(i2c_read(0)&0xff); //8 bits de los años
}
void obtenerTiempo(BYTE& hora, BYTE& minutos, BYTE& segundos){
    i2c_start(); //Iniciar comunicación
    i2c_write(0xD0); //Direccion de escritura
    i2c_write(0x00); //Puntero de la primera dirección
    i2c_start(); //Lectura
    i2c_write(0xD1); //Direccion de lectura
    segundos = convertidorBcdABin(i2c_read()&0x7f); //Leer y asignar los 7 bit de los segundos
    minutos = convertidorBcdABin(i2c_read()&0x7f); //7 bit de los minutos
    horas = convertidorBcdABin(i2c_read(0)&0x3f); //6 bit de las horas
    i2c_stop(); //Finaliza comunicación
}