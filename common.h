#ifndef DNCODE_H
#define DNCODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types


typedef struct _DocodeInfo
{ 

    
    char output_file_base_name[50];
    char secret_fname;       
    FILE *fptr_secret;     
    char extn_secret_file[10];     
    int size_secret_file;   

    
    char *stego_image_fname; 
    FILE *fptr_stego_image;  

} DecodeInfo;


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);


Status do_decoding(DecodeInfo *decInfo);


Status open_files(DecodeInfo *decInfo);



Status skip_bmp_header( FILE *fptr_stego_image);


Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);


Status decode_secret_file_extn_size( DecodeInfo *decInfo);


Status decode_secret_file_extn(DecodeInfo *decInfo);


Status decode_secret_file_size(DecodeInfo *decInfo);


Status decode_secret_file_data(DecodeInfo *decInfo);


Status decode_byte_to_lsb(char *data, char *image_buffer);


Status decode_size_to_lsb(int *size, char *imageBuffer);


#endif
