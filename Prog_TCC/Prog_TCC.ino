/****************************************************************************************/                                                                                     
/***  PROGRAMAÇÃO - MESA INTELIGENTE AUTO-REGULADORA - LICEU DE ARTES E OFICIOS       ***/
/*** PROJETO DE CONCLUSÃO DE CURSO - 2018 - ANA CLARA - BEATRIZ MARESCA - IGOR COSTA  ***/
/****************************************************************************************/
/*** Algoritmo responsável por promover e manter a funcionalidade do protótipo da mesa **/
/****************************************************************************************/ 

#include <LiquidCrystal.h>                  // Bibliotecas que comandam o funcionamento //
#include <TimerOne.h>                       // do LCD e da interrupção.                 //

int echoPin = A0;                                       
int triggerPin = A1;                        // Foram nomeadas as portas que serão       //
int LEDH = A4;                              // usadas no projeto.                       //
int LEDA = A5;
const int pinENA = 6;  
const int pinH = 7; 
const int pinAH = 8;
int BTA = 9;
int BTH = 10;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int x = 0;                                  // Foram nomeadas as variáveis utilizadas  //
int cm = 0;                        
int Dist_obj_set = 0;                       // na programação.                         //
int Dist_new_obj = 0;
char SequenciaHA = 0;
char First_Ajuste = 0;
char First_Home = 0;
int dist_AJUSTAVEL = 0;
char b = 0;
char TEMPO = 0;
char ESTADO = 0;

long readUltrasonicDistance(int triggerPin, int echoPin) 
{
  pinMode(triggerPin, OUTPUT);              
  digitalWrite(triggerPin, LOW);            
  delayMicroseconds(2);                     // Função responsável por configurar o     //
  digitalWrite(triggerPin, HIGH);           // sensor ultrassônico para a detecção     //    
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

 void callback()                                  // Função chamada a cada interrupção.//                          
{
  cm = 0.01723 * readUltrasonicDistance(A1, A0);  // Inclui na variável cm, o valor em //
  Serial.println(cm);                             // centímetros da distância medida.  //
  x++;
  TEMPO ++;
  
  for ( ; cm > 40; )                              // Condição de segurança, quando     //
  {                                               // a medida for imprecisa, o         //
   cm = 0.01723 * readUltrasonicDistance(A1, A0); // motor será travado e o LCD        //
   Rotacao_Motor (0);                             // apresentará que há um problema.   //
   Controle_LED (0);
   Frases_LCD (5);
   delay (500);
  }
}

void Rotacao_Motor (char Rotacao)            // Função que determina o estado do motor.//
{
  if (Rotacao == 0)                          // Motor desligado. //                           
  {
   digitalWrite(pinH, LOW);
   digitalWrite(pinAH, LOW);
  }
  if (Rotacao == 1)                          // Rotação Horária (sobe). //
  {
   digitalWrite(pinH, HIGH);
   digitalWrite(pinAH, LOW);
   analogWrite(pinENA, 5000);
  }
  if (Rotacao == 2)                          // Rotação Anti-Horária (desce). //
  {
   digitalWrite(pinH, LOW);
   digitalWrite(pinAH, HIGH);
   analogWrite(pinENA, 5000);
  }
  if (Rotacao == 3)                          // Freio momentâneo. //
  {
   digitalWrite(pinH, HIGH);
   digitalWrite(pinAH, HIGH);
  }
}

void Frases_LCD (char Expressao)             // Função que determina a frase expressa no LCD. //
{
  if (Expressao == 0)
  { 
   lcd.setCursor(0, 0);
   lcd.print("Clique no botao ");
   lcd.setCursor(0, 1);
   lcd.print("      HOME      ");
  }
  if (Expressao == 1)
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("    Ajustado    ");
   lcd.setCursor(0, 1);
   lcd.print("Distancia:    cm");
   lcd.setCursor (12, 1);
   lcd.print (cm);
   x = 0;
  }
  if (Expressao == 2)
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("   AJUSTANDO .  ");
  }
  if (Expressao == 3)
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("   AJUSTANDO .. ");
  }
  if (Expressao == 4)
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("   AJUSTANDO ... ");
  }
  if (Expressao == 5)
  {
   lcd.setCursor(0, 0);
   lcd.print("   OBJETO NAO   ");
   lcd.setCursor(0, 1);
   lcd.print("  IDENTIFICADO  ");
  }
  if (Expressao == 6)
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("ESCOLHA A MEDIDA");
   lcd.setCursor(0, 1);
   lcd.print("DISTANCIA:    cm");
   lcd.setCursor (12, 1);
   lcd.print (dist_AJUSTAVEL);
  }
   if (Expressao == 7)
  {
   lcd.setCursor(0, 0);
   lcd.print(" PRIMEIRA OPCAO ");
   lcd.setCursor(0, 1);
   lcd.print("                ");
  }
   if (Expressao == 8)
  {
   lcd.setCursor(0, 0);
   lcd.print("  SEGUNDA OPCAO ");
   lcd.setCursor(0, 1);
   lcd.print("                ");
  }
}

void Controle_LED (char Cores)              // Função responsável por controlar os estados dos LEDs. //
{
  if (Cores == 0)                           // Ambos desligados. //
  { 
   digitalWrite(LEDH, LOW);
   digitalWrite(LEDA, LOW);
  }
  if (Cores == 1)                           // LED Home ligado. //
  {
   digitalWrite(LEDH, HIGH);
   digitalWrite(LEDA, LOW);
  }
  if (Cores == 2)                           // LED Ajuste ligado. //
  {
   digitalWrite(LEDH, LOW);
   digitalWrite(LEDA, HIGH);
  }
}

void setup()
{
  Timer1.initialize(400000);           // Inicializa a interrupção e configura para ser chamada em um periodo de 400 MILIsegundos.//
  Timer1.attachInterrupt(callback);    // Configura a função callback() como a função para ser chamada a cada interrupção. //   
  
  pinMode(BTA, INPUT);                 // Informa ao arduino o estado (Entrada ou Saída) das portas nomeadas. //
  pinMode(BTH, INPUT);
  pinMode(pinH, OUTPUT);
  pinMode(pinAH, OUTPUT);
  pinMode(pinENA, OUTPUT);
  pinMode(LEDH, OUTPUT);
  pinMode(LEDA, OUTPUT);
  
  lcd.begin(16, 2);                    // Inicializa o LCD para o funcionamento. //
  lcd.clear();
  
  Rotacao_Motor (0);                   // Determina o estado inicial dos dispositivos utilizados, como o motor, //   
  Frases_LCD (0);                      // o LCD e os LEDs.                                                      //
  Controle_LED (1);
     
  Serial.begin(9600);
  delay (1000);
}

void loop()
{

 if (ESTADO == 0)
 {
  if ((digitalRead(BTA) == LOW)&&(digitalRead(BTH) == HIGH)&&(SequenciaHA == 0))    // Quando for a hora do Home e o botão for apertado é valido a condição. //
  {
    for ( ; digitalRead(BTH) == HIGH; );                 // Espera o Botão ser solto. // 
   
    First_Home = 1;                                      // Grava nas variáveis que o primeiro botão Home foi apertado //
    SequenciaHA = 1;                                     // e a nova sequencia é o Ajuste.                             //                                      
    
    if ((cm > 10) && (cm < 35) && (First_Ajuste == 0))   // Quando for o primeiro Home, o motor levantará a mesa //
    {                                                    // até os 40cm (SetPoint).                              //
     for ( ; cm < 35; )
     {
      Rotacao_Motor (1);
      Frases_LCD (2);
      delay (300);
      Frases_LCD (3);
      delay (300);
      Frases_LCD (4);
      delay (300);
     }
    }
      
    if ((cm < 35) && (First_Ajuste == 1))                                 // Quando já estiver ajustado entra na condição. //
    {
      Dist_new_obj = cm;                                                  // Grava a nova distancia do objeto. //
      for ( ; (cm < (Dist_obj_set + ( Dist_new_obj - 5))) || (cm > 40); ) // Lógica usada para calcular a distância até o setpoint. //
      {
       Rotacao_Motor (1);
       Frases_LCD (2);
       delay (300);
       Frases_LCD (3);
       delay (300);
       Frases_LCD (4);
       delay (300);
      }
    }
   Rotacao_Motor (3);
  }

  if ((digitalRead(BTH) == LOW)&&(digitalRead(BTA) == HIGH)&&(cm < 25)&&(SequenciaHA == 1)) // Quando for a hora de Ajustar, se o botão for apertado e tiver alguém, entra na condição. //
   {
    for ( ; digitalRead(BTA) == HIGH; );
    Dist_obj_set = cm;                                           // Grava a medida do SetPoint até o objeto. //
    First_Ajuste = 1;
    SequenciaHA = 0;
    
    for ( ; cm > 5; )                                            // Enquanto a distância for maior que cinco centímetros o motor funciona. //
    {
     Rotacao_Motor (2);
     Frases_LCD (2);
     delay (300);
     Frases_LCD (3);
     delay (300);
     Frases_LCD (4);
     delay (300);
    }
    Rotacao_Motor (3);
   }
 }
   

 if (ESTADO == 1)
 {
  if (b == 0)
   {
     dist_AJUSTAVEL = 4;
     TEMPO = 0;
     while (TEMPO < 10)
     {
        if (digitalRead(BTA) == HIGH)
        {
         for ( ; digitalRead(BTA) == HIGH; );
         dist_AJUSTAVEL ++;
         TEMPO = 0;
         if (dist_AJUSTAVEL == 11)
          {
           dist_AJUSTAVEL = 4;
          }
        }
        Frases_LCD (6);
        delay (500);
     }
     b = 1;
     delay(2000);
   }


  if ((digitalRead(BTA) == LOW)&&(digitalRead(BTH) == HIGH) && (SequenciaHA == 0))    // Quando for a hora do Home e o botão for apertado é valido a condição. //
  {
    for ( ; digitalRead(BTH) == HIGH; );                 // Espera o Botão ser solto. // 
    SequenciaHA = 1;                                     // e a nova sequencia é o Ajuste.                             //                                      
      
    if (cm < 35)                                 // Quando já estiver ajustado entra na condição. //
    {
      Dist_new_obj = cm;                                                  // Grava a nova distancia do objeto. //
      for ( ; (cm < (Dist_obj_set + ( Dist_new_obj - dist_AJUSTAVEL))) || (cm > 40); ) // Lógica usada para calcular a distância até o setpoint. //
      {
       Rotacao_Motor (1);
       Frases_LCD (2);
       delay (300);
       Frases_LCD (3);
       delay (300);
       Frases_LCD (4);
       delay (300);
      }
    }
   Rotacao_Motor (3);
  }

  
   if ((digitalRead(BTH) == LOW)&&(digitalRead(BTA) == HIGH) && (cm < 30) && (SequenciaHA == 1)) // Quando for a hora de Ajustar, se o botão for apertado e tiver alguém, entra na condição. //
   {
    for ( ; digitalRead(BTA) == HIGH; );
    Dist_obj_set = cm; 
    SequenciaHA = 0;
    
    for ( ; cm > dist_AJUSTAVEL; )                                            // Enquanto a distância for maior que cinco centímetros o motor funciona. //
    {
     Rotacao_Motor (2);
     Frases_LCD (2);
     delay (300);
     Frases_LCD (3);
     delay (300);
     Frases_LCD (4);
     delay (300);
    }
    Rotacao_Motor (3);
   } 
 }

 if ((digitalRead(BTA) == HIGH) && (digitalRead(BTH) == HIGH) && (SequenciaHA == 1) && (First_Home == 1))
 {
     while (BTA == HIGH);
     while (BTH == HIGH);
     if (ESTADO == 1)
     {
      ESTADO = 0;
      Frases_LCD (7);
     }
     else
     {
      ESTADO = 1;
      Frases_LCD (8);
     }
     delay(2000);
     b = 0;
 }
                                                 
   Rotacao_Motor (0);
                                      //Depois do primeiro botão HOME apertado, aparece a distância da mesa no LCD.//
   if (SequenciaHA == 0)
   {
    Controle_LED (1);
   }
   if (SequenciaHA == 1)
   {
    Controle_LED (2);
   }
   if ((First_Home == 0)&& (x > 3))
   {
    Frases_LCD (0);
   }
   if ((First_Home == 1) && (x > 3))
   {
    Frases_LCD (1);
   }
   TEMPO = 0;
}
