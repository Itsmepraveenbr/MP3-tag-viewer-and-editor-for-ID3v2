#ifndef TYPES_H
#define TYPES_H

//used for size calculations
typedef unsigned int uint;


//to use as a return type for the functions
typedef enum
{
    e_success,
    e_failure
}Status;

//to use as a return type to tell the  option selected by the user 
typedef enum
{
    e_view,  
    e_edit,
    e_help,
    e_unknown
}OperationType;

//to use as a return type for the function to tell  which editing option is  passed by the user
typedef enum
{
    e_t,
    e_a,
    e_A,
    e_y,
    e_m,
    e_c,
    NA
}Edit_options;

#endif
