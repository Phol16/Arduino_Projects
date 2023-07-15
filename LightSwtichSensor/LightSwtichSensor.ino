#define UDS_pin 9
#define Switch_pin 8

bool activator = false;
bool active = false;

byte validator = 0;

float TheDistance = 49.99;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(Switch_pin, OUTPUT);
}

void loop() {
  //function that reads UDS inputs
  int UDS_Reader = UltrasonicDistanceSensor();

  //conditional statment if reader is within a distance
  if (UDS_Reader <= TheDistance) {
    //conditional statment to validate distance
    if (validator >= 2) {
      activator = true;
    }

    //increment validation if sensor detects object within a certain distance
    validator += 1;
    delay(250);  // delay for 250ms
  } else {
    //if distance is far make validation number to 0
    validator = 0;
  }

//while loop if actovator is true do the codes inside
  while (activator) {
    //function that reads UDS inputs
    UDS_Reader = UltrasonicDistanceSensor();

    //make switch open
    digitalWrite(Switch_pin, HIGH);

    //somesort of validation also to check if object moved out of the given distance
    if (UDS_Reader >= TheDistance) {
      active = true;
    }

    //conditional statment if object moved out of the given distance again then will close the loop
    if (active && UDS_Reader <= TheDistance) {
      activator = false;
    }
  }

  //while loop when object is closed again and the first loop ended
  while (active) {
    //function that reads UDS inputs
    UDS_Reader = UltrasonicDistanceSensor();
    
    //will make the switch close
    digitalWrite(Switch_pin, LOW);

    //conditional statment to check if object moved out of the given distance
    if (UDS_Reader >= TheDistance) {
      active = false;
    }
  }
}


int UltrasonicDistanceSensor() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(UDS_pin, OUTPUT);
  digitalWrite(UDS_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(UDS_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(UDS_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(UDS_pin, INPUT);
  duration = pulseIn(UDS_pin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);

  Serial.print(inches);
  Serial.println("in, ");

  return inches;
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}