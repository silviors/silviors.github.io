#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "time.h"
#include "Wire.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#define slaveAdress 0x08
#define timer 1000
#define tamlog 50
#define horanoite 17
#define horadia 1
#define LDRnoite 75
#define portaonoite 22 // abre ate as 22:59
#define portaodia 6 //abre apos as 6:00
#define senhaportaodireito "2005"
#define senhaportaoesquerdo "2006"
#define senhaportaopedestre "2007"





//portas frente superior casa xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int luzportao = 15, luzgaragem = 4, luzmezanino = 16, luzsalatv = 1;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas fundos inferior xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int luzvaranda = 5, luzvarandalateral = 18, luzjardimfrente = 3, luzjardimfrente2 = 19, dicroicassala = 99;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//portas sensores e reles xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int campainha = 39, sensorAlarme = 35, emailinterfone = 13, buzina = 12, relealarme = 14, portao = 27, portao2 = 26, portaopedestre = 17, emailalarme = 23;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


int  controleportao = 1, ctltimer = 1, minuto = 0, minutoportao = 0, inthora, horaagora,
     sensorLDR = 32, sensorTemp = 25, inttime, intwire; //controle timer jardim e hora portao

unsigned int luminosidade = 1000;




/*Put your SSID & Password*/
const char* ssid = "silviowless";  // Enter SSID here
const char* password = "@silvinho20061";  //Enter Password here


const char* ntpServer = "0.pool.ntp.org";
const long  gmtOffset_sec = 3600 * -3;
const int   daylightOffset_sec = 3600;

long  tempoalarme = -300000;;
unsigned long tempo, ctportao = 0, intervaloalarme = 300000; //intervalo de envio de email
int intervalotime = 60000, horaint, tamndado;
struct tm * horadata;
time_t horatxt;
String datatxt, calendario, conexao, luz,  tinterna, vetortempdiario;
String readString;
String received = "NADA";
float texterna;
boolean diaLog = false, irparahome = false;


int tmwread = 0;
unsigned int dlogs[tamlog], hlogs[tamlog], alogs[tamlog], itamlog = 0, luzlogs[24];
float templogs[24];


OneWire oneWire(sensorTemp);
DallasTemperature tempSensor(&oneWire);




WiFiServer server(80);
// Variable to store the HTTP request
String header;
byte serversmtp[] = { 43, 228, 184, 6 }; // Colocar o ip do servidor SMTP
WiFiClient client;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

long temperatura;
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



#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();




void setup() {
  Serial.begin(115200);
  analogReadResolution(10);


  /////////////////////////////////////////////////////////////////////////////////
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, HIGH);
    delay(4000);
    digitalWrite(2, LOW);
    delay(1000);
  }
  conexao = "AO LIGAR";


  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
  })
  .onEnd([]() {
  })
  .onProgress([](unsigned int progress, unsigned int total) {
  })
  .onError([](ota_error_t error) {
  });
  ArduinoOTA.begin();
  /////////////////////////////////////////////////////////////////////////////////




  pinMode(luzportao, OUTPUT);
  digitalWrite(luzportao, HIGH);
  pinMode(luzgaragem, OUTPUT);
  digitalWrite(luzgaragem, HIGH);
  pinMode(luzmezanino, OUTPUT);
  digitalWrite(luzmezanino, HIGH);
  pinMode(luzsalatv, OUTPUT);
  digitalWrite(luzsalatv, HIGH);
  pinMode(luzvaranda, OUTPUT);
  digitalWrite(luzvaranda, HIGH);
  pinMode(luzvarandalateral, OUTPUT);
  digitalWrite(luzvarandalateral, HIGH);
  pinMode(luzjardimfrente, OUTPUT);
  digitalWrite(luzjardimfrente, LOW);
  pinMode(luzjardimfrente2, OUTPUT);
  digitalWrite(luzjardimfrente2, LOW);

  pinMode(emailinterfone, OUTPUT);
  digitalWrite(emailinterfone, HIGH);
  pinMode(buzina, OUTPUT);
  digitalWrite(buzina, HIGH);
  pinMode(relealarme, OUTPUT);
  digitalWrite(relealarme, HIGH);
  pinMode(portao, OUTPUT);
  digitalWrite(portao, HIGH);
  pinMode(portao2, OUTPUT);
  digitalWrite(portao2, HIGH);
  pinMode(portaopedestre, OUTPUT);
  digitalWrite(portaopedestre, HIGH);


  pinMode(campainha, INPUT);
  pinMode(sensorAlarme, INPUT);
  pinMode(sensorLDR, INPUT);
  pinMode(emailalarme, OUTPUT);
  digitalWrite(emailalarme, HIGH);








  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);



  server.begin();
  delay(2000);



  tempo = -1 * intervalotime;

  Wire.begin(); // ingressa ao barramento I2C


  tempSensor.begin();


}






void loop() {
  ArduinoOTA.handle();
  String hora, minuto, segundo, dia, mes;
  String semana[] = {"DOMINGO", "SEGUNDA-FEIRA", "TERCA-FEIRA", "QUARTA-FEIRA", "QUINTA-FEIRA", "SEXTA-FEIRA", "SABADO"};
  int calendarioint;


  if (analogRead(campainha) == 0) {
    digitalWrite(emailinterfone, LOW);
    enviacodigo("210"); //fazer rotina para nao enviar direto
  } else digitalWrite(emailinterfone, HIGH );


  luminosidade = analogRead(sensorLDR);
  if (luminosidade > 999)luminosidade = 0; else luminosidade = 1000 - luminosidade;

  //if (analogRead(sensorAlarme) > 0) enviacodigo("500000"); else enviacodigo("500001");


  wifiConnect();

  if (millis() - tempo > intervalotime) {

    time(&horatxt);
    horadata = localtime(&horatxt);
    if (horadata->tm_hour < 10) hora = "0" + String(horadata->tm_hour); else hora = String(horadata->tm_hour);
    if (horadata->tm_min < 10) minuto = "0" + String(horadata->tm_min); else minuto = String(horadata->tm_min);
    if (horadata->tm_sec < 10) segundo = "0" + String(horadata->tm_sec); else segundo = String(horadata->tm_sec);
    if (horadata->tm_mday + 1 < 10) dia = "0" + String(horadata->tm_mday); else dia = String(horadata->tm_mday);
    if (horadata->tm_mon + 1 < 10) mes = "0" + String(horadata->tm_mon + 1); else mes = String(horadata->tm_mon + 1);
    datatxt =  hora + ":" + minuto + ":" + segundo;
    horaint = 10000 + hora.toInt() * 100 + minuto.toInt();
    calendario = semana[horadata->tm_wday] + ", " + dia + "/" + mes + "/" + String(horadata->tm_year + 1900);
    calendarioint = (horadata->tm_wday + 1) * 1000000 + horadata->tm_mday * 10000 + (horadata->tm_mon + 1) * 100 + (horadata->tm_year - 100);
    horaagora = hora.toInt();

    if (conexao == "AO LIGAR")conexao = dia + "/" + mes + " - " + datatxt;

    temperatura = (temprature_sens_read() - 32) / 1.8;

    tempSensor.requestTemperatures();
    texterna = tempSensor.getTempCByIndex(0);

    if (minuto == "00") {
      if (hora == "00") {
        diaLog = true;
        envia();
      }
      templogs[hora.toInt()] = texterna;
      luzlogs[hora.toInt()] = luminosidade;
    }



    //enviacodigo(datatxt);
    enviacodigo(String(calendarioint) + datatxt);
    //enviacodigo(String(1000 + luminosidade));
    //enviacodigo(String(3000 + temperatura));
    tempo = millis();
  }


  // realiza uma requisicao de 1 byte de dados do modulo slave
  //Wire.requestFrom(slaveAdress, 4);
  Wire.requestFrom(slaveAdress, 50);
  int ndado = 0; //ponemos a 0 la variable de conteo
  while (Wire.available() > 0)   {
    int dado = Wire.read(); // Recibe la palabra del i2c tx
    if (dado)   {
      received[ndado] = dado;//guarda el caracter recibido en una poscicion del vector
      ndado++;//incrementamos el contador
      tamndado = ndado;
    }
    else {

      break;//al finalizar la transmicion rompemos el while
    }
  }

  if (received == senhaportaodireito) {
    ligadesl(buzina, 100);
    ligadesl(portao, 1000);
    enviacodigo("220");
    received = "0000";
  } else if (received == senhaportaoesquerdo) {
    ligadesl(buzina, 100);
    ligadesl(portao2, 1000);
    received = "0000";
    enviacodigo("230");
  } else if (received == senhaportaopedestre) {
    ligadesl(buzina, 100);
    ligadesl(portaopedestre, 2000);
    delay(1000);
    received = "0000";
    enviacodigo("240");
  }

  intwire = received.toInt();
  //else if (received.substring(0, 1) == "0")
  // received = received.substring(1);



  //  if (received.substring(0, 1) == "1")
  //    luz = received.substring(1);
  //  else if (received.substring(0, 1) == "2")
  //    texterna = received.substring(2);
  //  else if (received.substring(0, 1) == "3")
  //    tinterna = received.substring(2);


  //aguarda acionamento alarme  em intervalo de 5 minutos            xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  if (digitalRead(emailalarme) != HIGH && millis() - tempoalarme > intervaloalarme) {
    delay(3000);
    if (digitalRead(emailalarme) != HIGH) {
      envia();
      enviacodigo("0");
      digitalWrite(emailalarme, HIGH);
      tempoalarme = millis();
    }
  }  else if (digitalRead(emailalarme) != HIGH) Serial.println((millis() - tempoalarme) / 1000);
  if (millis() - tempoalarme > intervaloalarme) tempoalarme = -300000;




  if (horaagora > portaonoite  || horaagora < portaodia ) //horario de abrir o portao
    controleportao = 0; else controleportao = 1;

  if (luminosidade < LDRnoite)
    if ((horaagora >= horanoite) && (ctltimer > 0)) {
      digitalWrite(luzjardimfrente, HIGH);
      digitalWrite(luzjardimfrente2, HIGH);
      enviacodigo("200");
      ctltimer = 0;
    } else if (horaagora < horadia) {
      digitalWrite(luzjardimfrente, LOW);
      digitalWrite(luzjardimfrente2, LOW);
      ctltimer = 1;
    }


  client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n') {                    // if the byte is a newline character



          ligado_2 = controleluz("?2l", luzportao, ligado_2, "020", "021", "?2d", true);
          ligado_3 = controleluz("?3l", luzgaragem, ligado_3, "030", "031", "?3d", true);
          ligado_4 = controleluz("?4l", luzmezanino, ligado_4, "040", "041", "?4d", true);
          ligado_5 = controleluz("?5l", luzsalatv, ligado_5, "050", "051", "?5d", true);
          ligado_6 = controleluz("?6l", luzvaranda, ligado_6, "060", "061", "?6d", true);
          ligado_7 = controleluz("?7l", luzvarandalateral, ligado_7, "070", "071", "?7d", true);
          ligado_8 = controleluz("?8l", luzjardimfrente, ligado_8, "080", "081", "?8d", false);
          ligado_9 = controleluz("?9l", luzjardimfrente2, ligado_9, "090", "091", "?9d", false);
          ligado_10 = controleluz("?10l", luzportao, ligado_10, "100", "101", "?10d", true);
          ligado_11 = controleluz("?11l", luzportao, ligado_11, "110", "111", "?11d", true);
          ligado_12 = controleluz("?12l", luzportao, ligado_12, "120", "121", "?12d", true);
          ligado_13 = controleluz("?13l", luzportao, ligado_13, "130", "131", "?13d", true);
          ligado_14 = controleluz("?14l", relealarme, ligado_14, "140", "141", "?14d", true);
          ligado_15 = controleluz("?15l", buzina, ligado_15, "150", "151", "?15d", true);
          ligado_16 = controleluz("?16l", luzportao, ligado_16, "160", "161", "?16d", true);
          ligado_17 = controleluz("?17l", luzportao, ligado_17, "170", "171", "?17d", true);
          ligado_18 = controleluz("?18l", luzportao, ligado_18, "180", "181", "?18d", true);


          Serial.println(readString);


          //Controle do rele19
          //Liga o Rele 19
          if (readString.indexOf("?1l9") > 0)
          {
            enviacodigo(String(calendarioint) + datatxt);
            irparahome = true;
          }


          //Controle do rele1
          if (readString.indexOf("?1d") > 0 && controleportao > 0 && millis() - ctportao > 5000)
          {
            ligadesl(buzina, 100);
            ligadesl(portao, 1000);
            ctportao = millis();
            enviacodigo("220");
            irparahome = true;
          }
          if (readString.indexOf("?60d") > 0 && controleportao > 0 && millis() - ctportao > 5000)
          {
            ligadesl(buzina, 100);
            ligadesl(portao2, 1000);
            ctportao = millis();
            enviacodigo("230");
            irparahome = true;
          }




          readString = "";

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>ESP32 TORRE CX D'AGUA</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<meta name='viewport' content='width=360, initial-scale=0.65' />");
          client.println("<link rel='stylesheet' type='text/css' href='https://silviors.github.io/q6.css' />");
          client.println("<script type='text/javascript' src='https://silviors.github.io/q2.js'></script>");
          client.println("</head>");
          client.println("<body id=”pagina” display =’none’>");
          client.println("<div id='wrapper'>");

          client.println("<div id='dv0'>ESP32 ANTONIO ROS. MAIA 136<br>");
          client.println(calendario);
          client.println("<br>HORA: ");
          client.println(datatxt);
          client.println("<br>TEMP INT: ");
          client.println(temperatura);
          client.println("oC EXT: ");
          client.println(texterna);
          client.println("oC<br>LUZ (0-1000): ");
          client.println(luminosidade);
          client.print("WIRE: ");
          client.println(received);
          client.println("<br>HORA: ");
          client.println(horaint);
          client.println(" DATA: ");
          client.println(calendarioint);
          client.println("<br>ALARME: ");
          if (analogRead(sensorAlarme) == 0) client.println("DESLIGADO"); else client.println("LIGADO");
          client.println("</div>");

          if (horaagora > portaonoite  || horaagora < portaodia )
            idcabecalho(client, 1, "PORTAO<br><br><br><br><small>DESATIVADO</small>"); else idcabecalho(client, 1, "PORTAO<br><br><br><br><small>ATIVADO</small>");

          idcabecalho(client, 2, "LUZ PORTAO");
          idcabecalho(client, 3, "LUZ GARAGEM");
          idcabecalho(client, 4, "LUZ HALL");
          idcabecalho(client, 5, "LUZ SALA TV");
          idcabecalho(client, 6, "LUZ VARANDA");
          idcabecalho(client, 7, "LUZ ESCADA EXTERNA");
          if (ctltimer == 0) idcabecalho(client, 8, "LUZ JD FRENTE<br><br><br><br><small>LIGADO LDR</small>"); else idcabecalho(client, 8, "LUZ JD FRENTE");
          if (ctltimer == 0) idcabecalho(client, 9, "REFLETORES FRENTE<br><br><br><br><small>LIGADO LDR</small>"); else idcabecalho(client, 9, "REFLETORES FRENTE");
          idcabecalho(client, 10, "LUZ EDICULA");
          idcabecalho(client, 11, "LUZ EDICULA LATERAL");
          idcabecalho(client, 12, "LUZ EDICULA JARDIM");
          idcabecalho(client, 13, "LUZ FUNDOS JARDIM");

          if (analogRead(sensorAlarme) == 0)
            idcabecalho(client, 14, "ALARME<br><br><br><br><small>DESLIGADO</small>"); else idcabecalho(client, 14, "ALARME<br><br><br><br><small>LIGADO</small>");

          idcabecalho(client, 15, "SIRENE");
          idcabecalho(client, 16, "LUZ OFICINA");
          idcabecalho(client, 17, "REFLETOR FUNDOS");
          idcabecalho(client, 18, "LUZ PORAO");
          idcabecalho(client, 19, "ENVIAR HORA");



          client.println("<div id='dv50'><p class='red'>INFORMACOES</p>");
          client.println("Esp32-MaquinaOTAI2cV10<br>");
          client.println("WiFi connected! ");
          client.println(conexao);
          client.print("<br>IP address: ");
          client.println(WiFi.localIP());
          client.print("<br>ESP Mac Address: ");
          client.println(WiFi.macAddress());
          client.print("<br>Subnet Mask: ");
          client.println(WiFi.subnetMask());
          client.print("<br>Gateway IP: ");
          client.println(WiFi.gatewayIP());
          client.print("<br>DNS: ");
          client.println(WiFi.dnsIP());
          client.print("<br>LUZ: ");
          client.println(analogRead(sensorLDR));
          client.print("<br>TEMP EXTERNA: ");
          client.println(texterna);
          client.print("*C<br>TEMP INTERNA: ");
          client.println(tinterna);
          client.print("*C<br>TEMP CHIP: ");
          client.println(temperatura);
          client.println("*C");
          client.print("<br>INTWIRE: ");
          client.println(intwire);
          client.print("<br>SENSOR ALARME:");
          client.println(analogRead(sensorAlarme));
          client.print("<br>SENSOR CAMPAINHA:");
          client.println(analogRead(campainha));
          client.print("<br>DISPARO ALARME: ");
          if (digitalRead(emailalarme) > 0)
            client.println("NENHUM");
          else client.println("DISPARADO");
          client.print("<br><br><p class='red'>HORA-TEMPERATURA/LUZ:</p>");
          for (int i = 0; i < 24; i++) {
            if (templogs[i] > 0) {
              if (i < 10) client.print("0");
              client.println(String(i) + ":00 - TEMP: " +  String(templogs[i]) + " oC / LUZ: " + String(luzlogs[i]) + "<br>");
            }

          }




          client.println("<p class='red'>DATA - HORARIO - ACAO - EVENTO</p>");
          for (int i = 0; i < tamlog; i++) {
            String txtlog = escreverlog(dlogs[i], hlogs[i], alogs[i]);
            client.println(txtlog);
            if (txtlog != "")client.println("<br>");
          }
          client.println( "-----------------------   " + String(itamlog) + "   -----------------------");


          client.println("</div>");

          idbotao(client, 1);
          idbotao(client, 60);
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

          if (irparahome) {
            client.println("<script> window.location.href = 'http://silviors.dlinkddns.com:881' </script>");
            irparahome = false;
          }

          client.println("</body>");
          client.println("</head>");
          client.println("</html>");

          delay(10);
          client.stop();
        }
      }
    }
  }





}



void idcabecalho(WiFiClient cliente, int numero, String Titulo) {
  cliente.println("<div id='dv" + String(numero) + "'>" + Titulo + "</div>");
}
void idbotao(WiFiClient cliente, int numero) {
  cliente.println("<div id='bt" + String(numero) + "'></div>");
}
void idscript(WiFiClient cliente, int numero) {
  cliente.println("<script>AR" + String(numero) + "()</script>");
}





boolean controleluz(String indligar, int nluz, boolean ctrligado, String codigorfl, String codigorfd, String inddesligar, boolean sentido) {
  if (readString.indexOf(indligar) > 0)
  {
    if (sentido)
      digitalWrite(nluz, LOW);
    else digitalWrite(nluz, HIGH);
    ctrligado = false;
    int len = codigorfl.length() + 1;
    char arraychar[len];
    codigorfl.toCharArray(arraychar, len);
    Wire.beginTransmission(slaveAdress);
    for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
    Wire.endTransmission();
    irparahome = true;
    salvarlog(codigorfl);
  }
  else
  {
    if (readString.indexOf(inddesligar) > 0)
    {
      if (sentido)
        digitalWrite(nluz, HIGH );
      else digitalWrite(nluz, LOW);
      ctrligado = true;
      int len = codigorfd.length() + 1;
      char arraychar[len];
      codigorfd.toCharArray(arraychar, len);
      Wire.beginTransmission(slaveAdress);
      for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
      Wire.endTransmission();
      irparahome = true;
      salvarlog(codigorfd);
    }
  }






  return ctrligado;
}

boolean enviahora(String indligar, int nluz, boolean ctrligado, String inddesligar) {
  if (readString.indexOf(indligar) > 0)
  {
    digitalWrite(nluz, LOW);
    ctrligado = false;
    int len = datatxt.length() + 1;
    char arraychar[len];
    datatxt.toCharArray(arraychar, len);
    Wire.beginTransmission(slaveAdress);
    for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
    Wire.endTransmission();
  }
  else
  {
    if (readString.indexOf(inddesligar) > 0)
    {
      digitalWrite(nluz, HIGH );
      ctrligado = true;
      int len = datatxt.length() + 1;
      char arraychar[len];
      datatxt.toCharArray(arraychar, len);
      Wire.beginTransmission(slaveAdress);
      for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
      Wire.endTransmission();
    }
  }


  return ctrligado;
}

void enviacodigo(String txtenvio) {

  int len = txtenvio.length() + 1;
  char arraychar[len];
  txtenvio.toCharArray(arraychar, len);
  Wire.beginTransmission(slaveAdress);
  for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
  Wire.endTransmission();

  if (len < 5)
    salvarlog(txtenvio);



}

void wifiConnect()
{
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect(true);
    delay(1000);
    WiFi.begin(ssid, password);
    conexao = datatxt + "-" + calendario;
    return;
  }
  // other stuff
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


    if (diaLog)
    {
      client.println("Subject: Registros do dia " + calendario);  //Assunto
      recebe();
      delay(timer);
      client.println("HORA-TEMPERATURA/LUZ:");
      for (int i = 0; i < 24; i++) {
        if (templogs[i] > 0) {
          if (i < 10) client.print("0");
          client.println(String(i) + ":00 - TEMP: " +  String(templogs[i]) + " oC / LUZ: " + String(luzlogs[i]));
          recebe();
          delay(timer / 300);
        }
      }
      client.println("DATA - HORARIO - ACAO - EVENTO");
      for (int i = 0; i < tamlog; i++) {
        client.println(escreverlog(dlogs[i], hlogs[i], alogs[i]));
        recebe();
        delay(timer / 300);
      }
      diaLog = false;
    }

    else if (digitalRead(emailalarme) != LOW)
    {
      client.println("Subject: Registros do log arduino");  //Assunto
      recebe();
      delay(timer);
      client.println("DATA - HORARIO - ACAO - EVENTO");
      for (int i = 0; i < tamlog; i++) {
        client.println(escreverlog(dlogs[i], hlogs[i], alogs[i]));
        recebe();
        delay(timer / 300);
      }
    }

    else
    {
      client.println("Subject: Alarme disparado! - " + calendario + " - " + datatxt);  //Assunto
      recebe();
      delay(timer);
      client.println("O alarme da residencia disparou, email enviado do arduino, em 5 minutos sera novamente verificado, ");  //Corpo
      client.println(calendario);
      client.println(" HORA: ");
      client.println(datatxt);
      recebe();
      delay(timer);
      //      client.println(rtc.getTimeStr());  //Corpo
      recebe();
      delay(timer);
    }


    client.println(".");  //Indica fim do email.
    recebe();
    delay(timer);
    client.println();
    recebe();
    delay(timer);
    delay(timer);
    if (client.connected())  //Desconecta pra deixar pronto para novo envio.
    {
      client.stop();
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


String escreverlog(int data, int horario, int acao) {
  String logacoes[] = {"DISPARO ALARM", "ACIONOU PORTA", "LUZ FREN PORT", "LUZ GARAGEM",   "LUZ HALL ENTR", "LUZ SALA D TV",
                       "LUZ DA VARAND", "LUZ ESCAD EXT", "LUZ JARD FREN", "REFLETOR FREN", "LUZ EDICU PRI", "LUZ EDICU LAT",
                       "LUZ EDIC JARD", "LUZ FUNDO JAR", "CENTRAL ALARM", "SIRENE ALARME", "LUZ OFICI FER",
                       "LUZ REFLE MUR", "LUZ PORAO EDI", "DICROICAS SAL", "LUZES JARDINS", "TOQUE CAMPAIN", "PORTAO DIREIT",
                       "PORTAO ESQUER", "PORTAO PEDEST"
                      };
  String onoff, linhalog = "";
  int hora, minuto, dia, mes;

  if (par(acao))onoff = "LIGA"; else onoff = "DESL";

  dia = data / 100;
  mes = data % 100;
  hora = horario / 100;
  minuto = horario % 100;

  if (horario > 0)
    linhalog = convertestr(dia) + "/" + convertestr(mes) + " - " + convertestr(hora) + ":" + convertestr(minuto) + " - " + onoff  + " - " + logacoes[acao / 10];

  return linhalog;

}


boolean par (int num) {
  if (num % 2 == 0)
    return true;
  else return false;
}


void salvarlog(String txtsalvo) {
  int horalog = horaint - 10000;
  int datalog = horadata->tm_mday * 100 + (horadata->tm_mon + 1);
  int valorlido = txtsalvo.toInt();
  if (itamlog == tamlog) {
    envia();
    itamlog = 0;
  } else if (horalog != hlogs[itamlog - 1] || valorlido != alogs[itamlog - 1] ) {
    dlogs[itamlog] = datalog;
    hlogs[itamlog] = horalog;
    alogs[itamlog] = valorlido;
    itamlog++;
  }
}

void ligadesl(int rele, int delayrele) {
  digitalWrite(rele, LOW );
  delay(delayrele);
  digitalWrite(rele, HIGH);
}



String convertestr(int prefixoenvio) {
  String codigoenvio;
  if (prefixoenvio < 10)
    codigoenvio = "0" + String(prefixoenvio);
  else     codigoenvio = String(prefixoenvio);
  return codigoenvio;
}
