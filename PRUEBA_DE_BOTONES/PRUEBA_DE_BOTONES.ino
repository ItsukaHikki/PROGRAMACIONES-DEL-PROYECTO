const int bcancelar = 13;
const int bcloro = 12; //precio 10
const int bfabuloso = 11; // precio 15
const int bsalvo = 9; //precio 12
const int bmrmusculo = 7; // precio 16
const int bpinol = 5;// precio 14


unsigned long botones;
int CreditoAcum = 60;
int bestadoc = 0; 


void setup() 

{
  Serial.begin (9600);
  pinMode (13,INPUT);
  pinMode (12, INPUT);
  pinMode (11, INPUT);
  pinMode (9, INPUT);
  pinMode (7, INPUT);
  pinMode (5, INPUT);
  botones=millis();
}

void loop() 
{

if(millis()-botones >= 200) 
{
if (digitalRead(bcancelar) == HIGH && bestadoc >= 0) //INICIO DE CANCELAMIENTO DE COMPRA
{
CreditoAcum = 0;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
}
if (bestadoc==1 && digitalRead(bcancelar)==LOW){bestadoc=0;} //FIN DE CANCELAMIENTO DE COMPRA

  
if (digitalRead(bcloro) == HIGH && bestadoc == 0 && CreditoAcum>=10) //INICIO DE COMPRA DEL CLORO
{
CreditoAcum = CreditoAcum - 10;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
}
if (bestadoc==1 && digitalRead(bcloro)==LOW){bestadoc=0;} //FIN DE COMPRA DEL CLORO
}

if (digitalRead(bfabuloso) == HIGH && bestadoc == 0 && CreditoAcum>= 15) //INICIO DE COMPRA DEL FABULOSO
{
CreditoAcum = CreditoAcum - 15;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
}
if (bestadoc==1 && digitalRead(bfabuloso)==LOW){bestadoc=0;} //FIN DE COMPRA DEL FABULOSO


if (digitalRead(bsalvo) == HIGH && bestadoc == 0 && CreditoAcum>= 12) //INICIO DE COMPRA DEL SALVO
{
CreditoAcum = CreditoAcum - 12;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
}
if (bestadoc==1 && digitalRead(bsalvo)==LOW){bestadoc=0;} //FIN DE COMPRA DEL SALVO

if (digitalRead(bmrmusculo) == HIGH && bestadoc == 0 && CreditoAcum>=16) //INICIO DE COMPRA DEL MRMUSCULO
{
CreditoAcum = CreditoAcum - 16;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
}
if (bestadoc==1 && digitalRead(bmrmusculo)==LOW){bestadoc=0;} //FIN DE COMPRA DEL MRMUSCULO

if (digitalRead(bpinol) == HIGH && bestadoc == 0 && CreditoAcum>=14 ) //INICIO DE COMPRA DEL PINOL
{
CreditoAcum = CreditoAcum - 14;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
}
if (bestadoc==1 && digitalRead(bpinol)==LOW){bestadoc=0;} //FIN DE COMPRA DEL PINOL



}
