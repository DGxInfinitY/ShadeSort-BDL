/*  Project Shade Sort V0.3 Pre-Alpha Code
//  Developers:
//    Donovan Goodwin (DGxInfinitY): ddg2goodwin@gmail.com
//
//  v0.3
//    The code now compiles and is sort of functional but it has a lot
//    of problems, I need to spend a lot of time getting this to work.
//
//  v0.1:
//    Basic Code, as of this release the code is not fully functional.
//
*/

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <EDB.h>
#include <EEPROM.h>
//#include <PS2Keyboard.h>

#define TABLE_SIZE 512

int led = 13;

//Data and Clock Pins for the Keyboard
const int DataPin = 15;
const int IRQpin =  14;
//PS2Keyboard into keyboard
//PS2Keyboard keyboard;

//record info, this is how the record gets recorded.
struct newPuck {
  int id;
  const char* shade;
  int size;
  const char* puckname;
}
newPuck;

//defines the way to read and record the record in eeprom.
void writer(unsigned long address, byte data) {
  EEPROM.write(address, data);
}
byte reader(unsigned long address)  {
  return EEPROM.read(address);
}

//Create an EDB object with the appropriate write and read handlers
EDB db(&writer, &reader);




//Startup Stuff Begins
void setup()  {
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);

    Serial.begin(115200);
    Serial.print("Starting system...");
    //Create table at with starting address 0 if there isn't one(first run procedure)
    //if (firstrun() == 1)  {
      //Serial.println("");
      //Serial.print("This is the First Run of the device, creating new table...");
      //db.create(0, TABLE_SIZE, (unsigned int)sizeof(newPuck));
      //Serial.println("DONE");
    //}
    db.create(0, TABLE_SIZE, (unsigned int)sizeof(newPuck));
    Serial.print("Finishing System Startup...");
    delay(5);
    Serial.println("DONE");
    //Counts the total ammt. of Records
    Serial.print("Counting Records...");
    countRecords();
    Serial.println("DONE");
    //Keyboard Load info
    //keyboard.begin(DataPin, IRQpin);
    //Serial.print("Keyboard Ready...");
    //Serial.println("DONE");
    //Create our First Record with the Shade "A1" Size of "!2mm" and the Name of "First"
    createRecord(1, "A1", 12, "First");
    countRecords();
    printAll();
}

void loop() {
  //Put runtime stuff here
}



//Custom Functions


void printAll()
{
  for (int recno = 1; recno <= db.count(); recno++)
  {
    EDB_Status result = db.readRec(recno, EDB_REC newPuck);
    if (result == EDB_OK)
    {
      //Serial.print("Recno: ");
      //Serial.print(recno);
      Serial.print(" ID: ");
      Serial.print(newPuck.id);
      Serial.print(" Shade: ");
      Serial.println(newPuck.shade);
      Serial.print(" Size: ");
      Serial.println(newPuck.size);
      Serial.print(" Puckname: ");
      Serial.println(newPuck.puckname);
    }
    else printError(result);
  }
}



float firstrun() {
  if(EEPROM.read(0) != 1) {
    //delay(500);
    //Serial.println("");
    //Serial.print("DEBUG: EEPROM.read != 1 before: ");
    //Serial.println(EEPROM.read(0));
    EEPROM.write(0, 0);
    //delay(500);
    //Serial.print("DEBUG: EEPROM.write(0,0) after: ");
    //Serial.println(EEPROM.read(0));
    return 1;
  }
  if(EEPROM.read(0) != 0) {
    Serial.println(EEPROM.read(0));
    EEPROM.write(0, 1);
    Serial.println(EEPROM.read(0));
    return 0;
  }
}

void printError(EDB_Status err) {
  Serial.print("ERROR: ");
  switch (err)
  {
    case EDB_OUT_OF_RANGE:
      Serial.println("Recno out of range");
      break;
    case EDB_TABLE_FULL:
      Serial.println("Table full");
      break;
    case EDB_OK:
    default:
      Serial.println("OK");
      break;
  }
}

//Used to create the new records with the variable, shade, size, and puckname.
void createRecord(int id, const char* shade, int size, const char* puckname) {
  Serial.printf("Creating Record...");
  newPuck.id = id;
  newPuck.shade = shade;
  newPuck.size = size;
  newPuck.puckname = puckname;
  EDB_Status result = db.appendRec(EDB_REC newPuck);
  if (result != EDB_OK) printError(result);
  Serial.println("DONE");
}

//Delete one record by specifiing the recordnumber "recno"
void deleteOneRecord(int recno) {
  Serial.print("Deleting Record No: ");
  Serial.println(recno);
  db.deleteRec(recno);
}
//Counts all the records that are in the table.
void countRecords() {
  Serial.print("Record Count = ");
  Serial.println(db.count());
}
