//Pins for QTI connections on board
#define lineSensor1 47
#define lineSensor3 51
#define lineSensor5 52

//to control the servos
#include <Servo.h>

//initialize the output servos and the qti input pins
Servo leftServ;
Servo rightServ;
int qti1 = 0;
int qti3 = 0;
int qti5 = 0;

//setup, attaches servos to pins and begins the serial monitor to help with debugging
void setup()
{
    Serial.begin(9600); //start the serial monitor so we can view the output
    leftServ.attach(11);
    rightServ.attach(12);
}

//The actual executable part of
void loop()
{

    doWork(getStatus());
}

//functions below

//Defines funtion 'RCTime' to read value from QTI sensor
void getQTI()
{
    qti1 = RCTime(lineSensor1); //Calls funtion 'RCTime' Request reading from QTI sensor at pin 'linesensor1' saves value in variable 'qti'
    //delay(200);
    qti3 = RCTime(lineSensor3); //Calls funtion 'RCTime' Request reading from QTI sensor at pin 'linesensor1' saves value in variable 'qti'
    //delay(200);
    qti5 = RCTime(lineSensor5); //Calls funtion 'RCTime' Request reading from QTI sensor at pin 'linesensor1' saves value in variable 'qti'
    //delay(200);
    Serial.println(qti1);
    Serial.println(qti3);
    Serial.println(qti5); //Print QTI reading to serial monitor
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
 * @brief set up the cases for detecting lines.
 * 
 */

//case 3 -- not
void drive()
{
    Serial.println("case: 3");
    leftServ.writeMicroseconds(1600);  //left above 1500 goes forward
    rightServ.writeMicroseconds(1400); //right below 1500 goes forward
}
//case 2
void turnLeft()
{
    Serial.println("case: 2");
    leftServ.writeMicroseconds(1500);
    rightServ.writeMicroseconds(1400);
}
//case 1
void sharpLeft()
{
    Serial.println("case: 1");
    rightServ.writeMicroseconds(1300);
    leftServ.writeMicroseconds(1300);
}
//case 4
void turnRight()
{
    Serial.println("case: 4");
    rightServ.writeMicroseconds(1500);
    leftServ.writeMicroseconds(1600);
}
//case 5
void sharpRight()
{
    Serial.println("case: 5");
    leftServ.writeMicroseconds(1700);
    rightServ.writeMicroseconds(1700);
}
//case 9
void stop()
{
    Serial.println("case: 9");
    leftServ.writeMicroseconds(1500);
    rightServ.writeMicroseconds(1500);
}
//case 0
void reverse()
{
    Serial.println("case: 0");
    //leftServ.writeMicroseconds(1400);
    //rightServ.writeMicroseconds(1600);
}

//gets the status of the qti sensors
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

        return 3; //forward
    }
    else if ((qti1 > 100) && (qti5 < 100))
    {
        return 1; //sharp left
    }
    else if ((qti1 < 100) && (qti5 > 100))
    {
        return 5; //sharp right
    }
    else if ((qti3 < 100) && (qti1 > 100) && (qti5 < 100))
    {
        return 1; //slight left
    }
    else if ((qti3 < 100) && (qti1 < 100) && (qti5 > 100))
    {
        return 5; //slight right 
    }
    else if ((qti3 > 100) && (qti1 > 100) && (qti5 > 100))
    {
        return 9; //stop all three hit a line
    }
    else if ((qti3 < 100) && (qti1 < 100) && (qti5 < 100))
    {
        return 0; //reverse no lines detected
    }
}

//code that takes the status and applies a case for it to run
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
        delay(3000);
        drive();
        delay(200);
        return 9;

    default:
        stop();
        break;
    }
}
