#include <Wire.h>
#include <VirtualWire.h>
#include <LiquidCrystal.h>

//if (valor_recebido_RF == 10) acaorele(acao, rele0);9 EDICULA
//      if (valor_recebido_RF == 11) acaorele(acao, rele1);8 EDI LATERAL
//      if (valor_recebido_RF == 12) acaorele(acao, rele2);13 EDIC JARDIM
//      if (valor_recebido_RF == 13) acaorele(acao, rele3);17 FUND JARDIM
//      if (valor_recebido_RF == 16) acaorele(acao, rele4);14 OFICINA
//      if (valor_recebido_RF == 17) acaorele(acao, rele5);16 REFLET MURO
//      if (valor_recebido_RF == 18) acaorele(acao, rele6);15 PORAO

int valor_recebido_RF = 500, rfhora = 0, rfminuto = 0, rfsegundo = 0, pinoSom = 6, acao = 0,
    codigorcb, inthora = 0;
//int pinorf = 7, rele0 = 8, rele1 = 9, rele2 = 13, rele3 = 14, rele4 = 15, rele5 = 16, rele6 = 17;
int pinorf = 7, rele0 = 9, rele1 = 8, rele2 = 13, rele3 = 17, rele4 = 14, rele5 = 16, rele6 = 15;
char recebido_RF_char[9];
String acoes[] = {"ALARM DISPARADO", "PORTAO", "LZPORTAO", "LZGARAGEM", "LZHALL", "LZSALATV",
                  "LZVARANDA", "LZESCEXTERNA", "LZJARDFRENTE", "REFLFRENTE", "LZEDICULA", "LZEDILATERAL",
                  "LZEDICJARDIM", "LZFUNDJARDIM", "ALARME", "SIRENE", "LZOFICINA",
                  "LZRMURO", "LZPORAO", "19", "TMJARDIM", "CAMPAINHA", "TMREFFRENT"
                 };

String horasistema = "silvinho mane!", horaset ;
boolean c13 = true;
float ctrelogio;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup()
{
  pinMode(pinoSom, OUTPUT);
  pinMode(rele0, OUTPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  pinMode(rele5, OUTPUT);
  pinMode(rele6, OUTPUT);
  digitalWrite(rele0, HIGH);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  digitalWrite(rele3, HIGH);
  digitalWrite(rele4, HIGH);
  digitalWrite(rele5, HIGH);
  digitalWrite(rele6, HIGH);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("LOAD");
  ctrelogio = millis();
  rfsegundo = 0;
  rfminuto = 0;
  rfhora = 0;
  vw_set_rx_pin(pinorf);
  //Velocidade de comunicacao (bits por segundo)
  vw_setup(5000);
  //Inicia a recepcao
  vw_rx_start();
}

void loop()
{

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;


  Serial.println(vw_get_message(buf, &buflen));


  if (vw_get_message(buf, &buflen))
  {
    int i, tamtxt;
    for (i = 0; i < buflen; i++)
    {
      //Armazena os caracteres recebidos
      recebido_RF_char[i] = char(buf[i]);
    }
    recebido_RF_char[buflen] = '\0';
    horasistema = recebido_RF_char;

    //Converte o valor recebido para integer
    valor_recebido_RF = atoi(recebido_RF_char);

    if (horasistema.length() == 3) {
      if (horasistema.length() == 2)  {
        valor_recebido_RF = horasistema.substring(0, 1).toInt();
        acao = horasistema.substring(1, 2).toInt();
      }
      else {
        valor_recebido_RF = horasistema.substring(0, 2).toInt();
        acao = horasistema.substring(2, 3).toInt();
      }

      if (valor_recebido_RF == 10) acaorele(acao, rele0);
      if (valor_recebido_RF == 11) acaorele(acao, rele1);
      if (valor_recebido_RF == 12) acaorele(acao, rele2);
      if (valor_recebido_RF == 13) acaorele(acao, rele3);
      if (valor_recebido_RF == 16) acaorele(acao, rele4);
      if (valor_recebido_RF == 17) acaorele(acao, rele5);
      if (valor_recebido_RF == 18) acaorele(acao, rele6);
      if (valor_recebido_RF == 20) {
        acaorele(0, rele3);
        acaorele(0, rele5);
      }
      if (valor_recebido_RF == 21) {
        tone (pinoSom, 500, 5000);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(acoes[21]);
        delay(1000);
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print(acoes[21]);
        delay(1000);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(acoes[21]);
        lcd.setCursor(3, 1);
        lcd.print(acoes[21]);
        delay(1000);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(acoes[21]);
        delay(1000);
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print(acoes[21]);
        delay(1000);

      }






      tamtxt = acoes[valor_recebido_RF].length();
      lcd.setCursor(8, 0);
      if (acao == 0) lcd.print("+"); else lcd.print("-");
      lcd.print(acoes[valor_recebido_RF]);

      if (valor_recebido_RF != 21) tone(pinoSom, 2000, 300);
      codigorcb = valor_recebido_RF;
      valor_recebido_RF = 500;
    }

    if (horasistema.length() == 8) {
      inthora = horasistema.substring(0, 2).toInt() * 100 + horasistema.substring(3, 5).toInt();
      rfhora = horasistema.substring(0, 2).toInt();
      rfminuto = horasistema.substring(3, 5).toInt();
      rfsegundo = horasistema.substring(6, 8).toInt();
      horaset = horasistema;
    }
    if (horasistema.length() == 7) {
      lcd.setCursor(8, 0);
      lcd.print("   " + horasistema.substring(1, 3) + "/" + horasistema.substring(3, 5));
    }

    if (horasistema.length() == 4) {
      if (horasistema.substring(0, 1) == "1") {
        lcd.setCursor(9, 1);
        lcd.print("*C L");
        lcd.print(horasistema.substring(1));
      }
      else if (horasistema.substring(0, 1) == "2") {
        lcd.setCursor(3, 1);
        lcd.print("*C E");
        lcd.print(horasistema.substring(2));
      }
      else if (horasistema.substring(0, 1) == "3") {
        lcd.setCursor(0, 1);
        lcd.print("T");
        lcd.print(horasistema.substring(2));
      }
      //txx*c u77% L:234
    }

  }

  if (millis() - ctrelogio > 1000) {
    ctrelogio = millis();
    rfsegundo++;
    if (rfsegundo > 59) {
      rfsegundo = 0;
      rfminuto++;
    }
    if (rfminuto > 59) {
      rfminuto = 0;
      rfhora++;
    }
    if (rfhora > 23) {
      rfhora = 0;
    }
    lcd.setCursor(0, 0);
    if (rfhora > 9) lcd.print(""); else lcd.print("0");
    lcd.print(rfhora);
    if (rfminuto > 9) lcd.print(":"); else lcd.print(":0");
    lcd.print(rfminuto);
    if (rfsegundo > 9) lcd.print(":"); else lcd.print(":0");
    lcd.print(rfsegundo);
  }


  if (rfhora == 1) {
    acaorele(1, rele3);
    acaorele(1, rele5);
  }


}








boolean par (int num) {
  if (num % 2 == 0)
    return true;
  else return false;
}

void acaorele (int acaox, int relex) {
  if (acaox == 0) digitalWrite(relex, LOW); else digitalWrite(relex, HIGH);
}





void relogio() {

  if (millis() - ctrelogio > 1000) {
    ctrelogio = millis();
    rfsegundo++;
    if (rfsegundo > 59) {
      rfsegundo = 0;
      rfminuto++;
    }
    if (rfminuto > 59) {
      rfminuto = 0;
      rfhora++;
    }
    if (rfhora > 23) {
      rfhora = 0;
    }
  }



}

