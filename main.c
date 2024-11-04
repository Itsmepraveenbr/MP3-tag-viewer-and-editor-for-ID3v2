/* Name : Praveen B R
Description : MP3 Tag viewer and Editor project
 */

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "header.h"


int main(int argc, char* argv[] )
{
    //display error if required no of args not passed
    if(argc < 2)
    {
        printf("_____________________________________________________\n");
        printf("\n");
        printusage_message(argv);
        printf("________________________________________________________\n");

        return e_failure;

    }

    else
    {
        TAGINFO taginfo;
        int ret = check_operation_type(argv);
        //to display help menue
        if(ret == e_help)
        {
            printf("_______________________HELP MENUE_______________________\n");
            printf("\n");
            printf("1. -v ->to view m;p3 file contents\n");
            printf("2. -e ->to edit mp3 file contents \n");
            printf("\t 2.1 -t -> to edit song title\n");
            printf("\t 2.2 -a -> to edit artist name\n");
            printf("\t 2.3 -A -> to edit album name\n");
            printf("\t 2.4 -y -> to edit year\n");
            printf("\t 2.5 -m -> to edit content\n");
            printf("\t 2.6 -c -> to edit comment\n");
            printf("\n");
            printf("__________________________________________________________\n");
        }

        //if user selects to view 
        else if(ret == e_view)
        {
            //if user does not pass the mp3 file show error
            if(argv[2] == NULL)
            {
                printusage_message(argv);
                return e_failure;
            }
            else
            {
                printf("_____________SELECTED VIEW DETAILS______________________\n");
                printf("\n");
                printf("_________________________________________________________\n");
               
                //storing the mp3 file name to a variable
                taginfo.mp3file_name = argv[2];    
                
                //to validate the passed input file by opening it and reading for ID3 tag and version
                if(read_and_validate_args(argv,&taginfo) == e_success)
                {

                    printf("\t MP3 TAG READER AND EDITOR FOR ID3V2\n");
                    printf("_________________________________________________________\n");
                    
                    //if the file is validated print the information present in the file
                    if( to_view(&taginfo ) == e_success)
                    {
                        printf("______________________________________________________\n");
                        printf("\n");
                        printf("_________DETAILS DISPLAYED SUCCESSFULLY_________________\n");
                        printf("\n");
                    }
                    else
                    {
                        printf("unable to view taginfo\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Read and validate args failed please enter a valid file\n");
                }

            }

        }

        //if user selects to edit
        else if(ret == e_edit)
        {
            //to check user has passed the required number of arguments or not
            if(argc == 5)
            {
                //storing the mp3 file name in one variable
                taginfo.mp3file_name = argv[4];

                //to validate the mp3 file
                if(read_and_validate_args(argv,&taginfo) == e_success)
                {
                    printf("_________________SELECTED EDIT DETAILS_______________\n");
                    printf("\n");
                    printf("_____________________SELECTED EDIT OPTION________________\n");

                    Edit_options option; //variable to hold the edit option selected by the user
                            
                    option =  check_e_option(argv); //call the function to check user has passedd which option to edit

                    switch(option)
                    {
                        case e_t:
                            printf("________________CHANGE THE TITLE________________\n");

                            //to edit the title by calling to_edit function
                            if(to_edit("TIT2",&taginfo,argv) == e_success)
                            {
                                printf("TITLE  : %s\n", argv[3]);
                                printf("____________TITLE CHANGED SUCCESSFULLY_________\n");
                            }
                            break;
                        case e_a:
                            printf("________________CHANGE THE ARTIST NAME________________\n");

                              //to edit the artist by calling to_edit function
                            if(to_edit("TPE1",&taginfo,argv) == e_success)
                            {
                                printf("ARTIST  : %s\n", argv[3]);
                                printf("____________ARTIST CHANGED SUCCESSFULLY_________\n");
                            }
                            break;
                        case e_A:
                            printf("________________CHANGE THE ALBUM________________\n");

                              //to edit the album by calling to_edit function
                            if(to_edit("TALB",&taginfo,argv) == e_success)
                            {
                                printf("ALBUM  : %s\n", argv[3]);
                                printf("____________ALBUM CHANGED SUCCESSFULLY_________\n");
                            }
                            break;
                        case e_y:
                            printf("________________CHANGE THE YEAR________________\n");

                              //to edit the year by calling to_edit function
                            if(to_edit("TYER",&taginfo,argv) == e_success)
                            {
                                printf("YEAR  : %s\n", argv[3]);
                                printf("____________YEAR CHANGED SUCCESSFULLY_________\n");
                            }
                            break;
                        case e_m:
                            printf("________________CHANGE THE CONTENT TYPE________________\n");

                              //to edit the content type by calling to_edit function
                            if(to_edit("TCON",&taginfo,argv) == e_success)
                            {
                                printf("CONTENT TYPE  : %s\n", argv[3]);
                                printf("____________CONTENT TYPE CHANGED SUCCESSFULLY_________\n");
                            }
                            break;
                        case e_c:
                            printf("________________CHANGE THE COMMENT________________\n");

                              //to edit the comment by calling to_edit function
                            if(to_edit("COMM",&taginfo,argv) == e_success)
                            {
                                printf("COMMENT  : %s\n", argv[3]);
                                printf("____________COMMENT CHANGED SUCCESSFULLY_________\n");
                            }
                            break;

                        default: 
                                    //printing error if user does not pass a valid argument
                            printf("please pass the correct argument -t/-a/-A/-y/-m/-c   \n");
                            break;
                    }

                }
                //if .mp3 file is not passed validation of the file failed
                else
                {
                    printf("Read and validate args failed\n");
                }

            }

            //if user does not pass 5 arguments   
            else
            {
                printusage_message(argv);
            }

        }

        //if he user does not pass valid arguments (other than -v, -e, --help)
        else
        {
            printusage_message(argv);
            return e_failure;
        }

    }

}

//function to check which opration the user wats to perform
OperationType check_operation_type(char *argv[])
{
    if( strcmp(argv[1], "-v" ) == 0 )
    {
        return e_view;
    }
    else if( strcmp(argv[1], "-e" ) == 0 )
    {
        return e_edit;
    }
    else if( strcmp(argv[1], "--help" ) == 0 )
    {
        return e_help;
    }
    else
    {
        return e_unknown;
    }

}



