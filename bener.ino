const int sensorPin = 2; // Pin digital yang terhubung ke tombol
const int ledGreenPin = 9; // Pin digital yang terhubung ke LED hijau
const int ledYellowPin = 10; // Pin LED kuning
const int ledRedPin = 11; // Pin LED merah
const int buzzerPin = 8;

bool sensorState = false;
bool lastSensorState = false;
bool isSensorPressed = false;
unsigned long previousTime = 0;
const unsigned long yellowLedDuration = 5000; // Durasi LED kuning menyala (10 detik)
const unsigned long redLedDuration = 15000; // Durasi LED merah menyala (15 detik)

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledGreenPin, LOW); 
  digitalWrite(ledYellowPin, LOW);
  digitalWrite(ledRedPin, LOW);
}

void loop() {
  // Baca status tombol
  sensorState = digitalRead(sensorPin);

  if (sensorState == LOW && lastSensorState == HIGH) {
    isSensorPressed = true; // Set status tombol sudah ditekan
    digitalWrite(ledGreenPin, LOW); // Matikan LED hijau saat tombol sudah ditekan
    digitalWrite(ledYellowPin, HIGH); // Nyalakan LED kuning saat tombol ditekan
    Serial.println("kuning menyala"); // Kirim sinyal ke Python
    previousTime = millis(); // Catat waktu saat tombol ditekan
  }

  if (isSensorPressed) {
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= yellowLedDuration && currentTime - previousTime < redLedDuration) {
      digitalWrite(ledYellowPin, LOW); // Matikan LED kuning setelah 5 detik
      digitalWrite(ledRedPin, HIGH); // Nyalakan LED merah setelah 5 detik
      digitalWrite(buzzerPin, HIGH); // Matikan buzzer setelah 5 detik
      Serial.println("merah menyala");
    } else if (currentTime - previousTime >= redLedDuration) {
      digitalWrite(ledRedPin, LOW); // Matikan LED merah setelah 15 detik
      digitalWrite(buzzerPin, LOW); // Matikan buzzer setelah 5 detik
      digitalWrite(ledGreenPin, HIGH); // Kembali ke LED hijau menyala setelah 15 detik

      isSensorPressed = false; // Kembali ke kondisi standby setelah 15 detik
    }
  }

  lastSensorState = sensorState;
}