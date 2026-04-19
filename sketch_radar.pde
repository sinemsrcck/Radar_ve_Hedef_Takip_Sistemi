import processing.serial.*;

// SERI PORT
Serial myPort;

//  VERI DEGISKENLERI
String veri = "";
int aci = 0;
int mesafe = 80;
/*
  Pencere oluşturulur ve seri port başlatılır
*/
void setup() {
  size(900, 600);
  smooth();

  printArray(Serial.list());   // mevcut portları listeler

  // Kendi Arduino portuna göre index değiştirilmeli
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n');
}

// -------------------- ANA CIZIM DONGUSU --------------------
/*
  Radar arayüzünü sürekli günceller
*/
void draw() {
  background(10);
  translate(width/2, height*0.9);

  radarCiz();
  taramaCizgisiCiz();
  hedefCiz();
  bilgiYaz();
}
// -------------------- SERI VERI OKUMA --------------------
/*
  Arduino'dan gelen veri işlenir
  Formatlar:
  - "aci,mesafe"
  - "LOCK,aci,mesafe"
*/
void serialEvent(Serial myPort) {
  String veri = myPort.readStringUntil('\n');

  if (veri != null) {
    veri = trim(veri);
    println("GELEN: " + veri);

    if (veri.startsWith("LOCK")) {
      String[] parcalar = split(veri, ',');
      if (parcalar.length == 3) {
        aci = Integer.parseInt(parcalar[1]);
        mesafe = Integer.parseInt(parcalar[2]);
      }
    } else {
      String[] parcalar = split(veri, ',');
      if (parcalar.length == 2) {
        aci = Integer.parseInt(parcalar[0]);
        mesafe = Integer.parseInt(parcalar[1]);
      }
    }
  }
}
// -------------------- RADAR ARAYUZU --------------------
/*
  Radar halkalarını ve açı çizgilerini çizer
*/
void radarCiz() {
  stroke(0, 255, 100);
  noFill();

  // Yarım daire halkaları
  arc(0, 0, 800, 800, PI, TWO_PI);
  arc(0, 0, 600, 600, PI, TWO_PI);
  arc(0, 0, 400, 400, PI, TWO_PI);
  arc(0, 0, 200, 200, PI, TWO_PI);

  // Açı çizgileri
  for (int a = 0; a <= 180; a += 30) {
    float x = 400 * cos(radians(180 - a));
    float y = -400 * sin(radians(180 - a));
    line(0, 0, x, y);
  }
}
/*
  Aktif tarama çizgisini gösterir
*/
void taramaCizgisiCiz() {
  stroke(0, 255, 0);
  strokeWeight(3);

  float x = 400 * cos(radians(180 - aci));
  float y = -400 * sin(radians(180 - aci));
  line(0, 0, x, y);
}
/*
  Tespit edilen hedefi radar üzerinde çizer
  Mesafeye göre renk ve boyut değişir
*/
void hedefCiz() {
  if (mesafe > 0 && mesafe < 80) {
    float r = map(mesafe, 0, 80, 0, 400);

    float x = r * cos(radians(180 - aci));
    float y = -r * sin(radians(180 - aci));

    float boyut = map(mesafe, 0, 80, 22, 8);

    noStroke();

    if (mesafe <= 15) {
      fill(255, 0, 0);       // yakın = kırmızı
    } else if (mesafe <= 40) {
      fill(255, 200, 0);     // orta = sarı
    } else {
      fill(0, 255, 100);     // uzak = yeşil
    }

    ellipse(x, y, boyut, boyut);
  }
}
/*
  Ekrana bilgi yazılarını basar
*/
void bilgiYaz() {
  resetMatrix();

  fill(0, 255, 100);
  textSize(20);
  textAlign(LEFT);
  text("Aci: " + aci + "°", 30, 30);
  text("Mesafe: " + mesafe + " cm", 30, 60);

  if (mesafe > 0 && mesafe < 80) {
    if (mesafe <= 15) {
      fill(255, 0, 0);
      text("TARGET LOCKED - YAKIN", 30, 95);
    } else if (mesafe <= 40) {
      fill(255, 200, 0);
      text("TARGET DETECTED - ORTA", 30, 95);
    } else {
      fill(0, 255, 100);
      text("TARGET DETECTED - UZAK", 30, 95);
    }
  } else {
    fill(150);
    text("HEDEF YOK", 30, 95);
  }
}
