# Arduino Radar Sistemi 📡

Bu proje, Arduino kullanılarak geliştirilmiş bir radar sistemidir. Servo motor üzerine yerleştirilen ultrasonik sensör sayesinde belirli açılar arasında tarama yapılarak çevredeki nesneler algılanır ve takip edilir.

## Özellikler
- Ultrasonik sensör ile mesafe ölçümü
- Servo motor ile açı bazlı tarama
- En yakın hedefi tespit etme ve kilitlenme (tracking)
- Seri port üzerinden veri gönderimi
- Yakın mesafede buzzer ile uyarı
- Gürültüyü azaltmak için ortalama ölçüm alma

## Kullanılan Donanımlar
- Arduino Uno
- Ultrasonik sensör (HC-SR04)
- Servo motor
- Buzzer

## Pin Bağlantıları
- trig → 2
- echo → 3
- Buzzer → 8
- Servo → 9

## Nasıl Çalışır?
- Servo motor belirli açı aralığında tarama yapar
- Her açı için mesafe ölçümü alınır
- Ölçülen açı ve mesafe verileri seri port üzerinden gönderilir ve radar benzeri görselleştirme yapılabilir
- En yakın nesne tespit edilir ve o yöne kilitlenilir
- Hedef kaybolursa sistem geniş açı taraması yapar
- Nesne çok yaklaştığında buzzer ile uyarı verilir

