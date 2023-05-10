/* 
  Simple code for Keyestudio Arduino Tank Robot V3.
  It includes functions for controlling the robot's movements, 
  avoid front collision and display images on the dot matrix.

  Base code at (https://wiki.keyestudio.com/KS0526_Keyestudio_Mini_Tank_Robot_V3)
  This version was created with the help of ChatGPT 4.0 (https://openai.com/product/gpt-4)

  Cheers!
  Fernando Cido (https://github.com/fernandocido)
  
*/

//Motors
#define mlCtrl 4  //define the direction control pin of the left motor as 4
#define mlPwm 5   //define the PWM control pin of the left motor as 5
#define mrCtrl 2  //define the direction control pin of the right motor as 2
#define mrPwm 6   //define the PWM control pin of the right motor as 6

//Display
#define DIN A4
#define CLK A5

//Ultrasonic sensor
#define Trig 12
#define Echo 13

//Bluetooth
char bluetoothValue; //used to save the Bluetooth value 

//Ultrasonic sensor (distance reading)
long readDistance() {
  long duration, distance;
  digitalWrite(Trig, LOW);   // Ensure the trigger pin is low
  delayMicroseconds(2);      // Wait for 2 microseconds
  digitalWrite(Trig, HIGH);  // Set the trigger pin to high
  delayMicroseconds(10);     // Wait for 10 microseconds
  digitalWrite(Trig, LOW);   // Set the trigger pin back to low

  duration = pulseIn(Echo, HIGH); // Measure the duration of the echo pulse
  distance = duration * 0.0344 / 2; // Calculate the distance in cm

  return distance;
}

//Display patterns
//Movement patterns
unsigned char stop[] = {0x2E, 0x2A, 0x3A, 0x00, 0x02, 0x3E, 0x02, 0x00, 0x3E, 0x22, 0x3E, 0x00, 0x3E, 0x0A, 0x0E, 0x00};
unsigned char front[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x12, 0x09, 0x12, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char back[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x48, 0x90, 0x48, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char left[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x10, 0x44, 0x28, 0x10, 0x44, 0x28, 0x10, 0x00};
unsigned char right[] = {0x00, 0x10, 0x28, 0x44, 0x10, 0x28, 0x44, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//"Face" patterns
unsigned char smile[] = {0x00, 0x00, 0x1c, 0x02, 0x02, 0x02, 0x5c, 0x40, 0x40, 0x5c, 0x02, 0x02, 0x02, 0x1c, 0x00, 0x00};
unsigned char disgust[] = {0x00, 0x00, 0x02, 0x02, 0x02, 0x12, 0x08, 0x04, 0x08, 0x12, 0x22, 0x02, 0x02, 0x00, 0x00, 0x00};
unsigned char happy[] = {0x02, 0x02, 0x02, 0x02, 0x08, 0x18, 0x28, 0x48, 0x28, 0x18, 0x08, 0x02, 0x02, 0x02, 0x02, 0x00};
unsigned char squint[] = {0x00, 0x00, 0x00, 0x41, 0x22, 0x14, 0x48, 0x40, 0x40, 0x48, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00};
unsigned char despise[] = {0x00, 0x00, 0x06, 0x04, 0x04, 0x04, 0x24, 0x20, 0x20, 0x26, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00};
unsigned char heart[] = {0x00, 0x00, 0x0C, 0x1E, 0x3F, 0x7F, 0xFE, 0xFC, 0xFE, 0x7F, 0x3F, 0x1E, 0x0C, 0x00, 0x00, 0x00};
//Clear Pattern
unsigned char clear[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//C.O.R.A. [(C)oordenador (O)timizado em (R)obô (A)rduino] pattern
unsigned char cora[] = {0x00, 0x7e, 0x42, 0x00, 0x3c, 0x42, 0x3c, 0x00, 0x7e, 0x12, 0x6c, 0x00, 0x7c, 0x12, 0x7c, 0x00};

//Display functions
void matrixDisplay(unsigned char matrixValue[]) {
  displayStart();  //start transmitting data
  displaySend(0xc0);  //select an address
  
  //16 characters 
  for (int i = 0; i < 16; i++) {
    displaySend(matrixValue[i]); //data to transmit values
  }
  displayEnd();   //end the data transmission 
  displayStart();
  displaySend(0x8A);  //show control and select pulse width 4/16
  displayEnd();
}

//Display start
void displayStart() {
  digitalWrite(DIN, HIGH);
  digitalWrite(CLK, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN, LOW);
  delayMicroseconds(3);
  digitalWrite(CLK, LOW);
}

//Display data transmission
void displaySend(unsigned char sendData) {
  //each character has 8 digits, which is detected one by one
  for (byte mask = 0x01; mask != 0; mask <<= 1) {
    //set high or low light levels of each bit (0 or 1)
    if (sendData & mask) {
      digitalWrite(DIN, HIGH);
    } else {
      digitalWrite(DIN, LOW);
    }
    delayMicroseconds(3);
    digitalWrite(CLK, HIGH); //pull up the clock to end the data transmission
    delayMicroseconds(3);
    digitalWrite(CLK, LOW); //pull down the clock to change DIN signals
  }
}

//End of data transmission
void displayEnd() {
  digitalWrite(CLK, LOW);
  digitalWrite(DIN, LOW);
  delayMicroseconds(3);
  digitalWrite(CLK, HIGH);
  delayMicroseconds(3);
  digitalWrite(DIN, HIGH);
  delayMicroseconds(3);
}

//Movement
void carFront() {
  digitalWrite(mrCtrl, LOW);
  analogWrite(mrPwm, 235);
  digitalWrite(mlCtrl, LOW);
  analogWrite(mlPwm, 235);
  matrixDisplay(front);  //show the 'front' pattern
}

void carBack() {
  digitalWrite(mrCtrl, HIGH);
  analogWrite(mrPwm, 235);
  digitalWrite(mlCtrl, HIGH);
  analogWrite(mlPwm, 235);
  matrixDisplay(back);  //show the 'back' pattern
}

void carLeft() {
  digitalWrite(mrCtrl, LOW);
  analogWrite(mrPwm, 235);
  digitalWrite(mlCtrl, HIGH);
  analogWrite(mlPwm, 235);
  matrixDisplay(left);  //show the 'left' pattern
}

void carRight() {
  digitalWrite(mrCtrl, HIGH);
  analogWrite(mrPwm, 235);
  digitalWrite(mlCtrl, LOW);
  analogWrite(mlPwm, 235);
  matrixDisplay(right);  //show the 'right' pattern
}

void carStop() {
  digitalWrite(mrCtrl, LOW);
  analogWrite(mrPwm, 0);
  digitalWrite(mlCtrl, LOW);
  analogWrite(mlPwm, 0);
  matrixDisplay(cora);  //show CORA =]
}

//Object avoidance
void handleObstacleAvoidance() {
  long distance = readDistance();
  // Change this value to adjust the sensitivity
  if (distance < 12) {
    carStop();
    delay(1000);
    matrixDisplay(despise);
    delay(1000);
    carBack();
    delay(700);
    carStop();
    delay(1000);
  }
}

void setup() {
  //LED Display
  pinMode(DIN, OUTPUT); // Set DIN as an output pin
  pinMode(CLK, OUTPUT); // Set CLK as an output pin

  // Servo motor
  myservo.attach(servoPin);
  myservo.write(servoPosition);

  //Ultrasonic sensor
  pinMode(Trig, OUTPUT); // Set Trig as an output pin
  pinMode(Echo, INPUT);  // Set Echo as an input pin

  // Serial Communication
  Serial.begin(9600); // Set up serial communication at 9600 baud rate
}

void loop() {
  if (Serial.available()) {
    bluetoothValue = Serial.read();
    Serial.println(bluetoothValue);
    switch (bluetoothValue) {
      case 'F': // Bluetooth command to move 'FRONT'
        carFront();
        break;
      case 'B': // Bluetooth command to move 'BACK'
        carBack();
        break;
      case 'L': // Bluetooth command to move 'LEFT'
        carLeft();
        break;
      case 'R': // Bluetooth command to move 'RIGHT'
        carRight();
        break;
      case 'S': // Bluetooth command to 'STOP'
        carStop();
        break;
      default: break;
    }
  } else {
    handleObstacleAvoidance(); // Call the obstacle avoidance function when there's no pending Bluetooth command
  }
}