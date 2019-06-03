#include<lib/18F4620.h>
#FUSES NOWDT,NOFCMEN,NOIESO,PUT,NOBROWNOUT,NOPBADEN,NOLPT1OSC,NOLVP,NOXINST
#use delay(crystal=16000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use i2c(MASTER,FAST,SDA=PIN_C4,SCL=PIN_C3)

#use standard_io(c)

int convertirNumero(char numero[]);
int convertidorBcdABin(BYTE bcd);
int convertidorDecABcd(BYTE bcd);
void obtenerFecha(BYTE& dia, BYTE& mes, BYTE& year);
void obtenerTiempo(BYTE& hora, BYTE& minutos, BYTE& segundos);
void escribirFecha(int dia, int mes, int year);
void limpiarVariables();

int segundos,minutos,horas,dia,mes,year,contadorTimer=0,contadorCiclo=0;
int cambiarDia=0,cambiarMes=0,cambiarYear=0,indice=0;
char opcion="",numero[2];

#int_timer0
void isr_timer0(){
    if(opcion=='1'){
        contadorTimer++;
    }
    set_timer0(15536);
}
#int_RDA
void isr_serial(){
    opcion=getch();
    printf("%c",opcion);
    if(opcion=='2'){
        printf("\n\rIngrese el dia de la forma [0][1] o [3][1]:");
        numero[indice]=getch();
        numero[indice+1]=getch();
        cambiarDia=convertirNumero(numero);
        printf("\n\rIngrese el mes de la forma [0][1] o [1][2]:");
        numero[indice]=getch();
        numero[indice+1]=getch();
        cambiarMes=convertirNumero(numero);
        printf("\n\rIngrese el year de la forma [0][1] o [1][9]:");
        numero[indice]=getch();
        numero[indice+1]=getch();
        cambiarYear=convertirNumero(numero);
        escribirFecha(cambiarDia,cambiarMes,cambiarYear);
        opcion='0';
    }
}

void main(){
    setup_oscillator(OSC_16MHZ);
    setup_adc_ports(NO_ANALOGS);
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_8);
    set_timer0(15536);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    obtenerFecha(dia,mes,year);
    printf("Dia: %d Mes:%d Year:%d\n\r",dia,mes,year);
    printf("Elija opcion:\n\r[1]Leer datos\n\r[2]Escribir datos:");
    while(TRUE){
        if(opcion=='0'){
            printf("\n\rElija opcion:\n\r[1]Leer datos\n\r[2]Escribir datos:");
            opcion='x';
        }
        if(opcion=='1'){
            if(contadorTimer==10){
                obtenerTiempo(horas,minutos,segundos);
                printf("\n\rhoras:%d minutos:%d segundos:%d\n\r",horas,minutos,segundos);
                contadorCiclo++;
                if(contadorCiclo==5){
                    opcion='x';
                    printf("[0]Menu [1]Continuar");
                    contadorCiclo=0;
                }
                contadorTimer=0;
            }
        }
    }
}
void limpiarVariables(){
    contadorTimer=0;
    segundos=0;
    minutos=0;
    horas=0;
    dia=0;
    mes=0;
    year=0;
    contadorCiclo=0;
    numero[0]='0';
    numero[1]='0';
    cambiarDia=0;
}
int convertidorDecABcd(BYTE bcd){
    return (((bcd/10) << 4) | (bcd % 10));
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
    i2c_stop();
}
void obtenerTiempo(BYTE& horas, BYTE& minutos, BYTE& segundos){
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
void escribirFecha(int dia, int mes, int year){
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x04);
    i2c_write(convertidorDecABcd(dia));
    i2c_write(convertidorDecABcd(mes));
    i2c_write(convertidorDecABcd(year));
    i2c_stop();
}
int convertirNumero(char cadena[]){
    int indice=0,numero=0;
    for(indice=0;indice<2;indice++){
        numero=(numero*10)+(cadena[indice]-'0');
    }
    return numero;
}