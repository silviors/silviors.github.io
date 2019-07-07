#include <Keypad.h> //INCLUSÃO DE BIBLIOTECA
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <VirtualWire.h> //Insere a biblioteca VirtualWire
#include "Wire.h"


char recebido_RF_char[9];
byte message[VW_MAX_MESSAGE_LEN];    // Armazena as mensagens recebidas
byte msgLength = VW_MAX_MESSAGE_LEN; // Armazena o tamanho das mensagens

const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 4; //QUANTIDADE DE COLUNAS DO TECLADO

unsigned long cronometro = 0.0, cronometroDHT = 0.0, cronometroenvioSens = 0.0 ;

const int pinoDHT11 = A7;
dht DHT;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//SDA = A4 E SCL = A5


int LDR = A2, VLDR, temperature, tcod, valor_recebido_RF, acao;
String tecla = "", stringrecebidoRF = " ", mtecla, luz, tempexterna, tempinterna, hora, data, sensor;



//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

String acoes[] = {"ALARM DISPARADO", "PORTAO", "LUZ PORTAO", "LUZ GARAGEM", "LUZ HALL", "LUZ SALA TV",
                  "LUZ VARANDA", "LUZ ESC EXTERNA", "LUZ JARD FRENTE", "REFLETOR FRENTE", "LUZ EDICULA", "LUZ EDI LATERAL",
                  "LUZ EDIC JARDIM", "LUZ FUND JARDIM", "ALARME", "SIRENE", "LUZ OFICINA",
                  "LUZ REFLET MURO", "LUZ PORAO", "19", "TIMER JARDIM", "CAMPAINHA", "TIMER REF FRENT"
                 };


byte PinosqtdLinhas[qtdLinhas] = {9, 8, 7, 6}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {5, 4, 3, 2}; //PINOS UTILIZADOS PELAS COLUNAS

//INICIALIZAÇÃO DO TECLADO
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas);



// endereco do modulo slave que pode ser um valor de 0 a 255
#define myAdress 0x08
char Valor_CharMsg[20]; //variavel a ser transmitida via rf
int received = 0;
byte ndado = 0;


void setup() {
  Serial.begin(9600); //INICIALIZA A SERIAL
  lcd.begin(20, 4);
  pinMode(LED_BUILTIN, OUTPUT);

  lcd.setCursor(6, 0);
  lcd.print("HORARIO");

  lcd.setCursor(2, 1);
  lcd.print("CODIGOS E ACOES");

  lcd.setCursor(2, 2);
  lcd.print("# ENVIAR TECLADO");
  delay(3000);
  lcd.clear();

  lcd.setCursor(2, 1);
  lcd.print("CODIGOS E ACOES");

  lcd.setCursor(2, 2);
  lcd.print("# ENVIAR TECLADO");

  vw_set_tx_pin(A0);
  vw_set_rx_pin(A1); // Define o pino 5 do Arduino como entrada

  vw_setup(5000);   // Bits per sec
  vw_rx_start();

}

void loop() {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen))
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    for (int i = 0; i < buflen; i++)
    {
      //Armazena os caracteres recebidos
      recebido_RF_char[i] = char(buf[i]);
    }
    recebido_RF_char[buflen] = '\0';
    stringrecebidoRF = recebido_RF_char;
    tcod = stringrecebidoRF.length();
    Serial.println(stringrecebidoRF);
    cronometroenvioSens = millis();

    if (tcod == 3) {
      valor_recebido_RF = stringrecebidoRF.substring(0, 2).toInt();
      acao = stringrecebidoRF.substring(2, 3).toInt();
      lcd.setCursor(0, 1);
      lcd.print("C:");
      lcd.print(stringrecebidoRF);
      if (acao == 0) lcd.print(" +"); else lcd.print(" -");
      lcd.print(acoes[valor_recebido_RF]);
    }
    else   if (tcod == 8)
      hora = (stringrecebidoRF.substring(0, 5));
    else   if (tcod == 7)
      data = stringrecebidoRF;
    else   if (tcod == 4)
      if (stringrecebidoRF.substring(0, 1) == "1")
        luz = stringrecebidoRF.substring(1);

      else if (stringrecebidoRF.substring(0, 1) == "2")
        tempexterna = stringrecebidoRF.substring(2);

      else if (stringrecebidoRF.substring(0, 1) == "3")
        tempinterna = stringrecebidoRF.substring(2);










    //    int str_len = stringrecebidoRF.length() + 1;
    //    char Valor_CharMsg[str_len];
    //    stringrecebidoRF.toCharArray(Valor_CharMsg, str_len);
    //    for (int i = 0; i < 20; i++) {
    //      vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));    //Envio dos dados
    //      vw_wait_tx();//Aguarda envio dos dados
    //    }

  }









  char tecla_pressionada = meuteclado.getKey(); //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA

  if (tecla_pressionada) { //SE ALGUMA TECLA FOR PRESSIONADA, FAZ
    if (tecla_pressionada == '*') {
      tecla = "";
      mtecla = tecla;
      lcd.setCursor(0, 2);
      lcd.print("# ENVIAR TECLADO   ");

    } else if (tecla_pressionada == '#') {
      lcd.setCursor(0, 2);
      lcd.print("ENVIADO:"); //IMPRIME O TEXTO NO MONITOR SERIAL
      lcd.print(tecla); //IMPRIME NO MONITOR SERIAL A TECLA PRESSIONADA
      lcd.print("   ");
      int str_len = tecla.length() + 1;
      char Valor_CharMsg[str_len];
      tecla.toCharArray(Valor_CharMsg, str_len);
      mtecla = tecla;
      tecla = "";


      for (int i = 0; i < 20; i++) {
        vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));    //Envio dos dados
        vw_wait_tx();//Aguarda envio dos dados
      }
      delay(1000);
      lcd.setCursor(0, 2);
      lcd.print("# ENVIA ULT: ");
      lcd.print(mtecla);
      lcd.print("  ");


    } else {
      lcd.setCursor(0, 2);
      lcd.print("T:"); //IMPRIME O TEXTO NO MONITOR SERIAL
      lcd.print(tecla_pressionada); //IMPRIME NO MONITOR SERIAL A TECLA PRESSIONADA
      tecla = tecla + tecla_pressionada;
      lcd.print(" #ENV*CANC:");
      lcd.print(tecla);
      lcd.print("  ");
    }
  }

  if (millis() - cronometroDHT > 30000) {
    DHT.read11(pinoDHT11);
    lcd.setCursor(0, 1);
    lcd.print("DHT T:");
    lcd.print(DHT.temperature);
    lcd.print("*C U:");
    lcd.print(DHT.humidity);
    lcd.print("%");
    cronometroDHT = millis();

  }


  //if (millis() - cronometroenvioSens == 10000) enviasensores(VLDR, 1);
  //if (millis() - cronometroenvioSens == 15000) enviasensores(DHT.humidity, 2);
  //if (millis() - cronometroenvioSens == 25000) enviasensores(DHT.temperature, 3);

  if (millis() - cronometroenvioSens  > 10000) {
    lcd.setCursor(0, 0);
    lcd.print("H:");
    lcd.print(hora);


    lcd.setCursor(8, 0);
    lcd.print(tcod);


    lcd.setCursor(10, 0);
    lcd.print(data.substring(1, 3));
    lcd.print("/");
    lcd.print(data.substring(3, 5));
    lcd.print("/20");
    lcd.print(data.substring(5));



    lcd.setCursor(0, 3);
    lcd.print("LZ:");
    lcd.print(luz);
    lcd.print(" E:");
    lcd.print(tempexterna);
    lcd.print("*C I:");
    lcd.print(tempinterna);
    lcd.print("*C");


    cronometroenvioSens = millis();

    digitalWrite(LED_BUILTIN, LOW);
  }


  if (ndado > 0) {
    for (int i = 0; i < 20; i++) {
      vw_send((uint8_t *)Valor_CharMsg, ndado);    //Envio dos dados
      vw_wait_tx();//Aguarda envio dos dados
    }
    ndado = 0;
  }


}







void enviasensores(int valorenvio, int prefixoenvio) {
  String codigoenvio;
  if (valorenvio < 10) {
    codigoenvio = String(prefixoenvio * 100);
  } else if (valorenvio < 100) {
    codigoenvio = String(prefixoenvio * 10);
  } else if (valorenvio < 1000) {
    codigoenvio = String(prefixoenvio);
  }
  String sensor_CharMsg = String(valorenvio);
  sensor_CharMsg = codigoenvio + sensor_CharMsg;
  char sensor_CharMsg_envio[5];
  for (int i = 0; i < sensor_CharMsg.length(); i++) sensor_CharMsg_envio[i] = char(sensor_CharMsg[i]);
  sensor_CharMsg_envio[sensor_CharMsg.length()] = '\0';
  for (int i = 0; i < 10; i++) {
    vw_send((uint8_t *)sensor_CharMsg_envio, strlen(sensor_CharMsg_envio));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }
}




// funcao executada sempre que algum dado e recebido no barramento I2C
void dadorecebido()
{
  ndado = 0; //ponemos a 0 la variable de conteo
  while (Wire.available() > 0)   {
    char dado = Wire.read(); // Recibe la palabra del i2c tx
    if (dado)   {
      Valor_CharMsg[ndado] = dado;//guarda el caracter recibido en una poscicion del vector
      ndado++;//incrementamos el contador
    }
    else
      break;//al finalizar la transmicion rompemos el while
  }
  for (byte i = 0; i < ndado; i++) { //imprimimos lo que hay en el vector
    Serial.print(Valor_CharMsg[i]);
  }
  Serial.println(" ");
}



