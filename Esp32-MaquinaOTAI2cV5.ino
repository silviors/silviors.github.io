#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "time.h"
#include "Wire.h"

#define slaveAdress 0x08

/*Put your SSID & Password*/
const char* ssid = "silviowless";  // Enter SSID here
const char* password = "@silvinho20061";  //Enter Password here


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600 * -4;

unsigned long tempo;
int intervalotime = 60000, horaint;
struct tm * horadata;
time_t horatxt;
String datatxt, calendario;
String readString;


int luzportao = 3;




WiFiServer server(80);
// Variable to store the HTTP request
String header;

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 161);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 0, 1);   //optional




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


  /////////////////////////////////////////////////////////////////////////////////
  WiFi.mode(WIFI_STA);
   // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, LOW);
    delay(4000);
    digitalWrite(2, HIGH);
    delay(1000);
  }


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
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);



  server.begin();
  delay(2000);



  tempo = -1 * intervalotime;

  Wire.begin(); // ingressa ao barramento I2C


}

void loop() {
  ArduinoOTA.handle();
  String hora, minuto, segundo, dia, mes;
  String semana[] = {"DOMINGO", "SEGUNDA-FEIRA", "TERCA-FEIRA", "QUARTA-FEIRA", "QUINTA-FEIRA", "SEXTA-FEIRA", "SABADO"};



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



    temperatura = (temprature_sens_read() - 32) / 1.8;
    tempo = millis();
  }





  WiFiClient client = server.available();   // Listen for incoming clients
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


          ligado_2 = controleluz("?2l", luzportao, ligado_2, "030", "031", "?2d");
          ligado_3 = controleluz("?3l", luzportao, ligado_3, "030", "031", "?3d");
          ligado_4 = controleluz("?4l", luzportao, ligado_4, "040", "041", "?4d");
          ligado_5 = controleluz("?5l", luzportao, ligado_5, "050", "051", "?5d");
          ligado_6 = controleluz("?6l", luzportao, ligado_6, "060", "061", "?6d");
          ligado_7 = controleluz("?7l", luzportao, ligado_7, "070", "071", "?7d");
          ligado_8 = controleluz("?8l", luzportao, ligado_8, "080", "081", "?8d");
          ligado_9 = controleluz("?9l", luzportao, ligado_9, "090", "091", "?9d");
          ligado_10 = controleluz("?10l", luzportao, ligado_10, "100", "101", "?10d");
          ligado_11 = controleluz("?11l", luzportao, ligado_11, "110", "111", "?11d");
          ligado_12 = controleluz("?12l", luzportao, ligado_12, "120", "121", "?12d");
          ligado_13 = controleluz("?13l", luzportao, ligado_13, "130", "131", "?13d");
          ligado_14 = controleluz("?14l", luzportao, ligado_14, "140", "141", "?14d");
          ligado_15 = controleluz("?15l", luzportao, ligado_15, "150", "151", "?15d");
          ligado_16 = controleluz("?16l", luzportao, ligado_16, "160", "161", "?16d");
          ligado_17 = controleluz("?17l", luzportao, ligado_17, "170", "171", "?17d");
          ligado_18 = controleluz("?18l", luzportao, ligado_18, "180", "181", "?18d");


          //Controle do rele15
          Serial.println(readString);
          //Liga o Rele 15
          if (readString.indexOf("?1l") > 0)enviahora("?1l", luzportao, ligado_1, "?1l");




          readString = "";

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>ESP32</title>");
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
          client.println("<br>TEMPERATURA CHIP: ");
          client.println(temperatura);
          client.println("oC<br>LUZ (0-1000): ");
          client.println(horaint);
          client.println("<br>ALARME: ");
          //if (analogRead(sensorAlarme) == 0) client.println("DESLIGADO"); else client.println("LIGADO");
          // if (inthora > portaonoite  || inthora < portaodia )
          // client.println("<br>ACIONA PORTAO: DESATIVADO"); else client.println("<br>ACIONA PORTAO: ATIVADO");
          client.println("</div>");


          idcabecalho(client, 1, "ENVIAR HORA");
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



          client.println("<div id='dv50'>INFORMACOES<br><br>");
          client.println("<br>");
          client.println("WiFi connected!");
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
          client.println("</div>");

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

          delay(10);
          client.stop();
        }
      }
    }
  }




  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    digitalWrite(2, LOW);
  } else    digitalWrite(2,HIGH );






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




boolean controleluz(String indligar, int nluz, boolean ctrligado, String codigorfl, String codigorfd, String inddesligar) {
  if (readString.indexOf(indligar) > 0)
  {
    digitalWrite(nluz, HIGH);
    ctrligado = false;
    int len = codigorfl.length() + 1;
    char arraychar[len];
    codigorfl.toCharArray(arraychar, len);
    Wire.beginTransmission(slaveAdress);
    for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
    Wire.endTransmission();
  }
  else
  {
    if (readString.indexOf(inddesligar) > 0)
    {
      digitalWrite(nluz, LOW );
      ctrligado = true;
      int len = codigorfd.length() + 1;
      char arraychar[len];
      codigorfd.toCharArray(arraychar, len);
      Wire.beginTransmission(slaveAdress);
      for (int i = 0; i < len; ++i)Wire.write(arraychar[i]);
      Wire.endTransmission();
    }
  }




  return ctrligado;
}

boolean enviahora(String indligar, int nluz, boolean ctrligado, String inddesligar) {
  if (readString.indexOf(indligar) > 0)
  {
    digitalWrite(nluz, HIGH);
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
      digitalWrite(nluz, LOW );
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
