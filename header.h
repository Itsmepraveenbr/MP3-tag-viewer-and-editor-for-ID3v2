#ifndef HEADER_H
#define HEADER_H

#include "types.h"

//structure to hold the mp3 file and frame info
typedef struct 
{
    //for file and fraemID
    char *mp3file_name;  //to hold the mp3 file name
    FILE * file;         //file pointer for the mp3 file
    char frameID[4];       //to hold the frameID 

    //for frame contents and its size
    char *frame_content;
    uint frame_size;
 
}TAGINFO;


//TO PRINT USAGE MESSAGE
void printusage_message(char *argv[]);

//to check the type of operation
OperationType check_operation_type(char *argv[]);

//to validate the arguments
Status read_and_validate_args(char *argv[], TAGINFO *taginfo);

//too view the contents
Status to_view(TAGINFO *taginfo );

//to display each contents
Status display_info(TAGINFO *t);

//check which edit option is passesd
Edit_options check_e_option(char *argv[]);

//to edit the content
Status to_edit(char*FrameID,TAGINFO *tag,char * argv[]);


#endif



