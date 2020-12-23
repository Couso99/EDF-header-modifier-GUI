#ifndef EDF_HDR_H
#define EDF_HDR_H

/* Author: Pablo Couso */

/* EDF Header: 256 ASCII
8 ascii : version of this data format (0)
80 ascii : local patient identification (mind item 3 of the additional EDF+ specs)
80 ascii : local recording identification (mind item 4 of the additional EDF+ specs)
8 ascii : startdate of recording (dd.mm.yy) (mind item 2 of the additional EDF+ specs)
8 ascii : starttime of recording (hh.mm.ss)
8 ascii : number of bytes in header record
44 ascii : reserved
8 ascii : number of data records (-1 if unknown, obey item 10 of the additional EDF+ specs)
8 ascii : duration of a data record, in seconds
4 ascii : number of signals (ns) in data record */

// This code enables to modify the first 184 bytes, which do not interfere with the signals data

typedef struct patientData{    // Max 80 characters
  char localCode[60];  //- the code by which the patient is known in the hospital administration.
  char sex[2];         //- sex (English, so F or M).
  char bday[12];       //- birthdate in dd-MMM-yyyy format using the English 3-character abbreviations of the month in capitals. 02-AUG-1951 is OK, while 2-AUG-1951 is not.
  char name[60];       //- the patients name.
} PatientData; // X X X X is valid

typedef struct recordingData{  // Max 80 characters
  char startText[10];  //- The text 'Startdate'.
  char startDate[12];  //- The startdate itself in dd-MMM-yyyy format using the English 3-character abbreviations of the month in capitals.
  char adminCode[60];  //- The hospital administration code of the investigation, i.e. EEG number or PSG number.
  char technician[60]; //- A code specifying the responsible investigator or technician.
  char equipment[60];  //- A code specifying the used equipment.
} RecordingData; // Startdate X X X X is valid

void str2patientData(PatientData*, const char*);
void str2recordingData(RecordingData*, const char*);

void patientData2str(char*, const PatientData*);
void recordingData2str(char *, const RecordingData *);

void dateLetter2dateNumber(char *,const char *);

int get_header(char *, const char *);

void process_header(PatientData *, RecordingData *, char* , char*,const char *);

void modify_header(char *, PatientData *, RecordingData *, char* , char*);

void write_to_file(char *, const char *, const char *, int);

#endif /*EDF_HDR_H*/
