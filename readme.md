# Alat Tes Durability Kursi Kereta

Ini adalah repository untuk proyek alat tes destruksi/durability untuk kursi kereta yang dikerjakan oleh D-Tech Engineer.

Program ini diunggah ke Arduino Nano dan berkomunikasi dengan komputer melalui serial untuk mengirimkan data pengujian. Data ini kemudian dapat diolah, divisualisasikan, dan disimpan.

Kode sumber proyek ini tersedia di GitHub: [https://github.com/yuuahmad/tes-kursi-ketera](https://github.com/yuuahmad/tes-kursi-ketera)

## Persyaratan Hardware

*   Arduino Nano (atau Uno/Leonardo)
*   2x Sensor Load Cell dengan modul HX711
*   1x Modul Relay

## Pinout

| Komponen          | Pin Arduino |
| ----------------- | ----------- |
| Relay             | 8           |
| Load Cell Dudukan (DT)  | 12          |
| Load Cell Dudukan (SCK) | 13          |
| Load Cell Sandaran (DT) | 10          |
| Load Cell Sandaran (SCK)| 11          |

## Cara Penggunaan

1.  Rakit sirkuit sesuai dengan diagram pinout di atas.
2.  Unggah file `src/main.cpp` ke board Arduino Anda menggunakan PlatformIO atau Arduino IDE.
3.  Hubungkan alat ke komputer.
4.  Buka terminal serial (seperti Serial Monitor di Arduino IDE atau aplikasi eksternal seperti [QtSerialMonitor](https://github.com/mich-w/QtSerialMonitor)).
5.  Pilih port yang benar dan atur baud rate ke **115200**.
6.  Kirim karakter melalui terminal serial untuk mengontrol alat.

## Protokol Komunikasi Serial

*   `M`: Memulai siklus pengujian.
*   `B`: Menghentikan siklus pengujian.

## Format Data Output

Saat pengujian berjalan, alat akan mengirimkan data ke port serial dengan format berikut:

```
nilai_ke: [counter] nilai_dudukan: [kg] nilai_sandaran: [kg] nilai_relay: [status]
```

*   `nilai_ke`: Counter siklus (bertambah 0.5 setiap siklus relay).
*   `nilai_dudukan`: Pembacaan berat dari load cell dudukan (dalam kg).
*   `nilai_sandaran`: Pembacaan berat dari load cell sandaran (dalam kg).
*   `nilai_relay`: Status relay ( `1` untuk mati, `0` untuk hidup).

Pengujian akan berhenti secara otomatis setelah 10.000 siklus. Data yang diterima dapat disimpan dan dianalisis lebih lanjut menggunakan aplikasi seperti Microsoft Excel.