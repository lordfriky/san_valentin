//////////////////////////////////////////////
//                                          //
//  Proyecto de San Valentin - Curso ARACT  //
//          Daniel Hernández Muñoz          //
//           6 de febrero de 2021           //
//                                          //
//////////////////////////////////////////////

/*  Cabeceras  */
#include "pitches.h"
#include "careless_whisper.h"
#include "anim_uno.h"
#include "anim_dos.h"

/*  Pines  */
#define PIN_BOTON 2
#define PIN_BUZZER 3
#define PIN_LED01 4 // Numerados en sentido de las manecillas del reloj
#define PIN_LED02 5
#define PIN_LED03 6
#define PIN_LED04 7
#define PIN_LED05 8
#define PIN_LED06 9
#define PIN_LED07 10
#define PIN_LED08 11
#define PIN_LED09 12
#define PIN_LED10 13

/*  Variables y constantes  */
unsigned long tiempo;

int contadorNotas = 0;
bool nota = true;
long frecuenciaNota; // Lo usaré para una animación
long duracionNota;
unsigned long ultimaNota;

bool silencio = false;
long duracionSilencio;
unsigned long ultimoSilencio;

int numeroAnimacion = 1; // Empezaremos con la primera animación
int contadorAcciones = 0;
int cantidadAcciones;
long duracionAccion;
unsigned long ultimaAccion;
int accion[10];

void setup() {
  pinMode(PIN_BOTON, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED01, OUTPUT);
  pinMode(PIN_LED02, OUTPUT);
  pinMode(PIN_LED03, OUTPUT);
  pinMode(PIN_LED04, OUTPUT);
  pinMode(PIN_LED05, OUTPUT);
  pinMode(PIN_LED06, OUTPUT);
  pinMode(PIN_LED07, OUTPUT);
  pinMode(PIN_LED08, OUTPUT);
  pinMode(PIN_LED09, OUTPUT);
  pinMode(PIN_LED10, OUTPUT);
  
  // Reproducimos la primera nota
  frecuenciaNota = melodia[contadorNotas];
  ultimaNota = 0;
  duracionNota = 1000 / tempo[contadorNotas];
  tone(PIN_BUZZER, frecuenciaNota);

  ultimaAccion = 0;
  cantidadAcciones = sizeof(anim_uno_acciones) / sizeof(int[10]);
  duracionAccion = anim_uno_tiempos[contadorAcciones];
  ejecutarAccion(anim_uno_acciones[contadorAcciones]);
}

void loop() {
  tiempo = millis(); 

  int boton = digitalRead(PIN_BOTON);

  if(boton==HIGH){
    delay(150); // Sino lo va cambiar multiples veces y puede que no avance al que queramos

    numeroAnimacion++;
    if(numeroAnimacion > 3){
      numeroAnimacion = 1;
    }
    
    contadorAcciones = 0;
    ultimaAccion = tiempo;

    if(numeroAnimacion == 1){
      cantidadAcciones = sizeof(anim_uno_acciones) / sizeof(int[10]);
      duracionAccion = anim_uno_tiempos[contadorAcciones];
      ejecutarAccion(anim_uno_acciones[contadorAcciones]);
    } else if(numeroAnimacion == 2){
      cantidadAcciones = sizeof(anim_dos_acciones) / sizeof(int[10]);
      duracionAccion = anim_dos_tiempos[contadorAcciones];
      ejecutarAccion(anim_dos_acciones[contadorAcciones]);
    } else if(numeroAnimacion == 3){ // Espectro de audio (los leds encienden conforme a la canción)
      cantidadAcciones = 32; // No es realmente necesario esto pero meh
      if(nota){
        ultimaAccion = ultimaNota;
        duracionAccion = duracionNota;

        int accion[10];        

        if(frecuenciaNota == 220 || frecuenciaNota == 233 || frecuenciaNota == 262){ // No se puede usar switch case porque entonces no podriamos declarar accion
          int acciones[10] = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }; // No puedo pasar directamente el array ni declararlo adentro del if
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 294 || frecuenciaNota == 330 || frecuenciaNota == 349){
          int acciones[10] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 392 || frecuenciaNota == 440){
          int acciones[10] = { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 466 || frecuenciaNota == 523){
          int acciones[10] = { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 587 || frecuenciaNota == 659){
          int acciones[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        }

        ejecutarAccion(accion);
      } else if(silencio){
        ultimaAccion = ultimoSilencio;
        duracionAccion = duracionSilencio;
        int accion[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Tengo que declararlo primero en una variable porque no me deja pasarlo como { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } a la funcion
        ejecutarAccion(accion);
      }
    }
  }

  if(nota){
    if(tiempo - ultimaNota >= duracionNota){
      nota = false;

      frecuenciaNota = 0;
      ultimoSilencio = tiempo;
      duracionSilencio = duracionNota * 1.30; // Aplicamos un silencio de x1.3 de duración de la nota para que se perciba mejor
      
      noTone(PIN_BUZZER);
      silencio = true;
    }
  } else if(silencio){
    if(tiempo - ultimoSilencio >= duracionSilencio){
      silencio = false;
      
      contadorNotas++;
      if(contadorNotas == 32){
        contadorNotas = 0;
      }

      frecuenciaNota = melodia[contadorNotas];
      ultimaNota = tiempo;
      duracionNota = 1000 / tempo[contadorNotas];
      
      tone(PIN_BUZZER, frecuenciaNota);
      nota = true;
    }
  }

  if (tiempo - ultimaAccion >= duracionAccion){
    contadorAcciones++;
    if(contadorAcciones == cantidadAcciones){
      contadorAcciones = 0;
    }

    ultimaAccion = tiempo;
    
    if(numeroAnimacion == 1){
      duracionAccion = anim_uno_tiempos[contadorAcciones];
      ejecutarAccion(anim_uno_acciones[contadorAcciones]);
    } else if(numeroAnimacion == 2){
      duracionAccion = anim_dos_tiempos[contadorAcciones];
      ejecutarAccion(anim_dos_acciones[contadorAcciones]);
    } else if(numeroAnimacion == 3){
      if(nota){
        duracionAccion = duracionNota;

        int accion[10];        

        if(frecuenciaNota == 220 || frecuenciaNota == 233 || frecuenciaNota == 262){ // No se puede usar switch case porque entonces no podriamos declarar accion
          int acciones[10] = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }; // No puedo pasar directamente el array ni declararlo adentro del if
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 294 || frecuenciaNota == 330 || frecuenciaNota == 349){
          int acciones[10] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 392 || frecuenciaNota == 440){
          int acciones[10] = { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 466 || frecuenciaNota == 523){
          int acciones[10] = { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        } else if(frecuenciaNota == 587 || frecuenciaNota == 659){
          int acciones[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
          for(int i = 0; i < 10; i++){
            accion[i] = acciones[i];
          } 
        }

        ejecutarAccion(accion);
      } else if(silencio){
        duracionAccion = duracionSilencio;
        int accion[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Tengo que declararlo primero en una variable porque no me deja pasarlo como { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } a la funcion
        ejecutarAccion(accion);
      }
    }
  }
}

void ejecutarAccion(int leds[10]){
  digitalWrite(PIN_LED01, leds[0]); // Igual puedo hacer un ciclo for pero por los nombres de las variables de los pines meh
  digitalWrite(PIN_LED02, leds[1]);
  digitalWrite(PIN_LED03, leds[2]);
  digitalWrite(PIN_LED04, leds[3]);
  digitalWrite(PIN_LED05, leds[4]);
  digitalWrite(PIN_LED06, leds[5]);
  digitalWrite(PIN_LED07, leds[6]);
  digitalWrite(PIN_LED08, leds[7]);
  digitalWrite(PIN_LED09, leds[8]);
  digitalWrite(PIN_LED10, leds[9]);
}
