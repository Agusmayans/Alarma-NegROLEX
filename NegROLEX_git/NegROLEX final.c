//NegROLEX : RELOJ "BOMBA" DESARROLLADO POR EL GRUPO 8

#INCLUDE <16F873A.h>//incluye la biblioteca del pic
#FUSES XT,NOWDT //indicas que el osc es de cristal y desactivas el watch doge
#USE DELAY (CLOCK=4000000)//pones la frec ext de cristal
#USE FAST_IO(A)//pones que vas a usar los puertos 
#USE FAST_IO(B)
#USE FAST_IO(C)




int seg=0, min=0, horas=0, ms=0, min2, horas2, umin, dmin, uhoras, dhoras, multiplex=0;//estas son las variables globales
int cont=0, horas3=10, min3=30, segr, minr, dseg, useg, seg2, suma=0, supercont, traba;

#int_timer1 //apartir de aca escribis la funcion 
void timer1() //escribis el nombre y tipo de la funcion 
{                    
       ms=ms+1;//aumentas la variable ms en 1
       
   if (ms == 4) //preguntas si ms es = a 4
        {
            seg++; //aumenta los seg
            segr--; //disminuye los seg
            ms=0;  //reinicia la variable seg
            output_toggle(pin_a5); //cambias el estado del pin a5 (los puntitos del medio)
        }
   if (segr == (-1))
        {
            minr--;
            segr=59;
        }
   if (seg==57) //if encargado de aumentar los min
        {
            min++; //aumenta los min
            seg=0; //reseta la variabke seg
        }
   if (min == 60) //if encargado de aumentar las horas
        {
            horas++; //aumenta las horas
            min=0;   //resetea la variable min
        }        
        
   if (horas == 24) //if encargado de resetear la variable horas
        {
            horas=0; //resetea la variable horas
        }

}

#int_timer0 // a partir de aca pones la funcion q va a hacer
void timer0() //escribis el tipo y nombre de la funcion
{
   multiplex++; //aumenta en 1 la variable multiplex

   if (multiplex==4) //pregunta si multiplex es 4
   {
   multiplex=0; //reinicia multiplex
   }
}

#int_ext
void sumador_piolita()
{
#ASM
          movlw    2      //cargo w con 2
          sublw    suma       //le resto w a suma
          btfss    03,2   //pregunta si la resta da 0 o no
          goto     cereador    //si no da 0 tengo que llegar a 2 en suma
          sumador: 
          movlw    1       //cargo w con 1
          movwf    suma     //cargo a suma con 1 activando la interrupcion
          bcf      0x0B,1   //limpio el flag de la interrupcion
          goto     fin     //termina la interrupcion
          cereador:          //esta subrutina pone en 0 la variable
          movlw    0
          movwf    suma
          goto     sumador    //aca termina la subrutina y le suma un 1 para activar la interrupcion
          fin:      
          RETFIE
#ENDASM
          
}


void multiplexacion() //esta funcion se encargade multiplexar los displays
{ 

        dmin=min/10; //dmin pasa a ser el la decena de los min
        umin=min%10; //umin pasa a ser la unidad de los min

        dhoras=horas/10; //dhoras pasa a ser el la decena de los horas
        uhoras=horas%10; //uhoras pasa a ser la unidad de los horas
        
        if (multiplex==0) //pregunta si multiplex es 0
        {
        min2=umin+0b00010000; //le suma a la variable el bit de multiplexacion correspondiente
        output_c(min2); //mandas el dato al display
        }
        
        if (multiplex==1) //pregunta si multiplex es 1
        {
        min2=dmin+0b00100000; //le suma a la variable el bit de multiplexacion correspondiente
        output_c(min2); //mandas el dato al display
        }
               
        
        if (multiplex==2) //pregunta si multiplex es 2
        {
        horas2=uhoras+0b01000000; //le suma a la variable el bit de multiplexacion correspondiente
        output_c(horas2); //mandas el dato al display
        }
       
        if (multiplex==3) //pregunta si multiplex es 3
        {
        horas2=dhoras+0b10000000; //le suma a la variable el bit de multiplexacion correspondiente
        output_c(horas2); //mandas el dato al display
        }           
}   

void multiplexacion2() //es la funcion de muktiplexacion para la alarma
{   
    
        dmin=min3/10;
        umin=min3%10;

        dhoras=horas3/10;
        uhoras=horas3%10;
        
        if (multiplex==0)
        {
        min2=umin+0b00010000;
        output_c(min2);
        }
        
        if (multiplex==1)
        {
        min2=dmin+0b00100000;
        output_c(min2);
        }
                 
        if (multiplex==2)
        {
        horas2=uhoras+0b01000000;
        output_c(horas2);
        }
       
        if (multiplex==3)
        {
        horas2=dhoras+0b10000000;
        output_c(horas2);
        }
        
        output_low(pin_a5);    
}       

void multiplexacion_reg()
{
        dmin=minr/10;
        umin=minr%10;

        dseg=segr/10;
        useg=segr%10;
       
        if (multiplex==0)
        {
        min2=umin+0b01000000;
        output_c(min2);
        }
        
        if (multiplex==1)
        {
        min2=dmin+0b10000000;
        output_c(min2);
        }
                 
        if (multiplex==2)
        {
        seg2=useg+0b00010000;
        output_c(seg2);
        }
       
        if (multiplex==3)
        {
        seg2=dseg+0b00100000;
        output_c(seg2);
        }
        
        output_low(pin_a5);    
}

void jueguembler()
{
 minr=0;
 segr=30;
 supercont=0;
 
while(supercont==0)
 {
 traba=0;
 multiplexacion_reg();
 if (minr==0 && segr==0)
  {  
    while (traba==0)
    {
     output_toggle(pin_a1);
     delay_ms(250);
     if (input(pin_b5)==0)
      {
       suma++;  
       supercont++;
       traba++;
       output_low(pin_a1);
      }
    }
  }
 if (input(pin_b3)==0)
  {
    segr=segr-2;
    while (traba==0)
    {
     output_toggle(pin_a1);
     delay_ms(250);
     if (input(pin_b4)==0)
      {
       suma++;  
       supercont++;
       traba++;
       output_low(pin_a1);
      }
    }
  }
 if (input(pin_b4)==0)
  {
     suma++;  
     supercont++;
     output_low(pin_a1);
  }
 if (input(pin_b5)==0)
  {
    segr=segr-10;
    while (traba==0)
    {
     output_toggle(pin_a1);
     delay_ms(250);
     if (input(pin_b3)==0)
      {
       suma++;  
       supercont++;
       traba++;
       output_low(pin_a1);
      }
    }
  }
 }
}
void main()
{
   SET_TRIS_C(0b00000000); //seteas las I/O del puerto C
   SET_TRIS_B(0b11111111); //seteas las I/O del puerto B
   SET_TRIS_A(0b001101);   //seteas las I/O del puerto A
   ENABLE_INTERRUPTS(INT_TIMER0);
   ENABLE_INTERRUPTS(INT_TIMER1);
   ENABLE_INTERRUPTS(INT_EXT);
   ENABLE_INTERRUPTS(GLOBAL);
   EXT_INT_EDGE(H_TO_L);
   SETUP_TIMER_0 (RTCC_INTERNAL|RTCC_DIV_16);
   SETUP_TIMER_1 (T1_INTERNAL|T1_DIV_BY_4);
   SET_TIMER0(131);
   SET_TIMER1(3036); 
   
 while (true)
 {
   //Programa Principal
    while (suma==1)
    {
    jueguembler();
    }
    
   //INGRESO DE LA HORA PARA ARRANCAR EL PROGRAMA
    
    while (cont == 0)
    {
    if (input(pin_b1)==0)
     {
     horas++;
     delay_ms(150);
     if (horas == 24)
      {
      horas = 0;
      }
     }
     if (input(pin_b2)==0)
     { 
     cont=cont+1;
     delay_ms(150);
     }
     multiplexacion();
    }
   
   while (cont == 1)
    {
    if (input(pin_b1)==0)
     {
     min++;
     delay_ms(150);
     
     if (min == 60)
      {
      min=0;
      }
     }
     
     if (input(pin_b2)==0)
     { 
     cont++;
     delay_ms(150);
     }
     multiplexacion();
    } 
    //SETEO DE LA ALARMA
   while (cont == 3)
    {
    if (input(pin_b1)==0)
     {
     horas3++;
     delay_ms(150);
     if (horas3 == 24)
      {
      horas3 = 0;
      }
     }
     if (input(pin_b2)==0)
     { 
     cont++;
     delay_ms(200);
     }
     multiplexacion2();
    }
   
   while (cont == 4)
    {
    if (input(pin_b1)==0)
     {
     min3++;
     delay_ms(150);
     if (min3 == 60)
      {
      min3=0;
      }
     }
     if (input(pin_b2)==0)
     { 
     cont++;
     delay_ms(200);
     }
     multiplexacion2();
    } 
    //PROGRAMA ACTIVO
    multiplexacion();
        
    if (input(pin_b2)==0)
     { 
     cont++;
     delay_ms(200);
     }
   
   if (cont == 5)
     {
     cont=2;
     }
   
  //ALARMA
  if (min==min3 && horas==horas3)
     {
     output_toggle(pin_a1);
     delay_ms(500);
      if (input(pin_b4)==0)
      {
       output_low(pin_a1);
       horas3=0;
       min3=0;
      }
     }
 }
}
