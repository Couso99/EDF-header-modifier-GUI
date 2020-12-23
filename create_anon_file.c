#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edf_hdr.h"

typedef struct localCodes
{
  char original[60];
  char transformed[60];
} LocalCodes;

// Load data from .csv to data
int read_csv(LocalCodes *data, char *fname, int skip)
{
  int i, j, k;
  char line[1024];
  const char* tok;
  FILE *file;

	if (!(file = fopen(fname, "r")))
  {
    printf("New file created: %s\n", fname);
    file=fopen(fname, "w+");
    char header_csv[] = "OriginalLocalCode,TransformedLocalCode";
    fprintf(file, "%s\n", header_csv);
    fclose(file);
    return 0;
  }

	for (i=0;fgets(line, 1024, file);i++)
    {
      if(i<skip) continue;
	    for (j=0, tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n"))
        switch (j)
        {
          case 0:
            strcpy(data[i-skip].original, tok);
            break;
          case 1:
            strcpy(data[i-skip].transformed, tok);
            break;
        }
    }
  fclose(file);
  return (i-skip);
}

// Append new row to .csv
void append_to_csv(const char *original_localCode, const char *transformed_localCode, char *fname)
{
  FILE *file;
  char new_row[121];
  strcpy(new_row, original_localCode);
  strcat(new_row, ",");
  strcat(new_row, transformed_localCode);

  file = fopen(fname, "a");
  fprintf(file, "%s\n", new_row);
  fclose(file);

  return;
}

int get_transformed_code(char *transformed_localCode, const char *original_localCode, LocalCodes *data, int rows)
{
  for (int i=0; i<rows && *(data[i].original); i++)
    if(!strcmp(data[i].original,original_localCode))
    {
      strcpy(transformed_localCode,data[i].transformed);
      return 1;
    }
  sprintf(transformed_localCode, "SUJ%.4d", rows+1);
  return 0;
}

// Change original code to  transformed
int modify_patient_code(char *localCode, LocalCodes *data, char *fname, int *rows)
{
  int i, j, k, retval, boolRealloc, rows_local;
  char original_localCode[60], transformed_localCode[60];

  strcpy(original_localCode, localCode);
  boolRealloc = 0;
  rows_local = *rows;

  retval = get_transformed_code(transformed_localCode, original_localCode, data, rows_local);
  if (!retval)
  {
    append_to_csv(original_localCode, transformed_localCode, fname);
    strcpy(data[rows_local].original, original_localCode);
    strcpy(data[rows_local].transformed, transformed_localCode);
    boolRealloc = 1;
    (*rows)++;
  }

  strcpy(localCode, transformed_localCode);
  return boolRealloc;
}

// Method to modify dates to 1st of January but keep the year
void modify_patient_bday(char *bday)
{
  int i;
  char day_month[] = "01-JAN";
  for (i=0;i<6;i++)
    bday[i] = day_month[i];
  return;

}

// Initialize data and load it from .csv to data
void init_both_codes_data(LocalCodes **data, char *fname, int *rows, int skip)
{
  (*data) = (LocalCodes *) malloc(*rows * sizeof(LocalCodes));
  *rows = read_csv((*data), fname, skip);
  (*data) = (LocalCodes *) realloc((*data),((*rows)+1)*sizeof(LocalCodes));
  printf("Rows: %d\n", *rows);
  return;
}

int main(int argc, char const *argv[])
{
  int i, n, boolRealloc, rows, skip;
  static char buf[1024];
  char start_date[9], start_time[9];
  PatientData p_data;
  RecordingData r_data;
  LocalCodes *both_codes_data;

  // Depends on csv and number of lines
  rows=50; // Must be above actual number of lines in csv
  skip=1;  // Number of header rows in csv

  char empty_recording_data[] = "Startdate X X X X"; // Empty local recording data
  char code_transformation_file[] = "RESULTS/relacion_codigos_sujetos.csv";

  init_both_codes_data(&both_codes_data, code_transformation_file, &rows, skip);

  char *ifile_path = (char *) malloc(strlen(argv[1])+1);
  char *ofile_path = (char *) malloc(strlen(argv[2])+1);
  strcpy(ifile_path, argv[1]);
  strcpy(ofile_path, argv[2]);

  n = get_header(buf, ifile_path);
  process_header(&p_data, NULL, NULL, NULL, buf);

  /* Modify here p_data, r_data, start_date, start_time */
  /* -------------------------------------------------- */

    // Method to modify name according to .csv
  boolRealloc = modify_patient_code(p_data.localCode, both_codes_data, code_transformation_file, &rows);
  if (boolRealloc) both_codes_data = (LocalCodes *) realloc(both_codes_data, rows+1);

  modify_patient_bday(p_data.bday); // Modify birthday date to keep year and change day and month

  strcpy(p_data.sex, "X"); // Anonymize sex
  strcpy(p_data.name,"X"); // Anonymize name

  str2recordingData(&r_data, empty_recording_data); // Anonymize whole recording data info

  /* -------------------------------------------------- */

  // Write header to buf (char[1024])
  modify_header(buf, &p_data, &r_data, NULL, NULL);
  // Write new file with modified header
  write_to_file(buf, ofile_path, ifile_path, n);

  free(ifile_path);
  free(ofile_path);
  free(both_codes_data);

  return 0;
}
