
// TODO
// - Remove the delays that prevent the line following //done
// - finish the receiving code
// - change the code so that when we are stopped on the hash it only sends once  //done

// Pins for QTI connections on board
#define lineSensor1 47
#define lineSensor3 51
#define lineSensor5 52

// to control the servos
#include <Servo.h>

// colorsensor stuff
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// initialize the output servos and the qti input pins
Servo leftServ;
Servo rightServ;
int qti1 = 0;
int qti3 = 0;
int qti5 = 0;

////////////////////////

int val = 507; // Sets the cut off value that detects whether or not a magnet is present
int hall = A9;
int greenLED = 6;
int redLED = 7;

// Set the pins for the on board RGB LED
int blue = 44;
int red = 45;
int green = 46;

// bool setup
bool magDetected = false;
bool contDetected = false;
bool received = false;
// this bool is to detect if we've completed a circle.
bool isDone = false;
// bool to check if we've sent
bool isSent = false;
bool printed = false;

////////
// color sensor stuff
////////

int b = 44;
int r = 45;
int g = 46;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/// Communication stuff///////////////////////////////////////////////////
#define Rx 17 // DOUT to pin 17
#define Tx 16 // DIN to pin 16

int btn = 4; // sets the pin for out test button
int led = 3; // sets the pin for output led

int count = 0;
int num = 0;

//////////////////////////////////////////////////////////////////////////
//////LCD Stuff////
//////////////////////////////////////////////////////////////////////////
const int TxPin = 14;

#include <SoftwareSerial.h>
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

const String challenges[] = {"Nitrogen", "Health Info", "Medicine", "VR", "Discovery", "Brain", "Learning"};

int startTime = 0;

// functions below

// Defines funtion 'RCTime' to read value from QTI sensor
void getQTI()
{
    qti1 = RCTime(lineSensor1); // Calls funtion 'RCTime' Request reading from QTI sensor at pin 'linesensor1' saves value in variable 'qti'
    // delay(200);
    qti3 = RCTime(lineSensor3); // Calls funtion 'RCTime' Request reading from QTI sensor at pin 'linesensor1' saves value in variable 'qti'
    // delay(200);
    qti5 = RCTime(lineSensor5); // Calls funtion 'RCTime' Request reading from QTI sensor at pin 'linesensor1' saves value in variable 'qti'
    // delay(200);
    Serial.println(qti1);
    Serial.println(qti3);
    Serial.println(qti5); // Print QTI reading to serial monitor
}

// gets the values for the qti that we will use to determine whether or not we're on the line
long RCTime(int sensorIn)
{

    long duration = 0;
    pinMode(sensorIn, OUTPUT);    // Sets pin as OUTPUT
    digitalWrite(sensorIn, HIGH); // Pin HIGH
    delay(1);                     // Waits for 1 millisecond
    pinMode(sensorIn, INPUT);     // Sets pin as INPUT
    digitalWrite(sensorIn, LOW);  // Pin LOW

    while (digitalRead(sensorIn))
    { // Waits for the pin to go LOW
        duration++;
    }
    return duration; // Returns the duration of the pulse
}
/**
   @brief set up the cases for detecting lines.

*/

// case 3 -- not
void drive()
{
    Serial.println("case: 3");
    leftServ.writeMicroseconds(1600);  // left above 1500 goes forward
    rightServ.writeMicroseconds(1400); // right below 1500 goes forward
}
// case 2
void turnLeft()
{
    Serial.println("case: 2");
    leftServ.writeMicroseconds(1500);
    rightServ.writeMicroseconds(1400);
}
// case 1
void sharpLeft()
{
    Serial.println("case: 1");
    rightServ.writeMicroseconds(1300);
    leftServ.writeMicroseconds(1300);
}
// case 4
void turnRight()
{
    Serial.println("case: 4");
    rightServ.writeMicroseconds(1500);
    leftServ.writeMicroseconds(1600);
}
// case 5
void sharpRight()
{
    Serial.println("case: 5");
    leftServ.writeMicroseconds(1700);
    rightServ.writeMicroseconds(1700);
}
// case 9
void stop()
{
    Serial.println("case: 9");
    leftServ.writeMicroseconds(1500);
    rightServ.writeMicroseconds(1500);
    isDone = true;
}
// case 0
void reverse()
{
    Serial.println("case: 0");
    // leftServ.writeMicroseconds(1400);
    // rightServ.writeMicroseconds(1600);
}

// gets the status of the qti sensors
int getStatus()
{
    getQTI(); // gets the qti values

    // if ((qti1 < 100) && (qti5 < 100))
    // {

    //     return 3; //forward
    // }
    // else if ((qti3 > 100) && (qti1 > 100) && (qti5 < 100))
    // {
    //     return 1; //sharp left
    // }
    // else if ((qti3 > 100) && (qti1 < 100) && (qti5 > 100))
    // {
    //     return 5; //sharp right
    // }
    // else if ((qti3 < 100) && (qti1 > 100) && (qti5 < 100))
    // {
    //     return 2; //slight left
    // }
    // else if ((qti3 < 100) && (qti1 < 100) && (qti5 > 100))
    // {
    //     return 4; //slight right
    // }
    // else if ((qti3 > 100) && (qti1 > 100) && (qti5 > 100))
    // {
    //     return 9; //stop all three hit a line
    // }
    // else if ((qti3 < 100) && (qti1 < 100) && (qti5 < 100))
    // {
    //     return 0; //reverse no lines detected
    // }

    if ((qti1 < 100) && (qti5 < 100))
    {

        return 3; // forward
    }
    else if ((qti1 > 100) && (qti5 < 100))
    {
        return 1; // sharp left
    }
    else if ((qti1 < 100) && (qti5 > 100))
    {
        return 5; // sharp right
    }
    else if ((qti3 < 100) && (qti1 > 100) && (qti5 < 100))
    {
        return 1; // slight left
    }
    else if ((qti3 < 100) && (qti1 < 100) && (qti5 > 100))
    {
        return 5; // slight right
    }
    else if ((qti3 > 100) && (qti1 > 100) && (qti5 > 100))
    {

        return 9; // stop all three hit a line
    }
    else if ((qti3 < 100) && (qti1 < 100) && (qti5 < 100))
    {
        return 0; // reverse no lines detected
    }
}

// code that takes the status and applies a case for it to run
int doWork(int x)
{
    switch (x)
    {
    case 0:
        reverse();

        return 0;

    case 1:
        sharpLeft();
        return 1;

    case 2:
        turnLeft();
        return 2;

    case 3:
        drive();
        return 3;

    case 4:
        turnRight();
        return 4;

    case 5:
        sharpRight();
        return 5;

    case 9:
        stop();
        // delay(3000);
        // drive();
        // delay(200);
        return 9;

    default:
        stop();
        break;
    }
}
/////////////
void colorGray()
{
    analogWrite(b, 127);
    analogWrite(r, 127);
    analogWrite(g, 127);
}

void colorGreen()
{
    analogWrite(b, 255);
    analogWrite(r, 255);
    analogWrite(g, 0);
}

void colorBlue()
{
    analogWrite(b, 0);
    analogWrite(r, 255);
    analogWrite(g, 255);
}

void colorYellow()
{
    analogWrite(b, 255);
    analogWrite(r, 0);
    analogWrite(g, 0);
}

void colorRed()
{
    analogWrite(b, 255);
    analogWrite(r, 0);
    analogWrite(g, 255);
}

void colorPurple()
{
    analogWrite(b, 127);
    analogWrite(r, 127);
    analogWrite(g, 255);
}

// color sensor function
void getCont(int red, int green, int blue)
{

    if ((red > 350 && red < 550) && (green > 400 && green < 600) && (blue > 300 && blue < 500))
    {
        Serial.print("gray");
        colorGray();
        contDetected = true;
    }
    // gray
    else if ((red > 240 && red < 440) && (green > 430 && green < 630) && (blue > 120 && blue < 320))
    {
        Serial.print("green");
        colorGreen();
        contDetected = true;
    }
    // green
    else if ((red > 15 && red < 215) && (green > 63 && green < 263) && (blue > 133 && blue < 333))
    {
        Serial.print("blue");
        colorBlue();
        contDetected = true;
    }
    else if ((red > 611 && red < 811) && (green > 620 && green < 820) && (blue > 150 && blue < 350))
    {
        Serial.print("yellow");
        colorYellow();
        contDetected = true;
    }
    // yellow
    else if ((red > 340 && red < 540) && (green > 13 && green < 213) && (blue > 0 && blue < 200))
    {
        Serial.print("red");
        colorRed();
        contDetected = true;
    }
    // red
    else if ((red > 240 && red < 440) && (green > 90 && green < 290) && (blue > 150 && blue < 350))
    {
        Serial.print("purple");
        colorPurple();
        contDetected = true;
    }
    else
    {
        Serial.println("no continent");
    }
}
void receive()
{
    /**

       int[][][] =
       @brief this is the structure
       team number : 1, 2, 3, 4, 5, 6
       score:        0, 1, 0, 1, 0, 1
       recieved?:    1, 1, 1, 0, 0, 0

    */

    if (Serial2.available())
    {                                   // Is XBee available?
        char incoming = Serial2.read(); // Read character from XBee
        Serial.println(incoming);       // send to Serial Monitor
        // conditional on if the received is what we want to receive.

        digitalWrite(led, HIGH); // turn on LED
        delay(1);                // delay to be able to see the change in LED

        if (incoming <= 'E' && incoming >= 'A') //
        {
            count++;
            num++;
            // if (num == 4)
            // {
            //     Serial.print("Sum = ");
            //     Serial.println(count);
            // }
        }
        else if (incoming <= 'e' && incoming >= 'a')
        {
            num++;
        }
    }
    // delay(1);
    // digitalWrite(led, LOW);
    if (num == 4 && !printed || millis() - startTime > 25000)
    {
        if (magDetected)
            count++;
        Serial.println("something is working!!!");
        display(count);
        exit(0);
    }
}

void transmit(char outgoing)
{
    // char outgoing = 'd';
    // if (magDetected)
    // {
    //     char outgoing = 'D'; // Read character, this will be the default
    // }
    // else
    // {
    //     char outgoing = 'd';
    // }
    if (!isSent)
    {
        isSent = true;
        Serial2.print(outgoing); // send out character to XBee
        digitalWrite(led, HIGH); // turn on LED
        Serial.println("ran");

        // for (int x = 0; x < 2; x++)
        // {
        //     digitalWrite(led, HIGH); // turn on LED
        //     delay(1);                // delay to be able to see the change in LED
        //     digitalWrite(led, LOW);
        //     delay(1); // delay to be able to see the change in LED
        // }
    }
}

// function to use the LCD to display stuff
void display(int sum)
{
    Serial.print("yay!!!!");
    mySerial.write(12);
    mySerial.write(17);
    mySerial.print(challenges[count]);
    mySerial.write(13);
    mySerial.print("Sum = ");
    mySerial.print(sum);
    printed = true;
}

// setup, attaches servos to pins and begins the serial monitor to help with debugging
void setup()
{
    Serial.begin(9600);  // start the serial monitor so we can view the output
    Serial2.begin(9600); // type a char , then hit delay (500);
    leftServ.attach(11);
    rightServ.attach(12);

    pinMode(hall, INPUT);

    // led setup
    pinMode(greenLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);

    // color

    // comm set ups
    pinMode(btn, INPUT);  // setup the pin for the button as input
    pinMode(led, OUTPUT); // setup the pin for the led as output

    if (tcs.begin())
    {
        // Serial.println("Found sensor");
    }

    // Set LCD up
    pinMode(TxPin, OUTPUT);
    digitalWrite(TxPin, HIGH);

    mySerial.begin(9600);
    mySerial.write(12);
    mySerial.write(17);
}

// The actual executable part of
void loop()
{
    // qti line following
    if (!isDone)
    {
        doWork(getStatus());

        /////////////////////

        // detect magnet
        Serial.println(analogRead(hall));

        if (val - analogRead(hall) > 5)
        {
            Serial.println("Magnet Detected");
            // colorGreen();

            // digitalWrite(greenLED, HIGH);
            // digitalWrite(redLED, LOW);
            if (!magDetected)
            {
                // transmit('D');
                magDetected = true;
            }
        }
        else if (!magDetected)
        {
            Serial.println("No Magnet");
            // colorRed();
            digitalWrite(redLED, HIGH);
        }

        // get continent
        if (!contDetected)
        {
            uint16_t clear, red, green, blue;

            delay(60);

            tcs.getRawData(&red, &green, &blue, &clear);
            getCont(red, green, blue);
        }
        // check for the slowest teams signal
    }
    else if (!isSent)
    {
        startTime = millis();
        if (magDetected)
        {
            transmit('D');
            digitalWrite(greenLED, HIGH);
            digitalWrite(redLED, LOW);
            
        }
        else
        {
            transmit('d');
        }
    }

    if (num < 5)
    {
        receive();
    }
    Serial.print(isDone);

    //    mySerial.print
}
