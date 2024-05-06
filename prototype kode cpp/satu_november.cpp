#include <Arduino.h>
#include "HX711.h"
// #include "avr_debugger.h"
// #include <app_api.h>
/*
ini adalah projek untuk alat tes destruksi / dulability kursi kereta d-tech
kode ini diupload ke arduino nano dan dengan komunikasi serialnya, arduino ini akan mengirimkan data
ke laptop untuk selanjutnya diolah, divisualisasi, dan disimpan

anda dapat melihat kode ini di github saya (masih publik) di
https://github.com/yuuahmad/tes-kursi-ketera
*/

// inisialisassi pin RELAY
// const int RELAY_1 = 7;
const int RELAY_2 = 6;
bool nilai_relay = true;

// inisialisassi pin dan nilai2 LOADCELL
const int LOADCELL_DUDUKAN_SCK = 13; // ini loadcell dudukan
const int LOADCELL_DUDUKAN_DT = 12;
const int LOADCELL_SANDARAN_SCK = 11; // ini loadcell sandaran
const int LOADCELL_SANDARAN_DT = 10;
float KALIBRASI_DUDUKAN = -42921.66; // Kalibrasi sensor Load Cell 2
// SAYA DAPAT NILAI (1NOV) -42.921,66
float KALIBRASI_SANDARAN = -82456.11; // Kalibrasi sensor Load Cell 1
// SAYA DAPAT NILAI (1NOV) -82.456,11
float NILAI_DUDUKAN;
float NILAI_SANDARAN;

// inisialisasi nilai milis loadcell (untuk store data agar milis berjalan)
unsigned long milis_relay;
unsigned long milis_relay_sebelumnya;
unsigned long milis_loadcell;

// inisialisasi nilai untuk counter mundur
float nilai_ke = 0;
// inisialisasi nilai untuk user input
char userInput;
// bool untuk menyatakan sudah ada perintah dari komputer atau belum
bool mulai_program = false;

HX711 LOADCELL_DUDUKAN;
HX711 LOADCELL_SANDARAN;

void setup()
{
    Serial.begin(38400);
    pinMode(RELAY_2, OUTPUT);
    digitalWrite(RELAY_2, HIGH); // relay akan mati ketika keadaan HIGH
    // program eksekusi awal loadcell pertama
    LOADCELL_DUDUKAN.begin(LOADCELL_DUDUKAN_DT, LOADCELL_DUDUKAN_SCK);
    LOADCELL_DUDUKAN.set_scale(KALIBRASI_DUDUKAN);
    LOADCELL_DUDUKAN.tare();
    // program eksekusi loadcell kedua
    LOADCELL_SANDARAN.begin(LOADCELL_SANDARAN_DT, LOADCELL_SANDARAN_SCK);
    LOADCELL_SANDARAN.set_scale(KALIBRASI_SANDARAN);
    LOADCELL_SANDARAN.tare();
}

void loop()
{
    // perintah ini gagal karena dapat menyebabkan loadcell mengganggu penerimaan karakter dari komputer
    if (Serial.available() > 0)
    {
        userInput = Serial.read(); // read user input
        if (userInput == 'g')
        { // if we get expected value
            mulai_program = true;
        } // if user input 'g'
    }     // Serial.available

    // reset nilai milis relay
    milis_relay = millis();

    // dapatkan nilai pembancaan sensor loadcell
    // loadcell pertama
    NILAI_DUDUKAN = LOADCELL_DUDUKAN.get_units(); // baca nilai loadcell
    // // loadcell kedua
    NILAI_SANDARAN = LOADCELL_SANDARAN.get_units(); // baca nilai loadcell

    // program untuk switching relay
    if ((milis_relay - milis_relay_sebelumnya) >= 1200 && mulai_program) // jangan gunakan delay, tapi milis dan karena 1hz maka nilai hidup dan nilai mati 1/2 detik
    {
        if (nilai_ke < 10000)
            nilai_relay = !nilai_relay; // program untuk melakukan switcing pada relay
        else
            nilai_relay = true; // ini artinya nilai relay akan mati (seperti diatas, karena saya pakai normal close dan false/LOW artinya hidup)
        nilai_ke = nilai_ke + 0.5;
        milis_relay_sebelumnya = milis_relay;
    }
    // digitalWrite(RELAY_1, nilai_relay);
    digitalWrite(RELAY_2, nilai_relay);

    // program untuk mengrimkan data ke komputer
    if (nilai_ke < 10000 && mulai_program) // tunggu maksimal 1 setik untuk memastikan sensor hadir/terpasang dengan benar
    {
        Serial.print(nilai_ke);
        Serial.print(",");
        Serial.print(NILAI_DUDUKAN);
        Serial.print(",");
        Serial.print(NILAI_SANDARAN);
        Serial.print(",");
        Serial.println(nilai_relay);
    }
}