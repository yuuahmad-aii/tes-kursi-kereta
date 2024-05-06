#include <Arduino.h>
#include "HX711.h"
#include <Wire.h>
/*
ini adalah projek untuk alat tes destruksi / durability kursi kereta d-tech
kode ini diupload ke arduino nano dan dengan komunikasi serialnya, arduino ini akan mengirimkan data
ke laptop untuk selanjutnya diolah, divisualisasi, dan disimpan
anda dapat melihat kode ini di github saya di
https://github.com/yuuahmad/tes-kursi-ketera
*/

// inisialisassi relay
const int RELAY_2 = 6;
bool nilai_relay = true;
bool nilai_relay_sekarang = true;

// inisialisassi pin dan nilai2 LOADCELL
const int LOADCELL_SCK = 13; // ini loadcell dudukan dan sandaran
const int LOADCELL_DUDUKAN_DT = 12;
const int LOADCELL_SANDARAN_DT = 10;
float KALIBRASI_DUDUKAN = -42921.66;  // SAYA DAPAT NILAI (1NOV) -42.921,66
float KALIBRASI_SANDARAN = -82456.11; // SAYA DAPAT NILAI (1NOV) -82.456,11
float NILAI_DUDUKAN;
float NILAI_SANDARAN;
int minValue_dudukan, minValue_sandaran;
int maxValue_dudukan, maxValue_sandaran;

// inisialisasi nilai milis loadcell (untuk store data agar milis berjalan)
unsigned long milis;
unsigned long milis_relay_sebelumnya;

float nilai_ke = 0;         // inisialisasi nilai untuk counter mundur
char userInput;             // inisialisasi nilai untuk user input
bool mulai_program = false; // bool untuk menyatakan sudah ada perintah dari komputer atau belum

HX711 LOADCELL_DUDUKAN;
HX711 LOADCELL_SANDARAN;

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_2, HIGH); // relay akan mati ketika keadaan HIGH

  // program awal loadcell pertama
  LOADCELL_DUDUKAN.begin(LOADCELL_DUDUKAN_DT, LOADCELL_SCK);
  LOADCELL_DUDUKAN.set_scale(KALIBRASI_DUDUKAN);
  LOADCELL_DUDUKAN.tare();
  Serial.println("inisialisasi dudukan selesai");
  delay(500);

  // program loadcell kedua
  LOADCELL_SANDARAN.begin(LOADCELL_SANDARAN_DT, LOADCELL_SCK);
  LOADCELL_SANDARAN.set_scale(KALIBRASI_SANDARAN);
  LOADCELL_SANDARAN.tare();
  Serial.println("inisialisasi sandaran selesai");
  delay(100);

  Serial.println("siap!");
  Serial.println("------------------");
  Serial.println("M = untuk mulai");
  Serial.println("B = untuk berhenti");
  delay(500);
}

void loop()
{
  if (Serial.available() > 0)
  {
    userInput = Serial.read(); // read user input
    if (userInput == 'M')      // mulai
      mulai_program = true;
    else if (userInput == 'B') // berhenti
      mulai_program = false;
  }

  // reset nilai milis relay
  milis = millis();

  // dapatkan nilai pembancaan sensor loadcell
  NILAI_DUDUKAN = LOADCELL_DUDUKAN.get_units();   // baca nilai loadcell
  NILAI_SANDARAN = LOADCELL_SANDARAN.get_units(); // baca nilai loadcell

  // program untuk switching relay
  if ((milis - milis_relay_sebelumnya) >= 1200 && mulai_program)
  {
    milis_relay_sebelumnya = milis;
    if (nilai_ke < 10000)
      nilai_relay = !nilai_relay;
    else
      nilai_relay = true; // true = relay mati = program berhenti
    nilai_ke = nilai_ke + 0.5;
  }
  digitalWrite(RELAY_2, nilai_relay);

  // mengrimkan data ke komputer
  if (nilai_ke < 10000 && mulai_program)
  {
    Serial.print("nilai_ke: ");
    Serial.print(nilai_ke);
    Serial.print(" ");
    Serial.print("NILAI_DUDUKAN: ");
    Serial.print(NILAI_DUDUKAN);
    Serial.print(" ");
    Serial.print("NILAI_SANDARAN: ");
    Serial.print(NILAI_SANDARAN);
    Serial.print(" ");
    Serial.print("nilai_relay: ");
    Serial.println(nilai_relay);
  }
}
