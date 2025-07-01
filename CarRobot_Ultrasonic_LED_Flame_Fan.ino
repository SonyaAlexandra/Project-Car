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
    int distance = duration * 0.034 / 2;
    return distance;
}

void moveForward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void moveBackward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
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
}

void loop() {
    int distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    if (distance > SAFE_DISTANCE) {
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
