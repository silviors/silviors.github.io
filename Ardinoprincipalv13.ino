#include <Wire.h>
#include <VirtualWire.h>
#include <DS3231.h>      //Biblioteca para manipulação do DS3231
#include <SPI.h>
#include <UIPEthernet.h>
#include <UIPClient.h>
#include "thermistor.h"
#define timer 1000
#define tamlog 20
#define horanoite 17
#define horadia 1
#define portaonoite 22 // abre ate as 22:59
#define portaodia 6 //abre apos as 6:00

String readString;

boolean ligado_1 = true;
boolean ligado_2 = true;
boolean ligado_3 = true;
boolean ligado_4 = true;
boolean ligado_5 = true;
boolean ligado_6 = true;
boolean ligado_7 = true;
boolean ligado_8 = true;
boolean ligado_9 = true;
boolean ligado_10 = true;
boolean ligado_11 = true;
boolean ligado_12 = true;
boolean ligado_13 = true;
boolean ligado_14 = true;
boolean ligado_15 = true;
boolean ligado_16 = true;
boolean ligado_17 = true;
boolean ligado_18 = true;
boolean ligado_19 = true;




//--------------------------------------------------------------------------------------------------------------------------------------

//portas livres             xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int livre01 = 0, livre02 = 1, livre03 = 6, livre04 = 7, livre05 = 8, livre06 = 9, livre07 = 10, livre08 = 13;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//porta motores persianas    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int persiana01, persiana02, persiana03, persiana04;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas proximas a central xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int emailalarme = 14, emailinterfone = 15, campainha = 54, emailcerca = 34, buzina = 17, relealarme = 16, rftrans = 19, rfrecpt = 18, hora01 = 20, hora02 = 21;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas frente superior casa xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int luzportao = 22, luzgaragem = 23, luzmezanino = 24, luzsalatv = 25, portao = 26, reserva04 = 27;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas fundos inferior xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int luzvarandalateral = 28, luzvaranda = 29, luzjardimfrente = 30, luzjardimfrente2 = 31, dicroicassala = 32, reserva06 = 33;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas edicula e quintal xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//int luzedicula = 34, luzlateraledicula = 35, luzjardimedicula = 36, luzjardimfundos = 37, luzoficina = 38, reserva08 = 39;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas circuito disponivel 1  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int reserva09 = 40, reserva10 = 41, reserva11 = 42, reserva12 = 43, reserva13 = 44, reserva14 = 45, reserva15 = 46, reserva16 = 47;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas circuito disponivel 2  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int reserva17 = 48, reserva18 = 49;// ENC28J60-SO=50, ENC28J60-SI=51, ENC28J60-SCK=52, ENC28J60-CS=53;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas lcd                 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int lcd01 = 12, lcd02 = 11, lcd03 = 5, lcd04 = 4, lcd05 = 3, lcd06 = 2;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// portas analogicas disponiveis        xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//int a0 = 54, a1 = 55, a2 = 56, a3 = 57, a4 = 58, a5 = 59, a6 = 60, a7 = 61, a8 = 62, a9 = 63, a10 = 64, a11 = 65, a12 = 66, a13 = 67, a14 = 68, a15 = 69;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx




//--------------------------------------------------------------------------------------------------------------------------------------

//lcd portas LiquidCrystal lcd(<pino RS>, <pino enable>, <pino D4>, <pino D5>, <pino D6>, <pino D7>)          xxxxxxxxxxxxxxxxxxxxxxxxxx
//LiquidCrystal lcd(lcd01, lcd02, lcd03, lcd04, lcd05, lcd06); //lcd portas
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



// modulo hora e data                   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
DS3231  rtc(SDA, SCL);
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



//Definicoes de IP, mascara de rede e gateway     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
byte mac[] = { 0x84, 0x42, 0x8B, 0xBA, 0xB2, 0x31 };
IPAddress ip(192, 168, 0, 160);       //Define o endereco IP
IPAddress gateway(192, 168, 0, 1);   //Define o gateway
IPAddress DNS(192, 168, 0, 1);   //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



//Inicializacao de servidor e smtp                xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
byte serversmtp[] = { 43, 228, 184, 6 }; // Colocar o ip do servidor SMTP
EthernetClient client;
EthernetServer server(80);
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


//Inicializacao de outros                xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
char Valor_CharMsg[9]; //variavel a ser transmitida via rf
char recebido_RF_char[9];
String stringrecebidoRF = "NADA";
int valorlido = 500; //acao a ser tomada pela web
int controlerf = valorlido; //verifica se mudou o status para envio rf
unsigned long intervaloalarme = 300000; //intervalo de envio de email
long tempo = -300000; //variavel para controle de email
int  controleportao = 1, ctltimer = 1, minuto = 0, minutoportao = 0, 
sensorLDR = A1, sensorAlarme = A9, sensorTemp = A3, inthora; //controle timer jardim e hora portao
unsigned int luminosidade = 1000, LDRnoite = 75, temperature;
int codlog = 0, controletemp = 0, controleLDR = 0;
String logs[tamlog], logscontrole = "";
THERMISTOR temp(sensorTemp, 10000, 3950, 10000);

int repetenvio = 100;


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx




void setup() {

  Serial.begin(9600);

  //reles acionam luz  xxxxxxxxxxxxxx inicio xxxxxxxxxxxxxx
  pinMode(luzportao, OUTPUT);
  pinMode(luzgaragem, OUTPUT);
  pinMode(luzmezanino, OUTPUT);//intermediario
  pinMode(luzsalatv, OUTPUT);
  pinMode(luzvaranda, OUTPUT);//intermediario
  pinMode(luzvarandalateral, OUTPUT);
  pinMode(luzjardimfrente, OUTPUT);
  pinMode(luzjardimfrente2, OUTPUT);
  digitalWrite(luzportao, HIGH);
  digitalWrite(luzgaragem, HIGH);
  digitalWrite(luzmezanino, HIGH);
  digitalWrite(luzsalatv, HIGH);
  digitalWrite(luzvaranda, HIGH);
  digitalWrite(luzvarandalateral, HIGH);
  digitalWrite(luzjardimfrente, HIGH);
  digitalWrite(luzjardimfrente2, HIGH);
  digitalWrite(dicroicassala, HIGH);
  //reles acionam luz  xxxxxxxxxxxxxxx  fim   xxxxxxxxxxxxx dicroicassala

  //outros    xxxxxxxxxxxxxx inicio xxxxxxxxxxxxxx
  pinMode(buzina, OUTPUT);
  pinMode(relealarme, OUTPUT);
  pinMode(portao, OUTPUT);
  pinMode(emailinterfone, OUTPUT);
  pinMode(campainha, INPUT);
  pinMode(sensorLDR, INPUT);
  pinMode(sensorAlarme, INPUT);
  pinMode(sensorTemp, INPUT);
  digitalWrite(buzina, HIGH);
  digitalWrite(emailalarme, HIGH);
  digitalWrite(emailinterfone, HIGH);
  digitalWrite(relealarme, HIGH);
  digitalWrite(portao, HIGH);


  rtc.begin(); // Initialize the rtc object- relogio
  Ethernet.begin(mac, ip, DNS, gateway, subnet);
  server.begin();
  vw_set_tx_pin(rftrans);//rftransmissor
  vw_set_rx_pin(rfrecpt);
  vw_setup(5000);//rfalarme velocidade
  vw_rx_start();


  //outros    xxxxxxxxxxxxxxx  fim   xxxxxxxxxxxxx


  for (int i = 0; i <= tamlog; i++) logs[i] = "";


}



void loop()
{

  //aguarda acionamento alarme  em intervalo de 5 minutos            xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  if (digitalRead(emailalarme) != HIGH && millis() - tempo > intervaloalarme) {
    delay(3000);
    if (digitalRead(emailalarme) != HIGH) {
      envia();
      enviorf(0);
      digitalWrite(emailalarme, HIGH);
      tempo = millis();
    }
  }  else if (digitalRead(emailalarme) != HIGH) Serial.println((millis() - tempo) / 1000);
  if (millis() - tempo > intervaloalarme) tempo = -300000;



  if (analogRead(campainha) == 0) {
    digitalWrite(emailinterfone, LOW);
    enviorf(210);
  } else digitalWrite(emailinterfone, HIGH);


  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


  //timer luz jardim  luminosidade         xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  int inthora = rtc.getTime().hour;


  if ( (rtc.getTime().min) != minuto) { //envio do horario via rf e timer
    enviohora();
    minuto = (rtc.getTime().min);
  }

  if (inthora >= horanoite && luminosidade < LDRnoite && ctltimer > 0) {
    digitalWrite(luzjardimfrente, LOW);
    digitalWrite(luzjardimfrente2, LOW);
    soenviorf(200);
    ctltimer = 0;
  }

  if (inthora < horadia  && ctltimer == 0) {
    digitalWrite(luzjardimfrente, HIGH);
    digitalWrite(luzjardimfrente2, HIGH);
    ctltimer = 1;
  }

  if (inthora > portaonoite  || inthora < portaodia ) //horario de abrir o portao
    controleportao = 0; else controleportao = 1;


  if (analogRead(sensorLDR) < 1000)
    luminosidade = 1000 - analogRead(sensorLDR);
  else luminosidade = 0;
  //


  temperature = temp.read();







  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen))
  {
    for (int i = 0; i < buflen; i++)
    {
      //Armazena os caracteres recebidos
      recebido_RF_char[i] = char(buf[i]);
    }
    recebido_RF_char[buflen] = '\0';
    stringrecebidoRF = recebido_RF_char;
  }


  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



  EthernetClient client = server.available();
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n')
        {


          ligado_2 = controleluz("?2l", luzportao, ligado_2, 20, 21, "?2d");
          ligado_3 = controleluz("?3l", luzgaragem, ligado_3, 30, 31, "?3d");
          ligado_4 = controleluz("?4l", luzmezanino, ligado_4, 40, 41, "?4d");
          ligado_5 = controleluz("?5l", luzsalatv, ligado_5, 50, 51, "?5d");
          ligado_6 = controleluz("?6l", luzvaranda, ligado_6, 60, 61, "?6d");
          ligado_7 = controleluz("?7l", luzvarandalateral, ligado_7, 70, 71, "?7d");
          ligado_8 = controleluz("?8l", luzjardimfrente, ligado_8, 80, 81, "?8d");
          ligado_9 = controleluz("?9l", luzjardimfrente2, ligado_9, 90, 91, "?9d");
          ligado_10 = envioedi("?10l",  ligado_10, 100, 101, "?10d");
          ligado_11 = envioedi("?11l",  ligado_11, 110, 111, "?11d");
          ligado_12 = envioedi("?12l", ligado_12, 120, 121, "?12d");
          ligado_13 = envioedi("?13l", ligado_13, 130, 131, "?13d");
          ligado_14 = controleluz("?14l", relealarme, ligado_14, 140, 141, "?14d");
          ligado_15 = controleluz("?15l", buzina, ligado_15, 150, 151, "?15d");
          ligado_16 = envioedi("?16l", ligado_16, 160, 161, "?16d");
          ligado_17 = envioedi("?17l", ligado_17, 170, 171, "?17d");
          ligado_18 = envioedi("?18l", ligado_18, 180, 181, "?18d");
          ligado_19 = controlesenha("?19l", ligado_19, "?19d");





          //Controle do rele15
          Serial.println(readString);
          //Liga o Rele 15
          if (readString.indexOf("?1l") > 0 && controleportao > 0)
          {
            //minutoportao = inthora;
            digitalWrite(portao, LOW);
            delay(1000);
            digitalWrite(portao, HIGH);
            enviorf(10);
          }



          readString = "";

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>AR</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<meta name='viewport' content='width=360, initial-scale=0.65' />");
          client.println("<link rel='stylesheet' type='text/css' href='https://silviors.github.io/q6.css' />");
          client.println("<script type='text/javascript' src='https://silviors.github.io/q2.js'></script>");
          client.println("</head>");
          client.println("<body id=”pagina” display =’none’>");
          client.println("<div id='wrapper'>");

          client.println("<div id='dv0'>ARDUINO ANTONIO ROS. MAIA 136<br><br>HORA: ");
          client.println(rtc.getTimeStr());
          client.println("<br>TEMP ARD: ");
          client.println(rtc.getTemp());
          client.println("oC EXT: ");
          client.println(temperature);
          client.println("oC<br>LUZ (0-1000): ");
          client.println(luminosidade);
          client.println(" RRF:");
          client.println(stringrecebidoRF);
          client.println("<br>ALARME: ");
          if (analogRead(sensorAlarme) == 0) client.println("DESLIGADO"); else client.println("LIGADO");
          if (inthora > portaonoite  || inthora < portaodia )
            client.println("<br>ACIONA PORTAO: DESATIVADO"); else client.println("<br>ACIONA PORTAO: ATIVADO");
          client.println("</div>");


          client.println("<div id='dv1'>PORTAO</div>");
          client.println("<div id='dv2'>LUZ PORTAO</div>");
          client.println("<div id='dv3'>LUZ GARAGEM</div>");
          client.println("<div id='dv4'>LUZ HALL</div>");
          client.println("<div id='dv5'>LUZ SALA TV</div>");
          client.println("<div id='dv6'>LUZ VARANDA</div>");
          client.println("<div id='dv7'>LUZ ESCADA EXTERNA</div>");
          client.println("<div id='dv8'>LUZ JD FRENTE</div>");
          client.println("<div id='dv9'>REFLETORES FRENTE</div>");
          client.println("<div id='dv10'>LUZ EDICULA</div>");
          client.println("<div id='dv11'>LUZ EDICULA LATERAL</div>");
          client.println("<div id='dv12'>LUZ EDICULA JARDIM</div>");
          client.println("<div id='dv13'>LUZ FUNDOS JARDIM</div>");
          client.println("<div id='dv14'>ALARME</div>");
          client.println("<div id='dv15'>SIRENE</div>");
          client.println("<div id='dv16'>LUZ OFICINA</div>");
          client.println("<div id='dv17'>REFLETOR FUNDOS</div>");
          client.println("<div id='dv18'>LUZ PORAO</div>");
          client.println("<div id='dv19'>ENVIA HORA</div>");


          client.println("<div id='dv50'>HORARIO - ACAO - EVENTO<br><br>");
          for (int i = 0; logs[i]; i++) {
            client.println(logs[i]);
            client.println("<br>");
          }
          client.println("Ardinoprincipalv13</div>");

          client.println("<div id='bt1'></div>");
          client.println("<div id='bt2'></div>");
          client.println("<div id='bt3'></div>");
          client.println("<div id='bt4'></div>");
          client.println("<div id='bt5'></div>");
          client.println("<div id='bt6'></div>");
          client.println("<div id='bt7'></div>");
          client.println("<div id='bt8'></div>");
          client.println("<div id='bt9'></div>");
          client.println("<div id='bt10'></div>");
          client.println("<div id='bt11'></div>");
          client.println("<div id='bt12'></div>");
          client.println("<div id='bt13'></div>");
          client.println("<div id='bt14'></div>");
          client.println("<div id='bt15'></div>");
          client.println("<div id='bt16'></div>");
          client.println("<div id='bt17'></div>");
          client.println("<div id='bt18'></div>");
          client.println("<div id='bt19'></div>");

          client.print("<div id='rl1'></div><div id='et1'>");
          client.print(ligado_1);
          client.println("</div>");

          client.print("<div id='rl2'></div><div id='et2'>");
          client.print(ligado_2);
          client.println("</div>");

          client.print("<div id='rl3'></div><div id='et3'>");
          client.print(ligado_3);
          client.println("</div>");

          client.print("<div id='rl4'></div><div id='et4''>");
          client.print(ligado_4);
          client.println("</div>");

          client.print("<div id='rl5'></div><div id='et5'>");
          client.print(ligado_5);
          client.println("</div>");

          client.print("<div id='rl6'></div><div id='et6'>");
          client.print(ligado_6);
          client.println("</div>");

          client.print("<div id='rl7'></div><div id='et7'>");
          client.print(ligado_7);
          client.println("</div>");

          client.print("<div id='rl8'></div><div id='et8'>");
          client.print(ligado_8);
          client.println("</div>");

          client.print("<div id='rl9'></div><div id='et9'>");
          client.print(ligado_9);
          client.println("</div>");

          client.print("<div id='rl10'></div><div id='et10'>");
          client.print(ligado_10);
          client.println("</div>");

          client.print("<div id='rl11'></div><div id='et11'>");
          client.print(ligado_11);
          client.println("</div>");

          client.print("<div id='rl12'></div><div id='et12'>");
          client.print(ligado_12);
          client.println("</div>");

          client.print("<div id='rl13'></div><div id='et13'>");
          client.print(ligado_13);
          client.println("</div>");

          client.print("<div id='rl14'></div><div id='et14'>");
          client.print(ligado_14);
          client.println("</div>");

          client.print("<div id='rl15'></div><div id='et15'>");
          client.print(ligado_15);
          client.println("</div>");

          client.print("<div id='rl16'></div><div id='et16'>");
          client.print(ligado_16);
          client.println("</div>");

          client.print("<div id='rl17'></div><div id='et17'>");
          client.print(ligado_17);
          client.println("</div>");

          client.print("<div id='rl18'></div><div id='et18'>");
          client.print(ligado_18);
          client.println("</div>");

          client.print("<div id='rl19'></div><div id='et19'>");
          client.print(ligado_19);
          client.println("</div>");





          client.println("</div>");

          client.println("<script>AR1()</script>");
          client.println("<script>AR2()</script>");
          client.println("<script>AR3()</script>");
          client.println("<script>AR4()</script>");
          client.println("<script>AR5()</script>");
          client.println("<script>AR6()</script>");
          client.println("<script>AR7()</script>");
          client.println("<script>AR8()</script>");
          client.println("<script>AR9()</script>");
          client.println("<script>AR10()</script>");
          client.println("<script>AR11()</script>");
          client.println("<script>AR12()</script>");
          client.println("<script>AR13()</script>");
          client.println("<script>AR14()</script>");
          client.println("<script>AR15()</script>");
          client.println("<script>AR16()</script>");
          client.println("<script>AR17()</script>");
          client.println("<script>AR18()</script>");
          client.println("<script>AR19()</script>");
          //if (!ligado_19)client.println("<script>S1()</script>");


          client.println("</div>");

          client.println("</body>");
          client.println("</head>");
          client.println("</html>");

          delay(50);
          client.stop();
        }
      }
    }
  }






} // fim do programa





void envia()
{
  //lcd.setCursor(0, 1);
  delay(timer);
  Serial.println("conectando...");
  if (client.connect(serversmtp, 2525))  //Porta de conexao do servidor do email.
  {
    Serial.println("conectado!");
    Serial.println("enviando email...");
    Serial.println();
    client.println("EHLO localhost");
    recebe();
    delay(timer);
    client.println("AUTH LOGIN");
    recebe();
    delay(timer);
    client.println("YW50b25pb3Jvc2FyaW9tYWlhMTM2QGdtYWlsLmNvbQ==");  //Email de login em base de 64: http:base64-encoder-online.waraxe.us/
    recebe();
    delay(timer);
    client.println("c3JzanVuOTc="); // Senha do email em base de 64: http:base64-encoder-online.waraxe.us/
    recebe();
    delay(timer);
    client.println("mail from: <antoniorosariomaia136@gmail.com>"); //Email remetente
    recebe();
    delay(timer);
    client.println("rcpt to: <silviorjr@hotmail.com>");  //Email destinatário
    recebe();
    delay(timer);
    client.println("data");
    recebe();
    delay(timer);
    client.println("Subject: Alarme disparado!");  //Assunto
    recebe();
    delay(timer);
    client.println("O alarme da residencia disparou, email enviado do arduino, em 5 minutos sera novamente verificado, hora: ");  //Corpo
    recebe();
    delay(timer);
    client.println(rtc.getTimeStr());  //Corpo
    recebe();
    delay(timer);
    client.println(".");  //Indica fim do email.
    recebe();
    delay(timer);
    client.println();
    recebe();
    delay(timer);
    Serial.println("email enviado!");
    delay(timer);
    if (client.connected())  //Desconecta pra deixar pronto para novo envio.
    {
      Serial.println();
      Serial.println("desconectando...");
      client.stop();
      Serial.println();
      Serial.println();
    }
  }
  else
  {
    Serial.println("connection failed");
  }
  Serial.println("Ready. Press 'e' to connect.");
}


void recebe()
{
  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
}



void enviorf(int valorlido) {
  itoa(valorlido, Valor_CharMsg, 10);//Converte valor para envio
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }
  String loghora = rtc.getTimeStr();
  String onoff;
  String logacoes[] = {"DISPARO ALARME", "ACIONOU PORTAO ", "LUZ FRENTE PORT", "LUZ GARAGEM    ", "LUZ HALL ENTRAD", "LUZ SALA DE TV ",
                       "LUZ DA VARANDA", "LUZ ESCADA EXTE", "LUZ JARD FRENTE", "REFLETORES FREN", "LUZ EDICULA PRI", "LUZ EDICULA LAT",
                       "LUZ EDIC JARDI", "LUZ FUNDOS JARD", "CENTRAL ALARME ", "SIRENE ALARME  ", "LUZ OFICINA FER",
                       "LUZ REFLE MURO", "LUZ PORAO EDICU", "DICROICAS SALA ", "LUZES D JARDINS", "TOQUE CAMPAINHA"
                      };

  if (par(valorlido))onoff = "LIGA"; else onoff = "DESL";

  if (logacoes[valorlido / 10] + loghora.substring(0, 5) + onoff != logscontrole) {

    logs[codlog] = loghora + " - " + onoff + " - " + logacoes[valorlido / 10];
    logscontrole = logacoes[valorlido / 10] + loghora.substring(0, 5) + onoff;
    codlog = codlog + 1;
    if (codlog > tamlog - 1) {
      //envialog();
      codlog = 0;
    }
    //Serial.println(logscontrole);
    logs[codlog] = "--------------------   " + String(codlog) + "   --------------------";

  }
}

void soenviorf(int valorlido) {
  itoa(valorlido, Valor_CharMsg, 10);//Converte valor para envio
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }
}

void enviohora() {
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)rtc.getTimeStr(), strlen(rtc.getTimeStr()));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }
    enviasensores(luminosidade, 1);
    enviasensores(rtc.getTemp(), 3);
  
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
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)sensor_CharMsg_envio, strlen(sensor_CharMsg_envio));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }
}

void escreverlog() {
  String loghora = rtc.getTimeStr();
  String onoff;
  String logacoes[] = {"DISPARO ALARME", "ACIONOU PORTAO ", "LUZ FRENTE PORT", "LUZ GARAGEM    ", "LUZ HALL ENTRAD", "LUZ SALA DE TV ",
                       "LUZ DA VARANDA", "LUZ ESCADA EXTE", "LUZ JARD FRENTE", "REFLETORES FREN", "LUZ EDICULA PRI", "LUZ EDICULA LAT",
                       "LUZ EDIC JARDI", "LUZ FUNDOS JARD", "CENTRAL ALARME", "SIRENE ALARME", "LUZ OFICINA FER",
                       "LUZ REFLE MURO", "LUZ PORAO EDICU", "DICROICAS SALA", "LUZES JARDINS", "TOQUE CAMPAINHA"
                      };

  if (par(valorlido))onoff = "LIGA"; else onoff = "DESL";

  if (logacoes[valorlido / 10] + loghora.substring(0, 5) + onoff != logscontrole) {

    logs[codlog] = loghora + " - " + onoff + " - " + logacoes[valorlido / 10];
    logscontrole = logacoes[valorlido / 10] + loghora.substring(0, 5) + onoff;
    codlog = codlog + 1;
    if (codlog > tamlog) {
      //envialog();
      codlog = 0;
    }
    //Serial.println(logscontrole);
    logs[codlog] = "--------------------   " + String(codlog) + "   --------------------";

  }
}


boolean par (int num) {
  if (num % 2 == 0)
    return true;
  else return false;
}



//ligado_19 = controleluz("?19l", dicroicassala, ligado_19, 190, 191, "?19d");

boolean controleluz(String indligar, int nluz, boolean ctrligado, int codigorfl, int codigorfd, String inddesligar) {
  Serial.println(readString);
  if (readString.indexOf(indligar) > 0)
  {
    digitalWrite(nluz, LOW);
    ctrligado = false;
    enviorf(codigorfl);
  }
  else
  {
    if (readString.indexOf(inddesligar) > 0)
    {
      digitalWrite(nluz, HIGH);
      ctrligado = true;
      enviorf(codigorfd);
    }
  }
  return ctrligado;
}

boolean controlesenha(String indligar, boolean ctrligado, String inddesligar) {
  Serial.println(readString);
  if (readString.indexOf(indligar) > 0)
  {
    ctrligado = false;
  }
  else
  {
    if (readString.indexOf(inddesligar) > 0)
    {
      ctrligado = true;
    }
  }
  return ctrligado;
}

boolean envioedi(String indligar, boolean ctrligado, int codigorfl, int codigorfd, String inddesligar) {
  Serial.println(readString);
  if (readString.indexOf(indligar) > 0)
  {
    ctrligado = false;
    enviorf(codigorfl);
  }
  else
  {
    if (readString.indexOf(inddesligar) > 0)
    {
      ctrligado = true;
      enviorf(codigorfd);
    }
  }
  return ctrligado;
}




