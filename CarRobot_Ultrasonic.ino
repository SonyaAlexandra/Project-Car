#include <Servo.h>

#define ENA 9
#define IN1 8
#define IN2 7
#define IN3 6
#define IN4 4
#define ENB 10

#define TRIG 3
#define ECHO 2
#define SERVO_PIN 5

#define FAN_INA A1
#define FAN_INB A2
#define FLAME_SENSOR A0

#define LED_RED 11
#define LED_YELLOW 12

Servo servo;
#define SAFE_DISTANCE 20  

void setup() {
    Serial.begin(9600);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(FAN_INA, OUTPUT);
    pinMode(FAN_INB, OUTPUT);
    pinMode(FLAME_SENSOR, INPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    servo.attach(SERVO_PIN);
    servo.write(90);
}

int getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    long duration = pulseIn(ECHO, HIGH);
    return duration * 0.034 / 2;
}

void turnOnFan() {
    digitalWrite(FAN_INA, HIGH);
    digitalWrite(FAN_INB, LOW);
}

void turnOffFan() {
    digitalWrite(FAN_INA, LOW);
    digitalWrite(FAN_INB, LOW);
}

void turnOnRedLED() {
    digitalWrite(LED_RED, HIGH);
}

void turnOffRedLED() {
    digitalWrite(LED_RED, LOW);
}

void turnOnYellowLED() {
    digitalWrite(LED_YELLOW, HIGH);
}

void turnOffYellowLED() {
    digitalWrite(LED_YELLOW, LOW);
}

bool checkFlameSensor() {
    if (digitalRead(FLAME_SENSOR) == LOW) {
        turnOnRedLED();
        Serial.println("🔥 Flame detected! Stopping and keeping fan ON...");
        return true;
    } else {
        turnOffRedLED();
        return false;
    }
}

void moveForward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    turnOffYellowLED();
    turnOnFan();
}

void moveBackward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    turnOffYellowLED();
    turnOnFan();
}

void turnLeft() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);
}

void turnRight() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(500);
}

void stopCar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    turnOnYellowLED();
    turnOffFan();
}

void loop() {
    int distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    bool flameDetected = checkFlameSensor();

    if (flameDetected) {
        stopCar();
        turnOnFan();
        Serial.println("🚨 Fire detected! Stopping and keeping fan ON.");
    } else if (distance > SAFE_DISTANCE) {
        moveForward();
        Serial.println("Moving Backward...");
    } else {
        stopCar();
        Serial.println("Obstacle detected! Stopping...");
        servo.write(0);
        delay(500);
        int leftDistance = getDistance();
        servo.write(180);
        delay(500);
        int rightDistance = getDistance();
        servo.write(90);
        if (leftDistance > rightDistance) {
            turnLeft();
            Serial.println("Turning Right...");
        } else {
            turnRight();
            Serial.println("Turning Left...");
        }
        moveBackward();
    }
    delay(500);
}
