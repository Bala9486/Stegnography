#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>

/*	Name		    :	BALAKUMAR H
	Batchid		    :	26001_081
	Project Name	    :	Lsb Stenography
	Project description :	LSB (Least Significant Bit) Steganography is a method of hiding secret information inside an image by changing the last 				bit of the image data. These changes are so small that the image looks the same to the human eye.

	The project has two parts:

	Encoding 	     :	 Hide a secret message or file inside an image.
	Decoding  	     :	 Extract the hidden message or file from the image.
				 This project uses file handling and bit manipulation in C to securely store and retrieve hidden data.*/

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
	if(argc<2){
		printf("Error missing arguments");
		return 0;
	}
	int op_type= check_operation_type(argv[1]);

	if(op_type = e_encode){
		printf("Encode started");
	}
	EncodeInfo encInfo;

	if(read_and_validate_encode_args(argv,&encInfo)==e_success){
		if(do_encoding(&encInfo) == e_success){
			printf("Encoding completed sucessfully");
		}
		else{
			printf("Encoding failed");
			return 0;
		}
	}



	else{
		printf("Error : read and validate arguments failed\n");
		return e_failure;

	}
}

OperationType check_operation_type(char *symbol)
{
	
	if(symbol == NULL){
		return e_unsupported;
	}
	if (strcmp(symbol,"-e") == 0){
		return e_encode;
	}else if(strcmp(symbol,"-d") ==0){
		return e_decode;
	}
	else{
		return e_unsupported;
	}
}
