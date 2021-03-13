#ifndef ANON_EDF_H
#define ANON_EDF_H
/* Author: Pablo Couso */

#include <iostream>
#include "edf_hdr.h"

typedef struct localCodes {
  char original[60];
  char transformed[60];
} LocalCodes;

int read_csv(LocalCodes *, char *, int);
void append_to_csv(const char *, const char *, char *);
int get_transformed_code(char *, const char *, LocalCodes *, int);
int modify_patient_code(char *, LocalCodes *, char *, int);
void modify_patient_bday(char *);
int count_file_rows(char *);
int init_both_codes_data(LocalCodes **, char *, int);
int create_new_files(int argc, char* ,char *argv[]);

#endif // ANON_EDF_H
