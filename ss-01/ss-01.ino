/*  Project Shade Sort V0.1 Pre-Alpha Code
//  Developers:
//    Donovan Goodwin (DGxInfinitY): ddg2goodwin@gmail.com
//
//  v0.1:
//    Basic Code, as of this release the code is not fully functional.
//
//
//
*/

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <EDB.h>
#include <EEPROM.h>
#include <PS2Keyboard.h>

#define TABLE_SIZE 512

//
const int DataPin = 15;
const int IRQpin =  14;


//record info, this is how the record gets recorded.
struct newPuck {
  int shade;
  int size;
  int puckname;
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
void createRecord(int shade, int size, char* puckname) {
  Serial.printf("Creating Record...");
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
  Serial.print("Record Count: ");
  Serial.println(db.count());
}

//Startup Stuff Begins
void setup()
{
    Serial.begin(9600);
    Serial.print("Starting system...");
    // create table at with starting address 0
    db.create(0, TABLE_SIZE, (unsigned int)sizeof(newPuck));
    Serial.println("DONE");
    //Counts the total ammt. of Records
    Serial.print("Count Records...");
    countRecords();
    Serial.println("DONE");
    //Keyboard Load
    keyboard.begin(DataPin, IRQpin);
    Serial.print("Keyboard Ready...");
    Serial.println("DONE")
    //Create our First Record with the Shade "A1" Size of "!2mm" and the Name of "First"
    createRecord(A1, 12, First);
}

void loop() {
    //Keyboard input search(Right Now it's nonfunctional within the code.)
    if (keyboard.available()) {
    // read the next key
    char c = keyboard.read();
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } else {

      // otherwise, just print all normal characters
      Serial.print(c);
    }
  }
}
