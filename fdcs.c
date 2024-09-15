#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx;
Servo myservo;

#define Left 8
#define Right 9
#define Forward 10
#define LM1 2
#define LM2 3
#define RM1 4
#define RM2 5
#define pump 6

void setup() {
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);

  mlx.begin();
  myservo.attach(11);
  myservo.write(90);
}

void sweepServo() {
  for (int pos = 50; pos <= 130; pos += 1) {
    myservo.write(pos);
    delay(10);
  }
  for (int pos = 130; pos >= 50; pos -= 1) {
    myservo.write(pos);
    delay(10);
  }
}

void put_off_fire() {
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  digitalWrite(pump, HIGH);
  sweepServo();  // Sweep the servo while extinguishing the fire
  digitalWrite(pump, LOW);
}

void loop() {
  double temp = mlx.readObjectTempC();

  if (temp > 37.0) {
    put_off_fire();
  } else {
    myservo.write(90);  // Center the servo

    int leftSensor = digitalRead(Left);
    int rightSensor = digitalRead(Right);
    int forwardSensor = digitalRead(Forward);

    if (leftSensor && rightSensor && forwardSensor) {
    
      digitalWrite(LM1, HIGH);
      digitalWrite(LM2, HIGH);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, HIGH);
    } else {
      // Stop and adjust movement based on sensor inputs
      digitalWrite(LM1, HIGH);
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);

      if (leftSensor == LOW) {
        // Turn left
        digitalWrite(LM1, HIGH);
        digitalWrite(LM2, LOW);
        digitalWrite(RM1, HIGH);
        digitalWrite(RM2, HIGH);
      }

      if (rightSensor == LOW) {
        // Turn right
        digitalWrite(LM1, HIGH);
        digitalWrite(LM2, HIGH);
        digitalWrite(RM1, HIGH);
        digitalWrite(RM2, LOW);
      }
    }
  }
}