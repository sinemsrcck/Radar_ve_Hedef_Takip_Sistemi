#include <Servo.h>

//  NESNE TANIMI 
Servo servoMotor;

// PIN TANIMLARI 
const int trigPin = 2;
const int echoPin = 3;
const int buzzerPin = 8;

// SISTEM PARAMETRELERI
const int maxMesafe = 80;     // maksimum algılama mesafesi (cm)
const int yakinEsik = 15;     // yakın mesafe uyarı eşiği

int mevcutAci = 90;           // servonun anlık açısı
int hedefAci = 90;            // hedeflenen açı (yumuşatma için)
int kayipSayaci = 0;          // hedef kaybolursa sayacı
/*
  Ultrasonik sensör ile tek seferlik mesafe ölçümü yapar
*/
long mesafeOlc() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long sure = pulseIn(echoPin, HIGH, 30000);

  if (sure == 0) return 300; // sensör okumazsa uzak kabul et

  return sure * 0.034 / 2;
}
/*
  Gürültüyü azaltmak için birden fazla ölçüm alır
  Geçersiz ölçümleri filtreleyerek ortalama döndürür
*/
long temizMesafeOlc() {
  long toplam = 0;
  int sayac = 0;

  for (int i = 0; i < 3; i++) {
    long m = mesafeOlc();

    if (m > 5 && m < 200) {
      toplam += m;
      sayac++;
    }

    delay(8);
  }

  if (sayac == 0) return 300;

  return toplam / sayac;
}
/*
  Pin ayarları ve başlangıç konfigürasyonu
*/
void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  servoMotor.attach(9);
  servoMotor.write(mevcutAci);
}
/*
  Radar tarama algoritması:
  - Belirli açı aralığında tarama yapar
  - En yakın nesneyi bulur
  - Hedefe kilitlenir (LOCK)
  - Hedef kaybolursa geniş tarama yapar
*/
void loop() {
  int enIyiAci = -1;
  long enKucukMesafe = 300;
  // Mevcut açının etrafında dar tarama
  int baslangic = max(0, mevcutAci - 20);
  int bitis = min(180, mevcutAci + 20);

  for (int aci = baslangic; aci <= bitis; aci += 3) {
    servoMotor.write(aci);
    delay(100);

    long mesafe = temizMesafeOlc();

    int gonderilecekMesafe = mesafe;
    if (gonderilecekMesafe > maxMesafe) gonderilecekMesafe = 0;
    // Seri porta veri gönder (radar görselleştirme için)
    Serial.print(aci);
    Serial.print(",");
    Serial.println(gonderilecekMesafe);
    // En yakın nesneyi bul
    if (mesafe <= maxMesafe && mesafe < enKucukMesafe) {
      enKucukMesafe = mesafe;
      enIyiAci = aci;
    }
  }

  // HEDEFE KILITLENME 
  if (enIyiAci != -1) {
    kayipSayaci = 0;

    hedefAci = (hedefAci + enIyiAci) / 2;   // yumuşatma
    mevcutAci = hedefAci;
    servoMotor.write(hedefAci);

    Serial.print("LOCK,");
    Serial.print(hedefAci);
    Serial.print(",");
    Serial.println(enKucukMesafe);
    // Yakınsa buzzer ile uyar
    if (enKucukMesafe <= yakinEsik) {
      digitalWrite(buzzerPin, HIGH);
      delay(120);
      digitalWrite(buzzerPin, LOW);
    }
  }
   // HEDEF KAYBI
   else {
    kayipSayaci++;
    // Belirli sayıda kayıptan sonra geniş tarama
    if (kayipSayaci >= 3) {
      for (int aci = 0; aci <= 180; aci += 10) {
        servoMotor.write(aci);
        delay(60);

        long mesafe = temizMesafeOlc();

        int gonderilecekMesafe = mesafe;
        if (gonderilecekMesafe > maxMesafe) gonderilecekMesafe = 0;

        Serial.print(aci);
        Serial.print(",");
        Serial.println(gonderilecekMesafe);
        // Yeni hedef bulunursa o açıya geç
        if (mesafe <= maxMesafe) {
          mevcutAci = aci;
          hedefAci = aci;
          break;
        }
      }
    }
  }
}
