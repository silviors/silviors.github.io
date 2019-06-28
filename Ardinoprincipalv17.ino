#include <Wire.h>
#include <VirtualWire.h>
#include <DS3231.h>      //Biblioteca para manipulação do DS3231
#include <SPI.h>
#include <Thermistor.h>
#include <UIPEthernet.h>
#include <UIPClient.h>
#define timer 1000
#define tamlog 100
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

//portas proximas a central xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int emailalarme = 14, emailinterfone = 15, campainha = 54, emailcerca = 34, buzina = 17, relealarme = 16, rftrans = 19, rfrecpt = 18, hora01 = 20, hora02 = 21;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas frente superior casa xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int luzportao = 22, luzgaragem = 23, luzmezanino = 24, luzsalatv = 25, portao = 26;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas fundos inferior xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int luzvarandalateral = 28, luzvaranda = 29, luzjardimfrente = 30, luzjardimfrente2 = 31, dicroicassala = 32;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas ENC28J60  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// ENC28J60-SO=50, ENC28J60-SI=51, ENC28J60-SCK=52, ENC28J60-CS=53;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// portas analogicas disponiveis        xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//int a0 = 54, a1 = 55, a2 = 56, a3 = 57, a4 = 58, a5 = 59, a6 = 60, a7 = 61, a8 = 62, a9 = 63, a10 = 64, a11 = 65, a12 = 66, a13 = 67, a14 = 68, a15 = 69;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// modulo hora e data                   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
DS3231  rtc(SDA, SCL);
Time dataehora;
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
int  controleportao = 1, ctltimer = 1, minuto = 0, minutoportao = 0, inthora,
     sensorLDR = A1, sensorAlarme = A9, sensorTemp = 3, inttime; //controle timer jardim e hora portao
unsigned int luminosidade = 1000, LDRnoite = 75, temperature;
int codlog = 0, controletemp = 0, controleLDR = 0;
int dlogs[tamlog], hlogs[tamlog], alogs[tamlog], itamlog = 0;


unsigned int tcod, valor_recebido_RF, acaoRF, portaRF, horaRF, minutoRF, diaRF, mesRF, anoRF, semanaRF;

int repetenvio = 20;
Thermistor temp(sensorTemp);

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


  minuto = (rtc.getTime().min);

  //outros    xxxxxxxxxxxxxxx  fim   xxxxxxxxxxxxx

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

  inthora = rtc.getTime().hour;
  dataehora = rtc.getTime();
  inttime = 10000 + inthora * 100 + minuto;


  if ( (dataehora.min) != minuto) { //envio do horario via rf e timer
    minuto = (dataehora.min);
    soenviorf(inttime);
    enviasensores(luminosidade, 1);
    enviasensores(temperature, 2);
    enviasensores(rtc.getTemp(), 3);
    enviohora();//provisorio
  }


  if (inthora >= horanoite && luminosidade < LDRnoite && ctltimer > 0) {
    digitalWrite(luzjardimfrente, LOW);
    digitalWrite(luzjardimfrente2, LOW);
    enviorf(200);
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


  temperature = temp.getTemperatura();






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
    tcod = stringrecebidoRF.length();


    int vetorportas[] = {0, 0, luzportao, luzgaragem, luzmezanino, luzsalatv, luzvaranda, luzvarandalateral,
                         luzjardimfrente, luzjardimfrente2, 0, 0, 0, 0, relealarme, buzina, 0, 0, 0, 0
                        };


    if (tcod == 3) {
      valor_recebido_RF = stringrecebidoRF.substring(0, 2).toInt();
      acaoRF = stringrecebidoRF.substring(2, 3).toInt();
      portaRF = vetorportas[valor_recebido_RF];
      if (portaRF > 0)
        if (acaoRF > 0)digitalWrite(portaRF, HIGH);
        else digitalWrite(portaRF, LOW);
      int horalog = inthora * 100 + minuto;
      int logRF = stringrecebidoRF.toInt();
      if (horalog != hlogs[itamlog - 1] ||  logRF != alogs[itamlog - 1] ) {
        hlogs[itamlog] = horalog;
        alogs[itamlog] = logRF;
        itamlog++;
      }
      if (itamlog > tamlog) itamlog = 0;

    }
    else   if (tcod == 8) { //22:05:55
      horaRF = stringrecebidoRF.substring(0, 2).toInt();
      minutoRF = stringrecebidoRF.substring(3, 5).toInt();
      rtc.setTime(horaRF, minutoRF, 0);
    }
    else   if (tcod == 7) { //4260619
      //String semanaset[] = {"0" , "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY" };
      semanaRF = stringrecebidoRF.substring(0, 1).toInt();
      diaRF = stringrecebidoRF.substring(1, 3).toInt();
      mesRF = stringrecebidoRF.substring(3, 5).toInt();
      anoRF = ("20" + stringrecebidoRF.substring(5, 7)).toInt();
      rtc.setDate(diaRF, mesRF, anoRF);
    }

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
          ligado_19 = envioedi("?19l", ligado_19, 190, 191, "?19d");





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

          client.println("<div id='dv0'>ARDUINO ANTONIO ROS. MAIA 136<br><br>DATA: ");

          client.print(convertestr(dataehora.date));
          client.print("/");
          client.print(convertestr(dataehora.mon));
          client.print("/");
          client.print(dataehora.year);
          client.println("   -   ");
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


          idcabecalho(client, 1, "PORTAO");
          idcabecalho(client, 2, "LUZ PORTAO");
          idcabecalho(client, 3, "LUZ GARAGEM");
          idcabecalho(client, 4, "LUZ HALL");
          idcabecalho(client, 5, "LUZ SALA TV");
          idcabecalho(client, 6, "LUZ VARANDA");
          idcabecalho(client, 7, "LUZ ESCADA EXTERNA");
          idcabecalho(client, 8, "LUZ JD FRENTE");
          idcabecalho(client, 9, "REFLETORES FRENTE");
          idcabecalho(client, 10, "LUZ EDICULA");
          idcabecalho(client, 11, "LUZ EDICULA LATERAL");
          idcabecalho(client, 12, "LUZ EDICULA JARDIM");
          idcabecalho(client, 13, "LUZ FUNDOS JARDIM");
          idcabecalho(client, 14, "ALARME");
          idcabecalho(client, 15, "SIRENE");
          idcabecalho(client, 16, "LUZ OFICINA");
          idcabecalho(client, 17, "REFLETOR FUNDOS");
          idcabecalho(client, 18, "LUZ PORAO");
          idcabecalho(client, 19, "ENVIAR HORA");


          client.println("<div id='dv50'>DATA - HORARIO - ACAO - EVENTO<br><br>");
          for (int i = 0; i < tamlog; i++) {
            client.println(escreverlog(dlogs[i], hlogs[i], alogs[i]));
          }
          client.println( "--------------------   " + String(itamlog) + "   --------------------");



          client.println("Ardinoprincipalv17</div>");




          idbotao(client, 1);
          idbotao(client, 2);
          idbotao(client, 3);
          idbotao(client, 4);
          idbotao(client, 5);
          idbotao(client, 6);
          idbotao(client, 7);
          idbotao(client, 8);
          idbotao(client, 9);
          idbotao(client, 10);
          idbotao(client, 11);
          idbotao(client, 12);
          idbotao(client, 13);
          idbotao(client, 14);
          idbotao(client, 15);
          idbotao(client, 16);
          idbotao(client, 17);
          idbotao(client, 18);
          idbotao(client, 19);



          client.print("<div id='rl1'></div><div id='et1'>"); client.print(ligado_1); client.println("</div>");
          client.print("<div id='rl2'></div><div id='et2'>"); client.print(ligado_2); client.println("</div>");
          client.print("<div id='rl3'></div><div id='et3'>"); client.print(ligado_3); client.println("</div>");
          client.print("<div id='rl4'></div><div id='et4'>"); client.print(ligado_4); client.println("</div>");
          client.print("<div id='rl5'></div><div id='et5'>"); client.print(ligado_5); client.println("</div>");
          client.print("<div id='rl6'></div><div id='et6'>"); client.print(ligado_6); client.println("</div>");
          client.print("<div id='rl7'></div><div id='et7'>"); client.print(ligado_7); client.println("</div>");
          client.print("<div id='rl8'></div><div id='et8'>"); client.print(ligado_8); client.println("</div>");
          client.print("<div id='rl9'></div><div id='et9'>"); client.print(ligado_9); client.println("</div>");
          client.print("<div id='rl10'></div><div id='et10'>"); client.print(ligado_10); client.println("</div>");
          client.print("<div id='rl11'></div><div id='et11'>"); client.print(ligado_11); client.println("</div>");
          client.print("<div id='rl12'></div><div id='et12'>"); client.print(ligado_12); client.println("</div>");
          client.print("<div id='rl13'></div><div id='et13'>"); client.print(ligado_13); client.println("</div>");
          client.print("<div id='rl14'></div><div id='et14'>"); client.print(ligado_14); client.println("</div>");
          client.print("<div id='rl15'></div><div id='et15'>"); client.print(ligado_15); client.println("</div>");
          client.print("<div id='rl16'></div><div id='et16'>"); client.print(ligado_16); client.println("</div>");
          client.print("<div id='rl17'></div><div id='et17'>"); client.print(ligado_17); client.println("</div>");
          client.print("<div id='rl18'></div><div id='et18'>"); client.print(ligado_18); client.println("</div>");
          client.print("<div id='rl19'></div><div id='et19'>"); client.print(ligado_19); client.println("</div>");


          client.println("</div>");

          idscript(client, 1);
          idscript(client, 2);
          idscript(client, 3);
          idscript(client, 4);
          idscript(client, 5);
          idscript(client, 6);
          idscript(client, 7);
          idscript(client, 8);
          idscript(client, 9);
          idscript(client, 10);
          idscript(client, 11);
          idscript(client, 12);
          idscript(client, 13);
          idscript(client, 14);
          idscript(client, 15);
          idscript(client, 16);
          idscript(client, 17);
          idscript(client, 18);
          idscript(client, 19);
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





void idcabecalho(EthernetClient cliente, int numero, String Titulo) {
  cliente.println("<div id='dv" + String(numero) + "'>" + Titulo + "</div>");
}
void idbotao(EthernetClient cliente, int numero) {
  cliente.println("<div id='bt" + String(numero) + "'></div>");
}
void idscript(EthernetClient cliente, int numero) {
  cliente.println("<script>AR" + String(numero) + "()</script>");
}









void enviorf(int valorlido) {
  itoa(valorlido, Valor_CharMsg, 10);//Converte valor para envio
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }

  int horalog = inthora * 100 + minuto;
  int datalog = dataehora.date * 100 + dataehora.mon;
  if (horalog != hlogs[itamlog - 1] || valorlido != alogs[itamlog - 1] ) {
    dlogs[itamlog] = datalog;
    hlogs[itamlog] = horalog;
    alogs[itamlog] = valorlido;
    itamlog++;
  }
  if (itamlog > tamlog) itamlog = 0;
}

void soenviorf(int valorlido) {
  itoa(valorlido, Valor_CharMsg, 10);//Converte valor para envio
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
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
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)sensor_CharMsg_envio, strlen(sensor_CharMsg_envio));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }
}

String escreverlog(int data, int horario, int acao) {
  String logacoes[] = {"DISPARO ALARME", "ACIONOU PORTAO ", "LUZ FRENTE PORT", "LUZ GARAGEM    ", "LUZ HALL ENTRAD", "LUZ SALA DE TV ",
                       "LUZ DA VARANDA", "LUZ ESCADA EXTE", "LUZ JARD FRENTE", "REFLETORES FREN", "LUZ EDICULA PRI", "LUZ EDICULA LAT",
                       "LUZ EDIC JARDI", "LUZ FUNDOS JARD", "CENTRAL ALARME", "SIRENE ALARME", "LUZ OFICINA FER",
                       "LUZ REFLE MURO", "LUZ PORAO EDICU", "DICROICAS SALA", "LUZES JARDINS", "TOQUE CAMPAINHA"
                      };
  String onoff, linhalog = "";
  int hora, minuto, dia, mes;

  if (par(acao))onoff = "LIGA"; else onoff = "DESL";

  dia = data / 100;
  mes = data % 100;
  hora = horario / 100;
  minuto = horario % 100;

  if (horario > 0)
    linhalog = convertestr(dia) + "/" + convertestr(mes) + " - " + convertestr(hora) + ":" + convertestr(minuto) + " - " + onoff  + " - " + logacoes[acao / 10] + "<br>";

  return linhalog;

}


boolean par (int num) {
  if (num % 2 == 0)
    return true;
  else return false;
}

String convertestr(int prefixoenvio) {
  String codigoenvio;
  if (prefixoenvio < 10)
    codigoenvio = "0" + String(prefixoenvio);
  else     codigoenvio = String(prefixoenvio);
  return codigoenvio;
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





//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio//provisorio
void enviohora() {
  for (int i = 0; i < repetenvio; i++) {
    vw_send((uint8_t *)rtc.getTimeStr(), strlen(rtc.getTimeStr()));    //Envio dos dados
    vw_wait_tx();//Aguarda envio dos dados
  }

}
