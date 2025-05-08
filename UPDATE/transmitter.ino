#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ======================= Konfigurasi Pin =======================
#define DHTPIN 4                  // Pin untuk sensor DHT11
#define DHTTYPE DHT11             // Jenis sensor DHT
#define OLED_RESET -1             // Pin reset untuk OLED (gunakan -1 jika tidak ada)
#define SCREEN_WIDTH 128          // Lebar layar OLED
#define SCREEN_HEIGHT 64         // Tinggi layar OLED

// ======================= Pin LoRa =======================
#define LORA_SCK     18          // Pin SPI SCK
#define LORA_MISO    19          // Pin SPI MISO
#define LORA_MOSI    23          // Pin SPI MOSI
#define LORA_SS      5           // Pin Chip Select LoRa
#define LORA_RST     14          // Pin Reset LoRa
#define LORA_DIO0    26          // Pin Interrupt LoRa

// ======================= Objek =======================
DHT dht(DHTPIN, DHTTYPE);                     // Objek sensor DHT11
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Objek OLED

// ======================= Setup =======================
void setup() {
  Serial.begin(9600);  // Memulai komunikasi serial untuk debugging
  dht.begin();         // Memulai sensor DHT11

  // Inisialisasi OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Gagal inisialisasi OLED"));
    while (true);  // Menjaga program tetap berhenti jika OLED gagal
  }
  display.setRotation(2);  // Balik layar 180 derajat
  display.clearDisplay();  // Membersihkan layar OLED
  display.setTextSize(1);  // Ukuran teks
  display.setTextColor(SSD1306_WHITE);  // Warna teks OLED

  // Inisialisasi LoRa
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);  // Reset LoRa
  delay(10);
  digitalWrite(LORA_RST, HIGH);
  delay(10);

  // Menyiapkan LoRa pada frekuensi 433 MHz
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);  // Menentukan pin LoRa
  if (!LoRa.begin(433E6)) {  // Inisialisasi LoRa pada frekuensi 433 MHz
    Serial.println("Gagal menginisialisasi LoRa");
    while (true);  // Menjaga program tetap berhenti jika LoRa gagal
  }
  Serial.println("LoRa berhasil diinisialisasi");
}

// ======================= Loop =======================
void loop() {
  // Membaca suhu dan kelembapan dari DHT11
  float suhu = dht.readTemperature();       // Membaca suhu dalam Celcius
  float kelembaban = dht.readHumidity();   // Membaca kelembapan dalam persen

  // Mengecek jika pembacaan sensor gagal
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca sensor DHT11");
    return;  // Menghentikan loop jika terjadi kesalahan pembacaan sensor
  }

  // Membuat string data untuk dikirimkan
  String data = "Suhu: " + String(suhu) + " C, Kelembaban: " + String(kelembaban) + " %";
  Serial.println("Mengirim: " + data);

  // Menampilkan data suhu dan kelembapan pada OLED
  display.clearDisplay();  // Membersihkan layar OLED
  display.setCursor(0, 0);  // Mengatur posisi awal cursor pada OLED
  display.println("DHT11 Transmitter");
  display.print("Suhu: ");
  display.print(suhu);
  display.println(" C");
  display.print("Kelembaban: ");
  display.print(kelembaban);
  display.println(" %");
  display.display();  // Menampilkan data di layar OLED

  // Mengirimkan data melalui LoRa
  LoRa.beginPacket();  // Memulai paket LoRa
  LoRa.print(data);    // Mengirim data suhu dan kelembapan
  LoRa.endPacket();    // Mengakhiri pengiriman paket LoRa

  delay(5000);  // Delay 5 detik sebelum pengiriman data berikutnya
}
