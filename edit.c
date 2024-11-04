#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types.h"
#include"header.h"

//function to check which option to edit the user has passed
Edit_options check_e_option(char *argv[])
{
    if( strcmp(argv[2], "-t") == 0 )
    {
        return e_t;  
    }

    else if( strcmp(argv[2], "-a") == 0 )
    {
        return e_a;  
    }

    else if( strcmp(argv[2], "-A") == 0 ) 
    {
        return e_A;
    }

    else if( strcmp(argv[2], "-y") == 0 )
    {
        return e_y;
    }

    else if( strcmp(argv[2], "-m") == 0 )
    {
        return e_m;
    }

    else if( strcmp(argv[2], "-c") == 0 )
    {
        return e_c;
    
    }

    else
    {
        return NA; //if user has selected an invalid option to edit
    }

}


//function to edit the mp3 tag data
Status to_edit(char *frameID, TAGINFO *tag, char *argv[])
{
      //open the mp3 file in reading mode
      tag -> file = fopen(tag -> mp3file_name, "r");
     
      //a temporary file to hold the data temporarily
      FILE * ftemp;
      ftemp = fopen("temp.mp3" , "w");
 
      if ((! tag -> file) || ! ftemp )
      {
          printf("Error opening files\n");
          return e_failure;
      }
 
      // Skip the ID3 header
      //copy the header data from original file to temporary file 
      char header[10];
      fread(header, 10 ,1 , tag -> file);
      fwrite(header, 10 ,1 , ftemp);
    
       char ch;  //variable to hold the characters extracted from mp3 file
      
       //looping till the end of file
       while( (ch = fgetc(tag -> file)) != EOF)
       {
            ungetc(ch,tag -> file); //to put back the character back to the stream
            
            char fid[5]; //variable to hold the frame id read from the original mp3 file
            fread(fid, 1, 4, tag -> file);  // Read frame ID
            fid[4] = '\0';     //to terminate the string with null for error handling 
        
            uint f_size;       
            fread(&f_size, 4, 1, tag -> file);  // Read frame size
 
                // Convert the size from big endian to little endian
            uint lEndian = f_size;
            lEndian =  ((lEndian & 0xFF000000) >> 24) | ((lEndian & 0x00FF0000) >> 8)  |  ((lEndian &     0x0000FF00) << 8)  |  ((lEndian & 0x000000FF) << 24) ;
            
            unsigned int *point = &f_size; //pointer to hold the address of size variable
 
            // Skip the frame flags (2 bytes)
            fseek(tag -> file,2 , SEEK_CUR);
    
            //printf("comparing %s \n", fid);
            //printf("with the frame id : %s \n",frameID);
    
            //if the obtained frameID matches with the frameID to be edited
                if (strcmp(frameID, fid) == 0)
                {
                    // store the new data to be entered
                    char *new_data = argv[3];
                    //calculate the new size
                    unsigned int new_size = strlen(argv[3]);

                    //converting the size to big endian in order to store in the mp3 file
                    uint  new_size_bigendian = ((new_size & 0xFF000000) >> 24) | ((new_size & 0x00FF0000) >> 8) | ((new_size & 0x0000FF00) << 8) | ((new_size & 0x000000FF) << 24);
                
                    unsigned int * p = &new_size_bigendian;  //pointer to hod the address of size variable in big endian format
                
                    //copy the frameID size,the flag bytes and the new data into the temporary file
                    fwrite(fid ,1 ,4 ,ftemp);
                    fwrite(p ,4 ,1 ,ftemp);
                    fwrite("\0\0", 2, 1, ftemp);
                    fwrite(new_data ,new_size, 1,ftemp);
                
                    //to move the file offset of original file in order to make it point to next frameID
                    fseek(tag -> file, lEndian, SEEK_CUR);
              
                    //printf("Copied matching data \n");
                    break;  // to edit only the first matching frame and come out of the loop
                } 
                //if the obtained frameID does not match with the frameID we want to edit copy it as it is to the temp file    
                else 
                {
                    // write the obtained frame contents as it is to the temporary file
                    fwrite(fid ,1 ,4 ,ftemp);
                    fwrite(point ,4 ,1 ,ftemp);
                    fwrite("\0\0" ,2 ,1 ,ftemp);
                    char *data = malloc(sizeof(char) * lEndian); //allocate memory to copy the frame contents
                
                    //read the frame contents and write it to the temporary file
                    fread(data, lEndian , 1 ,tag -> file);
                    fwrite(data, lEndian, 1, ftemp);
                    free(data);

                    //printf("copied unmatching frame data\n");
                }
        }
 
       //to copy the remaining data from the original file to temporary file

        char content[1];  //variable  to hold the characters being read from the original file 
 
        while(! feof(tag -> file))
        {
            content[0] = fgetc(tag -> file);
            fwrite(content, 1, 1, ftemp);
        }
        //printf("all contents copied from original file to temp file\n");
   
        //close the files after copying
        fclose(tag -> file);
        fclose(ftemp);

        //open the original file in writing mode
        tag -> file = fopen(tag -> mp3file_name, "w");

        //open the temporary file in reading mode
        ftemp = fopen("temp.mp3" , "r");

        if ((! tag -> file) )
        {
           printf("Error opening file\n");
           return e_failure;
        }

        //copy all the contents from the temporary file to the original file
        while(! feof(ftemp))
        {
            content[0] = fgetc(ftemp);
            fwrite(content, 1, 1, tag -> file);
        }
        // printf("all contents copied from temp file to original file\n");
  
        //close both the files
        fclose(tag -> file);
        fclose(ftemp);
        
        remove ("temp.mp3");  //remove the temporarily created file

        return e_success;
 }

