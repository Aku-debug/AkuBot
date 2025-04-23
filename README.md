# AkuBot

**AkuBot**, Aim Lab oyununu otomatik başlatan, siyah hedefleri (RGB: 0, 0, 0) ekranın ortasında tespit ettiğinde tıklama yapan ve kullanıcı girişlerine göre davranan bir C++ uygulamasıdır.

## 🔧 Özellikler

- 🖱️ Otomatik mouse tıklama (siyah hedef varsa)
- 🎮 Aim Lab çalışıyorsa aktif olur
- 🎯 Hedef rengine göre tıklama (merkez piksel kontrolü)
- 🧠 Akıllı renk toleransı (varsayılan: 30)
- 🖥️ `Insert`, `Delete` ve `Home` tuşları ile kontrol
- 🔒 Arka planda çalışır, kullanıcı komutlarını dinler

## 🔑 Kısayol Tuşları

| Tuş       | Görev                         |
|-----------|-------------------------------|
| `Insert`  | Aim Lab'ı başlatır            |
| `Delete`  | Aim Lab'ı kapatır             |
| `Home`    | AkuBot'u sonlandırır          |

## 🚀 Nasıl Çalışır?

1. Program başlatıldığında CMD ekranin da çalışır.
2. Her 1ms'de bir ekranın ortasındaki pikselin rengi kontrol edilir.
3. Renk siyaha yakınsa (±30 toleransla), mouse tıklaması gönderilir.
4. Aim Lab yalnızca ön plandaysa çalışır.
5. Kullanıcı, klavyeden `Insert`, `Delete` veya `Home` tuşlarını kullanarak uygulamayı kontrol edebilir.

## 📁 Aim Lab Yolu Algılama

Uygulama, Aim Lab'ın kurulu olduğu yaygın dizinleri kontrol eder:

- `C:\SteamLibrary\steamapps\common\Aim Lab\AimLab_tb.exe`
- `D:\SteamLibrary\steamapps\common\Aim Lab\AimLab_tb.exe`
- `E:\SteamLibrary\steamapps\common\Aim Lab\AimLab_tb.exe`
- (Yoksa: `C:\Program Files (x86)\Steam\steamapps\common\Aim Lab\...` yolu varsayılır)

## 🧪 Gereksinimler

- Windows işletim sistemi
- `WinAPI` fonksiyonları için uygun C++ derleyici (MSVC önerilir)

## 🛠️ Derleme

```bash
g++ main.cpp -o AkuBot.exe -lpsapi -lshlwapi
