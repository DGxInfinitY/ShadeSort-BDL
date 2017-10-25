/*
 * interpreter.ino: Simple Arduino command line interpreter.
 *
 * This is intended solely as a template for building richer,
 * application-specific interpreters. Add your specific commands to the
 * exec() function, and whatever you need to setup() and loop().
 *
 * Usage:
 *   Talk to it through the serial port at 9600/8N1. Commands should be
 *   terminated by CR (\r), answers are terminated by CRLF (\r\n). This
 *   should play well with typical terminal emulators. For the list of
 *   supported commands, see the answer to the "help" command.
 *
 * Copyright (c) 2016 Edgar Bonet Orozco.
 * Released under the terms of the MIT license:
 * https://opensource.org/licenses/MIT
 */

#include <avr/pgmspace.h>
#include "Arduino.h"
#include <EDB.h>
#include <EEPROM.h>

#define TABLE_SIZE 512
#define BUF_LENGTH 128  /* Buffer for the incoming command. */

int shade;
int size;
int puckname;


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


//Start of the Serial Command reader
static bool do_echo = true;



/* Execute a complete command. */
static void exec(char *cmdline)
{
    char *command = strsep(&cmdline, " ");

    if (strcmp_P(command, PSTR("help")) == 0) {
        Serial.println(F(
            "addrecord <PuckShade> <Size> <PuckName>: createRecord();\r\n"
            "read <pin>: digitalRead()\r\n"
            "aread <pin>: analogRead()\r\n"
            "write <pin> <value>: digitalWrite()\r\n"
            "awrite <pin> <value>: analogWrite()\r\n"
            "echo <value>: set echo off (0) or on (1)"));
    } else if (strcmp_P(command, PSTR("addrecord")) == 0) {

    } else if (strcmp_P(command, PSTR("read")) == 0) {
        int pin = atoi(cmdline);
        Serial.println(digitalRead(pin));
    } else if (strcmp_P(command, PSTR("aread")) == 0) {
        int pin = atoi(cmdline);
        Serial.println(analogRead(pin));
    } else if (strcmp_P(command, PSTR("write")) == 0) {
        int pin = atoi(strsep(&cmdline, " "));
        int value = atoi(cmdline);
        digitalWrite(pin, value);
    } else if (strcmp_P(command, PSTR("awrite")) == 0) {
        int pin = atoi(strsep(&cmdline, " "));
        int value = atoi(cmdline);
        analogWrite(pin, value);
    } else if (strcmp_P(command, PSTR("echo")) == 0) {
        do_echo = atoi(cmdline);
    } else {
        Serial.print(F("Error: Unknown command: "));
        Serial.println(command);
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
#define puckname;
//Create the Records
void createRecord(int shade, int size) {
  Serial.printf("Creating Record...");
  newPuck.shade = shade;
  newPuck.size = size;
  newPuck.puckname = puckname;
  EDB_Status result = db.appendRec(EDB_REC newPuck);
  if (result != EDB_OK) printError(result);
  Serial.println("DONE");
}

void recordLimit()  {
  Serial.print("Record Limit: ");
  Serial.println(db.limit());
}

void deleteOneRecord(int recno) {
  Serial.print("Deleting recno: ");
  Serial.println(recno);
  db.deleteRec(recno);
}

void deleteAll()  {
  Serial.print("Turnacating table...");
  db.clear();
  Serial.println("DONE");
}

void countRecords() {
  Serial.print("Record Count: ");
  Serial.println(db.count());
}

//Real Stuff Begins
void setup()
{
    Serial.begin(9600);
    Serial.print("Creating table...");
    // create table at with starting address 0
    db.create(0, TABLE_SIZE, (unsigned int)sizeof(newPuck));
    Serial.println("DONE");
    recordLimit();
    countRecords();
    createRecord(A1, 12);
}

void loop() {

    /* Process incoming commands. */
    while (Serial.available()) {
        static char buffer[BUF_LENGTH];
        static int length = 0;

        int data = Serial.read();
        if (do_echo) Serial.write(data);
        if ((data == '\b' || data == '\177') && length) {  // BS and DEL
            length--;
            if (data == '\177') Serial.write('\b');
            Serial.write(" \b");
        }
        else if (data == '\r') {
            if (do_echo) Serial.write('\n');    // output CRLF
            buffer[length] = '\0';
            if (length) exec(buffer);
            length = 0;
        }
        else if (length < BUF_LENGTH - 1)
            buffer[length++] = data;
    }

}
