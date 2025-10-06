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
const int pin_relay = 8;
bool nilai_relay = true;

// inisialisassi pin dan nilai2 LOADCELL
const int loadcell_dudukan_sck = 13;
const int loadcell_dudukan_dt = 12;
const int loadcell_sandaran_sck = 11;
const int loadcell_sandaran_dt = 10;
float kalibrasi_dudukan = -42921.66;  // SAYA DAPAT NILAI (1NOV) -42.921,66
float kalibrasi_sandaran = -82456.11; // SAYA DAPAT NILAI (1NOV) -82.456,11
float nilai_dudukan;
float nilai_sandaran;

// inisialisasi nilai milis loadcell (untuk store data agar milis berjalan)
unsigned long milis;
unsigned long milis_relay_sebelumnya;

float nilai_ke = 0;         // inisialisasi nilai untuk counter mundur
char user_input;            // inisialisasi nilai untuk user input
bool mulai_program = false; // bool untuk menyatakan sudah ada perintah dari komputer atau belum

HX711 loadcell_dudukan;
HX711 loadcell_sandaran;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(pin_relay, OUTPUT);
  digitalWrite(pin_relay, HIGH); // relay akan mati ketika keadaan HIGH

  // program awal loadcell pertama
  loadcell_dudukan.begin(loadcell_dudukan_dt, loadcell_dudukan_sck);
  loadcell_dudukan.set_scale(kalibrasi_dudukan);
  loadcell_dudukan.tare();
  Serial.println();
  // Serial.println("inisialisasi dudukan selesai");
  delay(500);

  // program loadcell kedua
  loadcell_sandaran.begin(loadcell_sandaran_dt, loadcell_sandaran_sck);
  loadcell_sandaran.set_scale(kalibrasi_sandaran);
  loadcell_sandaran.tare();
  // Serial.println("inisialisasi sandaran selesai");
  delay(500);

  // Serial.println("siap!");
  // Serial.println("------------------");
  // Serial.println("M = untuk mulai");
  // Serial.println("B = untuk berhenti");
  // delay(500);
}

void loop()
{
  if (Serial.available() > 0)
  {
    user_input = Serial.read(); // read user input
    if (user_input == 'M')      // mulai
      mulai_program = true;
    else if (user_input == 'B') // berhenti
      mulai_program = false;
  }

  // reset nilai milis relay
  milis = millis();

  // dapatkan nilai pembancaan sensor loadcell
  nilai_dudukan = loadcell_dudukan.get_units();   // baca nilai loadcell
  nilai_sandaran = loadcell_sandaran.get_units(); // baca nilai loadcell

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
  digitalWrite(pin_relay, nilai_relay);

  // mengrimkan data ke komputer
  if (nilai_ke < 10000 && mulai_program)
  {
    // Serial.print("nilai_ke: ");
    Serial.print(nilai_ke);
    Serial.print(",");
    // Serial.print("nilai_dudukan: ");
    Serial.print(nilai_dudukan);
    Serial.print(",");
    // Serial.print("nilai_sandaran: ");
    Serial.print(nilai_sandaran);
    Serial.print(",");
    // Serial.print("nilai_relay: ");
    Serial.println(nilai_relay);
  }
}
