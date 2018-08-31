#include <SPI.h>
#include <cp437font.h>
#include <LedMatrix.h>
#include <WiFiClient.h>
#include <HTTPClient.h>



LedMatrix ledMatrix = LedMatrix(8, 18, -1, 23, 5);
//LedMatrix ledMatrix = LedMatrix(1, 23, 23, 18, 5);

void rocket(void)
{
  byte rocket[8] = {
    0b00011110,
    0b00000010,
    0b00000111,
    0b01111111,
    0b01111111,
    0b00000111,
    0b00000010,
    0b00011110,
  };
  byte rocket2[8] = {
    0b00111111,
    0b01000011,
    0b00000111,
    0b00111110,
    0b00111110,
    0b00000111,
    0b01000011,
    0b00111111,
  };

  byte rocket3[8] = {
    // This is the first frame for alien #1
    // If you squint you can kind of see the
    // image in the 0's and 1's
    B10100101,
    B01011010,
    B00100100,
    B11111111,
    B11011011,
    B01111110,
    B00111100,
    B00011000,
  };


  ledMatrix.clear();
  ledMatrix.commit();
  for (byte dev = 0; dev < 12; ++dev) {
    for (byte p = 0; p < 8; ++p) {

      byte reverse = 7 - p;
      byte help;
      byte new_col;
      for (byte x = 0; x < 8; ++x) {
        // last rocket... we need to cleanup
        help = (rocket2[x] & (0b10000000 >> p)) >> reverse;
        ledMatrix.setByteOnDisplayFix(dev - 3, x, rocket2[x] << (p + 1));
        ledMatrix.blitColumnMove(dev - 2, x, help);

        help = (rocket[x] & (0b10000000 >> p)) >> reverse;
        ledMatrix.blitColumnMove(dev - 1, x, help);

        help = (rocket3[x] & (0b10000000 >> p)) >> reverse;
        ledMatrix.blitColumnMove(dev, x, help);
                /*
        help = (rocket2[x] & (0b10000000 >> p)) >> reverse;
        new_col = (ledMatrix.getColumn(dev - 2, x) << 1) | help;
        ledMatrix.setByteOnDisplayFix(dev - 3, x, rocket2[x] << (p + 1));
        ledMatrix.setByteOnDisplayFix(dev - 2, x, new_col);

        help = (rocket[x] & (0b10000000 >> p)) >> reverse;
        new_col = (ledMatrix.getColumn(dev - 1, x) << 1) | help;
        ledMatrix.setByteOnDisplayFix(dev - 1, x, new_col);

        help = (rocket3[x] & (0b10000000 >> p)) >> reverse;
        new_col = (ledMatrix.getColumn(dev, x) << 1) | help;
        ledMatrix.setByteOnDisplayFix(dev, x, new_col);
        */
      }
      ledMatrix.commit();
      //ledMatrix.clear();
      delay(50);
    }
  }

}

void setup() {
  Serial.begin(115200);
  delay(100);

  ledMatrix.init();
  ledMatrix.clear();
  ledMatrix.commit();
  ledMatrix.setIntensity(1);
  ledMatrix.clear();
  ledMatrix.commit();
  ledMatrix.setCharWidth(8);
  ledMatrix.setTextAlignment(TEXT_ALIGN_LEFT);
  ledMatrix.setText("WiFiWait");
  ledMatrix.drawText();
  ledMatrix.commit();
  Serial.println("Attempting to connect to SSID: ");
  WiFi.begin("mywifi", "password");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }
  ledMatrix.clear();
  ledMatrix.commit();
}

void loop() {
  HTTPClient http;

  http.begin("http://192.168.32.61:8000/index.html");
  const int httpCode = http.GET();
  if (httpCode <= 0) {
    ledMatrix.clear();
    ledMatrix.setText("ServFail");
    ledMatrix.drawText();
    ledMatrix.commit();
    delay(5000);
    return;
  }

  if (httpCode != 200) {
    delay(5000);
    return;
  }

  String payload = http.getString();
  ledMatrix.clear();
  ledMatrix.commit();
  if (payload == "rockets rocks") {
    rocket();
  }
  ledMatrix.clear();
  ledMatrix.commit();

  if (payload.length() > 8) {
    bool init = true;
    for (byte p = 0; p < payload.length(); ++p) {
      String real_payload = payload.substring(p, p + 8);
      ledMatrix.setText(real_payload);

      ledMatrix.clear();
      ledMatrix.drawText();
      ledMatrix.commit();
      if (init) {
        delay(2000);
        init = false;
      }

      delay(600);
    }
  } else {
    ledMatrix.setText(payload);
    ledMatrix.clear();
    ledMatrix.drawText();
    ledMatrix.commit();
    delay(5000);
    return;
  }
  ledMatrix.clear();
  ledMatrix.commit();
  delay(4000);
  // put your main code here, to run repeatedly:

}
