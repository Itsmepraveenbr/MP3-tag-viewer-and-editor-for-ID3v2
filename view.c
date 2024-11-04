#include <stdio.h>
#include <string.h>
#include "types.h"
#include "header.h"
#include <stdlib.h>

//function to print usage message
void printusage_message(char * argv[])
{
    printf("ERROR : %s %s  INVALID ARGUMENTS\n", argv[0], argv[1]);
    printf("USage: \n To view please pass like:  ./a.out -v mp3filename \n to edit please pass     like : ./a.out -e -t/-a/-A/-y/-m/-c  changing_text mp3_filename\n To get help pass like: ./a.out     --help\n ");
    printf("\n");

}

//function to read and validate the input arguments
Status read_and_validate_args(char *argv[], TAGINFO *taginfo)
{
    //to check if the user has passed a .mp3 file or not
    if( ! (strcmp(strstr(taginfo -> mp3file_name, "." ), ".mp3" ) == 0))
    {
        printf("please pass a file with .mp3 extension\n");
        return e_failure;
    }
        
    //if the user has passed an mp3 file open the filr in reading mode
    taginfo -> file = fopen(taginfo -> mp3file_name, "r");
    
    if(taginfo -> file == NULL)
    {
       printf("unable to open the %s file\n", argv[2]);
       return e_failure;
    }
    else
    {
      // printf("opened the mp3 file successfully\n");
       
      //to read the ID3 tag
       char str[3];  // variable to hold the tag present in mp3 file user has passed
       fread(str, 1, 3, taginfo -> file);

       //to check if the passed file has ID3 tag or not
       if(! (strcmp(str, "ID3") == 0))
       {
           printf("the given mp3 file %s is not of ID3 signature\n", argv[2]);
           return e_failure;
       }

       fclose(taginfo -> file);
    }
   return e_success;
}

//function to view the taginfo
Status to_view(TAGINFO *taginfo)
{
    //open the mp3 file in reading mode
     taginfo -> file = fopen(taginfo -> mp3file_name, "r");
  
     if( taginfo -> file == NULL)
     {
         printf("Unable to open the file\n");
     }
     //to move the file offset 10 bytes by skipping the header
     fseek( taginfo -> file, 10L, SEEK_SET);
    
     //to check the Frame IDs and print their contents
     while( ! feof(taginfo -> file))
     {
        char str[4];  //variable to store the frameID
        int ret;
       
        int count =0; 
        //to read the frameID to and store to str variable
        fread(str,1,4, taginfo -> file);

        //if the read frame id matched with TIT2 printing the title contents 
         if( strcmp(str, "TIT2") == 0)
         {
            ret = display_info(taginfo);
    
            if(ret == e_success)
            {
                if( (taginfo -> frame_content )[0] != '\0')
                {
                    printf("TITLE : %s \n",(taginfo -> frame_content));
                }
                else
                {
                    printf("TITLE : %s \n",(taginfo -> frame_content)+1);
                }
                count++;
               
            } 
         }

         //if the read frame id matched with TPE1 printing the artist name 
         else if( strcmp(str, "TPE1") == 0)
         {
            ret = display_info(taginfo);

            if(ret == e_success)
            {
                if((taginfo -> frame_content)[0] != '\0')
                {
                    printf("ARTIST : %s \n",(taginfo -> frame_content));
                }
                else
                {    
                    printf("ARTIST : %s \n",(taginfo -> frame_content)+1);
                }
                count++;
               
            }
         }

         //if the read frame id matched with TALB printing the album name 
         else if( strcmp(str, "TALB") == 0)
         {
            ret = display_info(taginfo);

            if(ret == e_success)
            {
                if((taginfo -> frame_content)[0] != '\0')
                {
                     printf("ALBUM : %s \n", (taginfo -> frame_content));
                }
                else
                {
                    printf("ALBUM : %s \n", (taginfo -> frame_content) + 1);
                }
                count++;
               
            }
         }

         //if the read frame id matched with TYER printing the year 
         else if( strcmp(str, "TYER") == 0)
         {
            ret = display_info(taginfo);

            if(ret == e_success)
            {
                if((taginfo -> frame_content)[0] != '\0')
                {
                    printf("YEAR : %s \n",(taginfo -> frame_content));
                }
                else
                {
                    printf("YEAR : %s \n",(taginfo -> frame_content) + 1);
                }
                count++;
                
            }
         }

         //if the read frame id matched with TCON printing the conent type
         else if( strcmp(str, "TCON") == 0)
         {
            ret = display_info(taginfo);

            if(ret == e_success)
            {
                if((taginfo -> frame_content)[0] != '\0')
                {
                    printf("CONTENT TYPE : %s \n",(taginfo -> frame_content));
                }
                else
                {
                    printf("CONTENT TYPE : %s \n",(taginfo -> frame_content) + 1);
                }
                count++;
                
            }
         }

         //if the read frame id matched with TCOM printing the composer
         else if( strcmp(str, "TCOM") == 0)
         { 
            ret = display_info(taginfo);

            if(ret == e_success)
            {
                if((taginfo -> frame_content)[0] != '\0')
                {
                    printf("COMPOSER : %s \n",(taginfo -> frame_content));
                }
                else
                {
                    printf("COMPOSER : %s \n",(taginfo -> frame_content) + 1);
                }
                count++;
            }
         }

         //if the read frame id matched with COMM printing the comments
         else if( strcmp(str, "COMM") == 0)
         {
            ret = display_info(taginfo);

            if(ret == e_success)
            {
                if((taginfo -> frame_content)[0] != '\0')
                {
                    printf("COMMENTS : %s \n",(taginfo -> frame_content));
                }
                else
                {  
                    printf("COMMENTS : %s \n",(taginfo -> frame_content) + 1);
                }
              count++;
            }
        }

    //this is to move the offset for the frames which we are not displaying
       else
        {
            display_info(taginfo);
            //  printf(" %s : %s\n",str,taginfo->title);
              
        }

     }
    
    fclose(taginfo -> file);
    return e_success;

}


//to display the contents of the frame id
Status display_info(TAGINFO *t)
{
        
        int * a = &(t -> frame_size);  //pointer to read the frame size 
    
        // to read the frame size 
        fread( (void *)a ,4,1, t -> file);

        //converting the size from big endian into little endian form 
        uint lEndian = (t) -> frame_size;
        lEndian =  ((lEndian & 0xFF000000) >> 24) | ((lEndian & 0x00FF0000) >> 8)  |  ((lEndian & 0x0000FF00) << 8)  |  ((lEndian & 0x000000FF) << 24) ;

        //allocate the size bytes of memory to read the frame contents
        t -> frame_content = malloc(sizeof(char)*(lEndian));
        
        //skip the 2 bytes of flag
        fseek(t -> file,2L,SEEK_CUR);

        //  printf("%ld\n",ftell(file));

        //to read the frame content from the file and store it in one variable
        fread(t -> frame_content ,1 , lEndian, t -> file);

        return e_success;
}







