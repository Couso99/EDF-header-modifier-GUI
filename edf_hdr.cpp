#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edf_hdr.h"

/* Author: Pablo Couso */

void str2patientData(PatientData* data,const char*string)
{
  int i=0;
  char str[81], *tok;
  strncpy(str,string,80);
  for(tok=strtok(str," "); tok && *tok; i++, tok=strtok(NULL," "))
  {
    switch (i) {
      case 0:
        strcpy(data->localCode,tok);
        break;
      case 1:
        strcpy(data->sex,tok);
        break;
      case 2:
        strcpy(data->bday,tok);
        break;
      case 3:
        strcpy(data->name,tok);
        break;
    }
  }
  free(tok);
  return;
}

void str2recordingData(RecordingData* data,const char*string)
{
  int i=0;
  char str[81], *tok;
  strncpy(str,string,80);
  for(tok=strtok(str," ");tok && *tok; i++, tok=strtok(NULL," "))
  {
    switch (i) {
      case 0:
        strcpy(data->startText,tok);
        break;
      case 1:
        strcpy(data->startDate,tok);
        break;
      case 2:
        strcpy(data->adminCode,tok);
        break;
      case 3:
        strcpy(data->technician,tok);
        break;
      case 4:
        strcpy(data->equipment,tok);
        break;
    }
  }
  free(tok);
  return;
}

void patientData2str(char* str, const PatientData *data)
{
  int j;

  if (!data)
  {
    char empty_data[] = "X X X X";
    for (j=0; empty_data[j]; j++)
      str[j] = empty_data[j];
    str[j] = '\0';
    return;
  }

  for (j=0; data->localCode[j] && data->localCode[j]!=' ';j++)
    str[j] = data->localCode[j];

  for (int i=1;i<4;i++)
  {
    str[j] = ' ';
    j++;
    switch (i) {
      case 1:
        str[j] = data->sex[0];
        j++;
        break;
      case 2:
        for(int k=0;data->bday[k];j++,k++)
          str[j]=data->bday[k];
        break;
      case 3:
        for(int k=0;data->name[k] && data->name[k]!=' ';j++,k++)
          str[j]=data->name[k];
        break;
    }
  }
  str[j]='\0';
  return;
}

void recordingData2str(char *str, const RecordingData *data)
{
  int i, j, k;

  if (!data)
  {
    char empty_data[] = "Startdate X X X X";
    for (j=0; empty_data[j]; j++)
      str[j] = empty_data[j];
    str[j] = '\0';
    return;
  }

  for (j=0; data->startText[j] && data->startText[j]!=' ';j++)
    str[j] = data->startText[j];
  for (i=1;i<5;i++)
  {
    str[j] = ' ';
    j++;
    switch (i) {
      case 1:
        for (k=0;data->startDate[k];j++,k++)
          str[j] = data->startDate[k];
        break;
      case 2:
        for (k=0;data->adminCode[k];j++,k++)
          str[j] = data->adminCode[k];
        break;
      case 3:
        for (k=0;data->technician[k];j++,k++)
          str[j] = data->technician[k];
        break;
      case 4:
        for (k=0;data->equipment[k];j++,k++)
          str[j] = data->equipment[k];
        break;
    }
  }
  str[j]='\0';
  return;
}

// dd-MMM-yyyy -> dd.mm.yy
void dateLetter2dateNumber(char *date_number,const char *date_letter)
{
  int j, i=0;
  char date_copy[12], *tok;
  strncpy(date_copy,date_letter,12);

  for(tok=strtok(date_copy,"-");tok && *tok; i++, tok=strtok(NULL,"-"))
    switch (i)
    {
      case 0:
        for (j=0;tok[j];j++)
          date_number[j]=tok[j];
        date_number[j] = '.';
        j++;
        break;
      case 1:
        date_number[j]='0'; j++;
        if(strcmp(tok,"JAN"))       date_number[j]='1';
        else if(strcmp(tok,"FEB"))  date_number[j]='2';
        else if(strcmp(tok,"MAR"))  date_number[j]='3';
        else if(strcmp(tok,"APR"))  date_number[j]='4';
        else if(strcmp(tok,"MAY"))  date_number[j]='5';
        else if(strcmp(tok,"JUN"))  date_number[j]='6';
        else if(strcmp(tok,"JUL"))  date_number[j]='7';
        else if(strcmp(tok,"AUG"))  date_number[j]='8';
        else if(strcmp(tok,"SEP"))  date_number[j]='9';
        else if(strcmp(tok,"OCT")) {date_number[j-1]='1';date_number[j] = '0';}
        else if(strcmp(tok,"NOV")) {date_number[j-1]='1';date_number[j] = '1';}
        else if(strcmp(tok,"DEC")) {date_number[j-1]='1';date_number[j] = '0';}
        j++;
        date_number[j] = '.';
        j++;
        break;
      case 2:
        for (int k=2;tok[k];k++,j++)
          date_number[j] = tok[k];
        break;
    }
  date_number[j] = '\0';
  return;
}

// Read file header
int get_header(char *buf, const char *ifile_path)
{
  int n;
  FILE *ifile;
  if ((ifile = fopen(ifile_path, "rb")) == NULL) {
    fprintf(stderr, "Can't open %s\n", ifile_path);
    exit(1);
  }
  /* All EDF/EDF+ files begin with a 256-byte fixed-format header */
  n = fread(buf, 1, 1024, ifile);
  if (n < 256) {
    fprintf(stderr, "%s is too short for an EDF/EDF+ file\n No output written\n", ifile_path);
    fclose(ifile);
    exit(1);
  }
  fclose(ifile);
  return n;
}

// Extract header into the respective sections
void process_header(PatientData *p_data, RecordingData *r_data, char* start_date, char*start_time,const char *buf)
{
  if (p_data) str2patientData(p_data,buf+8);
  if (r_data) str2recordingData(r_data,buf+88);
  if (start_date)
  {
      strncpy(start_date,buf+168,8);
      start_date[8] = '\0';
  }
  if (start_time)
  {
      strncpy(start_time,buf+176,8);
      start_time[8] = '\0';
  }

  return;
}

void modify_header(char *buf, PatientData *p_data, RecordingData *r_data, char* start_date, char*start_time)
{
  int i;
  char p_data_str[81], r_data_str[81];

  // Change structs to strings for writing
  patientData2str(p_data_str, p_data);
  recordingData2str(r_data_str, r_data);

  // Replace local patient identification data
  for (i = 0; i < 80 && p_data_str[i]; i++) buf[i+8] = p_data_str[i];
  for (; i < 80; i++) buf[i+8] = ' ';

  // Replace local recording identification data
  for (i = 0; i < 80 && r_data_str[i]; i++) buf[i+88] = r_data_str[i];
  for (; i < 80; i++) buf[i+88] = ' ';

  // Replace date and time of recording
  if (start_date) strncpy(buf+168, start_date, 8);
  if (start_time) strncpy(buf+176, start_time, 8);

  // Check the format and warn if the reserved area is not empty.
  i = 5;
  if (strncmp("EDF+C", buf+192, 5) == 0) fprintf(stderr, "Format: EDF+, continuous recording\n");
  else if (strncmp("EDF+D", buf+192, 5) == 0) fprintf(stderr, "Format: EDF+, discontinuous recording\n");
  else {
    fprintf(stderr, "Format: EDF\n");
    i = 0;
  }

  for ( ; i < 44; i++) {
    if (buf[i+192] != ' ') {
      fprintf(stderr, "WARNING\n Reserved area of header is not empty as expected\n"
	    " Check bytes %d-236 of intput file\n", i+192);
      break;
    }
  }
  puts("Header successfully modified");
  return;
}

void write_to_file(char *buf, const char *ofile_path, const char *ifile_path, int n)
{
  FILE *ifile, *ofile;

  if ((ofile = fopen(ofile_path, "wb")) == NULL) {
    fprintf(stderr, "Can't open %s\n", ofile_path);
    exit(1);
  }
  if ((ifile = fopen(ifile_path, "rb")) == NULL) {
    fprintf(stderr, "Can't open %s\n", ifile_path);
    exit(1);
  }

  // Start reading file after header
  fseek(ifile,n,SEEK_SET);

  // Write the output, beginning with the modified header
  do fwrite(buf, 1, n, ofile);
  while ((n = fread(buf, 1, 1024, ifile)));

  fclose(ofile);
  fclose(ifile);
  printf("Successfully written to file %s\n", ofile_path);
  return;
}
