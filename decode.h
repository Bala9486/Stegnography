#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo){
    if(argv[2] == NULL || strstr(argv[2], ".bmp") == NULL){
        printf("Error: Stego image file must be a .bmp file\n");
        return e_failure;
    }
    decInfo -> stego_image_fname = argv[2];

    if(argv[3] == NULL){
        strcpy(decInfo -> output_file_base_name, "decoded_secret"); 
    }else{
        strcpy(decInfo -> output_file_base_name, argv[3]);
    }
    return e_success; 
}

Status open_decode_files(DecodeInfo *decInfo){

    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "rb");
    if(decInfo -> fptr_stego_image == NULL){
        perror("fopen");
        fprintf(stderr, "Error : Unable to open the stego image file\n");
        return e_failure;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer){
    char ch = 0;
    for(int i=0; i<8; i++){
        int bit = image_buffer[i] & 1;
        ch = ch | (bit << (7-i));
    }
    *data = ch;
    return e_success;

}
Status decode_size_from_lsb(int *size, char *imageBuffer){

    int extracted_size = 0;
    for(int i=0; i<32; i++){
        int bit = imageBuffer[i] & 1;
        extracted_size = extracted_size | (bit << (31-i));
    }
    *size = extracted_size;
    return e_success;

}
Status skip_bmp_header(FILE *fptr_stego_image){
    if(fseek(fptr_stego_image, 54, SEEK_SET) != 0){
        return e_failure;
    }
    return e_success;

}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo){
    int len = strlen(magic_string);
    char buffer[8];
    char data;
    char decode_magic[len + 1];

    for(int i=0; i<len; i++){
        if(fread(buffer, 1, 8, decInfo -> fptr_stego_image) != 8){
            return e_failure;
        }
        decode_byte_from_lsb(&data, buffer);
        decode_magic[i] = data;
    }
    decode_magic[len] = '\0';

    if(strcmp(decode_magic, magic_string) == 0){
        return e_success;
    }
    return e_failure;

}

Status decode_secret_file_extn_size(DecodeInfo *decInfo){
    char buffer[32];
    int size = 0;
    if(fread(buffer, 1, 32, decInfo -> fptr_stego_image) != 32){
        return e_failure;
    }
    decode_size_from_lsb(&size, buffer);
    decInfo -> exten_size = size;
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo){
    char buffer[8];
    int i;
    for(i=0; i<decInfo -> exten_size; i++){
        if(fread(buffer, 1, 8, decInfo -> fptr_stego_image) != 8){
            return e_failure;
        }
        decode_byte_from_lsb(&decInfo -> extn_secret_file[i], buffer);
    }
    decInfo -> extn_secret_file[i] = '\0';
    strcpy(decInfo -> secret_fname, decInfo -> output_file_base_name);
    strcat(decInfo -> secret_fname, decInfo -> extn_secret_file);
    


    decInfo -> fptr_secret = fopen(decInfo -> secret_fname, "w");
    if(decInfo -> fptr_secret == NULL){
        perror("fopen");
        return e_failure;
    }
    return e_success;

}

Status decode_secret_file_size(DecodeInfo *decInfo){
    char buffer[32];
    if(fread(buffer, 1, 32, decInfo -> fptr_stego_image) != 32){
        return e_failure;
    }
    decode_size_from_lsb(&decInfo -> size_secret_file, buffer);
    return e_success;

}

Status decode_secret_file_data(DecodeInfo *decInfo){
    char buffer[8];
    char secret_char;
    for(int i=0; i<decInfo -> size_secret_file; i++){
        if(fread(buffer, 1, 8, decInfo -> fptr_stego_image) != 8){
            printf("Error : Failed while extracting data\n");
            return e_failure;
        }
        decode_byte_from_lsb(&secret_char, buffer);
        fputc(secret_char, decInfo -> fptr_secret);
    }
    return e_success;

}

Status do_decoding(DecodeInfo *decInfo){
    if(open_decode_files(decInfo) == e_success){
        printf("Info : Open files passed successfully\n");
    }else{
        printf("Error : Open files fialed\n");
        return e_failure;
    }
    if(skip_bmp_header(decInfo -> fptr_stego_image) == e_success){
        printf("Info : Checking header files\n");
    }else{
        printf("Error : Checking header files failed\n");
        return e_failure;
    }

    if(decode_magic_string(MAGIC_STRING, decInfo) == e_success){
        printf("Info : Magic string verification passed\n");
    }else{
        printf("Error : Magic string verification failed\n");
        return e_failure;
    }

    if(decode_secret_file_extn_size(decInfo) == e_success){
        printf("Info : Decode secret file extension size passed\n");
    }else{
        printf("Error : Decode secret file extension size failed\n");
        return e_failure;
    }

    if(decode_secret_file_extn(decInfo) == e_success){
        printf("Info : Decode secret file extension passed\n");
    }else{
        printf("Error : Decode secret file extension failed\n");
        return e_failure;
    }

    if(decode_secret_file_size(decInfo) == e_success){
        printf("Info : Decode secret file size passed\n");
    }else{
        printf("Error : Decode secret file size failed\n");
        return e_failure;
    }

    if(decode_secret_file_data(decInfo) == e_success){
        printf("Info : Decode secret file data passed\n");
    }else{
        printf("Error : Decode secret file data failed\n");
        return e_failure;
    }

    fclose(decInfo -> fptr_stego_image);
    fclose(decInfo -> fptr_secret);

    return e_success;
}


