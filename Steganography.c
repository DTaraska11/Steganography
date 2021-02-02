#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <time.h>

//Dominic Taraska


#define HEADER_SIZE 54
#define SIZE_BEGIN 2
#define WIDTH_BEGIN 18
#define HEIGHT_BEGIN 22

typedef struct Bitmaps{

    unsigned long size;
    int width;
    int height;
    char* data;
}Bitmap;


char* read_file(unsigned long *size, char *file_name);
int file_write(unsigned long size, char *output, char *file_name2);
void make_rand_key(int key_length, char* key);
void encrypt(char* clear, char* rand_key, char* cipher);
void decrypt(char* clear, char* rand_key, char* cipher);
Bitmap read_bmp(char* file_name);
unsigned int write_bmp(Bitmap m, char* filename);
void encode(char* filename, char* bitFileName);
void decode();


int main(){

    //Used a while loop for main menu. Keeps user in while loop until they input a 1 , 2 , or 3.
    int choice = 0;
    while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 ){
        printf("Encrypt a file: 1 \n Decrypt a file: 2 \n Encode a bitmap: 3 \n Decode a bitmap: 4 \n Exit: 5 \n");
        printf("Enter a choice:\n");
        scanf("%d", &choice);
    }

    //switch statement to execute user's choice
    //case 1 is for encryption
    //case 2 is for decryption
    //case 3 is to encode
    //case 4 is to decode
    //case 5 is to exit
    switch(choice){

    case 1 :
            printf("Enter the name of the clear text file you want to read from: \n");
            char clear_file_name[100];
            scanf("%s", clear_file_name);
            char* rand_key_name = "key.txt";
            printf("Randomized key file will be saved as key.txt. Please make sure there are no other files with this name. \n");
            char* cipher_file_name = "cipher.txt";
            printf("Cipher file will be saved as cipher.txt. Please make sure there are no other files with this name. \n");
            encrypt(clear_file_name, rand_key_name, cipher_file_name);
            printf("Successfully encrypted! \n");
            break;

    case 2 : ;
            char* cipher_file_name2 = "cipher.txt";
            printf("Found cipher.txt \n");
            char* rand_key_name2 = "key.txt";
            printf("Found key.txt \n");
            char* clear_file_name2 = "clear.txt";
            printf("Decrypted clear file will be saved as clear.txt. Please make sure there are no other files with this name. \n");
            decrypt(clear_file_name2, rand_key_name2, cipher_file_name2);
            printf("Decryption successful! \n");
            break;

    case 3:
            printf("Enter the name of the message file: ");
            char message_file[100];
            scanf("%s", message_file);
            printf("Enter the name of the bit map file: ");
            char bmp_file[100];
            scanf("%s", bmp_file);
            encode(message_file, bmp_file);
            printf("File saved as encoded.bmp");
            break;

    case 4:
            decode();
            printf("File saved as decoded.txt");
            break;

    case 5:
            exit(0);
            break;
    }
    return 0;
}
//read from a file function.
char* read_file(unsigned long *size, char *file_name){

    FILE *file;
    file = fopen(file_name, "rb");
    if (file == NULL){
        printf("Cannot open file \n");

    }
    fseek(file,0,SEEK_END);
    *size = (unsigned long)ftell(file);
    rewind(file);
    char* string = (char *)malloc((int)size+1);
        if(string == NULL){

            printf("memory alloc failed");
        }
    fread(string, (unsigned long)size, 1, file);
    fclose(file);
    return string;
}
//write to a file function
int file_write(unsigned long size, char *output, char *file_name2){

    FILE *file;
    file = fopen(file_name2, "wb");
    if(file == NULL){
        printf("Cannot open file");
    }
    size = fwrite(output, 1, strlen(output), file);

    fclose(file);
    free(output);

    return size;
}
//function to make the random key
void make_rand_key(int key_length, char* key){

    srand(time(NULL));

    int x;
    for(x = 0; x < key_length; x++){
        key[x] = ((char)rand() % (255 + 1 - 0) + 0);

    }

}
//encryption function
void encrypt(char* clear, char* rand_key, char* cipher){

        unsigned long size;
        char* contents = read_file(&size, clear);

        char* rand_key_text;

        rand_key_text = (char*)malloc(size+1);
        if(rand_key_text == NULL){

            printf("memory alloc failed");
        }
        make_rand_key(size, rand_key_text);
        char* cipher_text;
        cipher_text = (char*)malloc(size+1);
        if(cipher_text == NULL){

            printf("memory alloc failed");
        }
        int x = 0;
        for(x; x < size ; x++){
            cipher_text[x] = (contents[x] ^ rand_key_text[x]);
        }

        unsigned long size2;
        unsigned long size3;

        file_write(size2, cipher_text , cipher);
        file_write(size3, rand_key_text , rand_key);
        //freeing up the memory allocated for 'rand_key_text' and 'cipher_text' since they are no longer needed.
        free(rand_key_text);
        free(cipher_text);
}
//decryption function
void decrypt(char* clear, char* rand_key, char* cipher){
        unsigned long size;
        char* rand_key_text = read_file(&size, rand_key);
        char* cipher_text = read_file(&size, cipher);
        char* clear_text;
        clear_text = (char*)malloc(size+1);
        if(clear_text == NULL){
            printf("memory alloc failed");
        }
        int x;
        for(x = 0; x < size ; x++){
            clear_text[x] = (rand_key_text[x] ^ cipher_text[x]);
        }
        unsigned long size2;
        file_write(size2, clear_text , clear);
}
//There's an issue somewhere in my read_bmp function where it doesn't correctly copy the contents of the bitmapfile to the allocated char array. Haven't been able to figure it out. I believe the issue lies somewhere between my marked lines. But it gets the size just fine.
Bitmap read_bmp(char* file_name){

    FILE *file;
    file = fopen(file_name, "rb");
    if (file == NULL){
        printf("Cannot open file \n");

    }
    fseek(file,0,SEEK_END);
    unsigned long size = (unsigned long)ftell(file);
    rewind(file);
    //-----------------------------------------------------------------------------------------------

    char* contents = (char*)malloc(size+1);
    if(contents == NULL){
            printf("memory alloc failed");
   }
    fread(contents, (unsigned long)size, 1, file);
    Bitmap m = {0, 0, 0 , contents};

//-------------------------------------------------------------------------------------------------------

    free(contents);
    fseek(file, SIZE_BEGIN, SEEK_SET);
    fread(&m.size, 4, 1, file);
    fseek(file, WIDTH_BEGIN, SEEK_SET);
    fread(&m.width, 4, 1, file);
    fseek(file, HEIGHT_BEGIN, SEEK_SET);
    fread(&m.height, 4, 1, file);
    //m.size = *((int*)&(m.data[SIZE_BEGIN]));
    //m.width = *((int*)&(m.data[WIDTH_BEGIN]));
    //m.height = *((int *)&m.data[HEIGHT_BEGIN]);
    rewind(file);
    fclose(file);
    return m;
}
//create the bitmap file
unsigned int write_bmp(Bitmap m, char* filename){

    FILE *file;
    file = fopen(filename, "wb");
    if(file == NULL){
        printf("Cannot open file");
    }
    //m.size = *((int*)&m.data[SIZE_BEGIN]);
    //m.width = *((int*)&m.data[WIDTH_BEGIN]);
    //m.height = *((int*)&m.data[HEIGHT_BEGIN]);
    int bytesWritten;
    //fwrite(m.size, 4, 1, file);
    //fwrite(m.width, 4, 1, file);
    //fwrite(m.height, 4, 1, file);
    bytesWritten = fwrite(m.data, m.size, 1, file);
    fclose(file);
    return bytesWritten;

}
//encode function
void encode(char* filename, char* bitFileName){
     Bitmap m = read_bmp(bitFileName);

     unsigned long *size = (unsigned long*)malloc(4);
     if(size == NULL){
            printf("memory alloc failed");
        }

     char* message = read_file(size, filename);
     printf("\n message is: %s \n", message);
     printf("size is %d", *size);
     //Write message length in first 32 chars of bmp
     int bmp_cnt;
     unsigned long msg_size_2 = (unsigned long)size;
     for(bmp_cnt=HEADER_SIZE; bmp_cnt<HEADER_SIZE+32; bmp_cnt++){
            //Flip LSB based on a bit in message size
            if(msg_size_2 & INT_MIN)
            // msg_size_2 is message size
            m.data[bmp_cnt] |= 1;  //Flip LSB to 1
            else
            // m is a Bitmap struct and data file data
            m.data[bmp_cnt] &= 254;//Flip LSB to 0
        msg_size_2 <<= 1;
        }
        int x;
        int j;

        //nested for loop that iterates thru every bit of every char in the desired message

        for(x=0; x<strlen(message); x++){

            for(j = 0; j < 8; j++){
                if (message[x] & 128) { //if the leading (leftmost) bit of the current character is 1, flip the last bit of current byte in m.data to 1
                    m.data[j+87] |= 1;
                }
                else {
                    m.data[j+87] &= 254; //if the leading bit of the current character is 0, flip the last bit of the current byte in m.data to 0
                }

                message[x] << 1; //left shift the current char in message in order to test the next bit of the char
            }
        }
        write_bmp(m, "encoded.bmp");//create the bitmap file
        printf("\n data: %s", m.data);

}

//decode function
void decode(){
    Bitmap m = read_bmp("encoded.bmp");
    int x;
    int message_length = 0;
    //for loop to retrieve message length
    for(x = HEADER_SIZE; x<HEADER_SIZE+32; x++){
        if(m.data[x] & 1){
            message_length++;
        }
    }
    char* message = (char*)malloc(message_length+1);
    if(message == NULL){
            printf("memory alloc failed");
    }
    int y;
    int j;
    //nested for loop to retrieve message
    for(y=HEADER_SIZE+32; y<message_length; y++){

        for(j = 0; j<8; j++){
            if(m.data[j] & 1){//if least significant bit is 1, flip last bit of current char of message to 1
                message[y] |= 1;
            }
            else{ //if least significant bit is 0,
                message[y] &= 254;
            }
            message[y] << 1; //left shift last bit of current char of message by 1, once the inner for loop has iterated 7 times, the desired char's bits will be in proper order
        }

    }

    unsigned long size;
    file_write(size, message, "decoded.txt"); //creating the txt file containing the decoded message
    free(message);


}

