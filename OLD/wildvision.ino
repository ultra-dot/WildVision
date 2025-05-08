#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Konfigurasi pin ESP32-CAM AI-Thinker
#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27
#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

#define LED_FLASH      4  // Flash dimatikan
#define PIR_PIN       15  // Pin sensor PIR
#define LED_INDICATOR 12  // LED Indikator (menyala saat mengirim foto)

const char* ssid = "ssid";
const char* password = "pw";
const char* serverUrl = "server:port/upload"; // FIXED

void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Kamera gagal diinisialisasi");
    return;
  }
}

void sendPhoto(camera_fb_t *fb) {
  if (!fb) {
    Serial.println("Gagal mengambil foto!");
    return;
  }

  WiFiClient client;
  HTTPClient http;

  Serial.println("Mengirim foto...");
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "image/jpeg");

  digitalWrite(LED_INDICATOR, HIGH);  // LED indikator menyala saat mengirim

  int httpResponseCode = http.POST(fb->buf, fb->len);

  if (httpResponseCode > 0) {
    Serial.println("Foto berhasil dikirim! Kode: " + String(httpResponseCode));
  } else {
    Serial.println("Gagal mengirim foto, kode: " + String(httpResponseCode));
  }

  http.end();
  digitalWrite(LED_INDICATOR, LOW);  // Matikan LED setelah selesai
}

void takePhoto() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Gagal mengambil foto");
    return;
  }

  sendPhoto(fb);
  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_FLASH, OUTPUT);
  digitalWrite(LED_FLASH, LOW);  // Matikan Flash
  pinMode(LED_INDICATOR, OUTPUT);
  digitalWrite(LED_INDICATOR, LOW);  // Matikan LED indikator

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi");

  startCamera();
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Gerakan terdeteksi! Mengambil foto...");
    takePhoto();
    delay(5000); // Hindari pengambilan foto terus-menerus
  }
}
