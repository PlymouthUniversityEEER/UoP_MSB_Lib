#include "mbed.h"
#include "MSB_Config.h"
#include "SDCard.h"


#include "mbed.h"
#include "MSB_Config.h"

#include "SDBlockDevice.h"
#include "FATFileSystem.h"





// Function: SDCard class constructor
// Initialises the SDBlockDevice object
SDCard::SDCard(PinName mosi,PinName miso,PinName sclk,PinName cs) : sd(mosi,miso,sclk,cs){

}

// Function: SDCard class write_test
// Write's test data to a file
int SDCard::write_test(){
    printf("Initialise and write to a file\n");
    int err;
    err=sd.init();
    if ( 0 != err) {
        printf("Init failed %d\n",err);
        return -1;
    }

    FATFileSystem fs("sd", &sd);
    FILE *fp = fopen("/sd/test.txt","w");
    if(fp == NULL) {
        error("Could not open file for write\n");
        sd.deinit();
        return -1;
    } else {
        //Put some text in the file...
        fprintf(fp, "Martin Says Hi!\n");
        fprintf(fp, "Andy Says Bye!\n");
        //Tidy up here
        fclose(fp);
        printf("SD Write done...\n");
        sd.deinit();
        return 0;
    }
}

// Function: SDCard class read_test
// Reads data from the test file and prints to terminal
int SDCard::read_test(){
    printf("Initialise and read from a file\n");

    // call the SDBlockDevice instance initialisation method.
    if ( 0 != sd.init()) {
        printf("Init failed \n");
        return -1;
    }
    
    FATFileSystem fs("sd", &sd);
    FILE *fp = fopen("/sd/test.txt","r");
    if(fp == NULL) {
        error("Could not open or find file for read\n");
        sd.deinit();
        return -1;
    } else {
        //Check for text in the file...
        char buff[64]; buff[63] = 0;
        while (!feof(fp)) {
            fgets(buff, 63, fp);
            printf("%s\n", buff);
        }
        //Tidy up here
        fclose(fp);
        printf("SD Write done...\n");
        sd.deinit();
        return 0;
    }
}

// Function: SDCard class write_file
// Write data from an array to a file
int SDCard::write_file(char* filename, char* text_to_write, bool print_debug ){
    if(print_debug){
        printf("Initialise and write to a file\n");
    }
    int err;
    err=sd.init();
    if ( 0 != err) {
        if(print_debug){
            printf("Init failed %d\n",err);
        }
        return -1;
    }

    FATFileSystem fs("sd", &sd);
    char file_path[128];
    sprintf(file_path,"/sd/%s",filename);
    FILE *fp = fopen(file_path,"w");
    if(fp == NULL) {
        error("Could not open file for write\n");
        sd.deinit();
        return -1;
    } else {
        //Put some text in the file...
        fprintf(fp, text_to_write);
        fclose(fp);
        if(print_debug){
            printf("SD Write done...\n");
        }
        sd.deinit();
        return 0;
    }
}
// Function: SDCard class print_file
// Reads the data from a file and prints it to the terminal

int SDCard::print_file(char* filename, bool print_debug ){
    if(print_debug){
        printf("Initialise and read from a file\n");
    }

    // call the SDBlockDevice instance initialisation method.
    if ( 0 != sd.init()) {
        if(print_debug){
            printf("Init failed \n");
        }
        return -1;
    }

    FATFileSystem fs("sd", &sd);
    char file_path[128];
    sprintf(file_path,"/sd/%s",filename);
    FILE *fp = fopen(file_path,"r");
    if(fp == NULL) {
        error("Could not open or find file for read\n");
        sd.deinit();
        return -1;
    } else {
        //Check for text in the file...
        char buff[128]; buff[127] = 0;
        while (!feof(fp)) {
            fgets(buff, 127, fp);
            if(print_debug){
                printf("%s\n", buff);
            }
        }
        //Tidy up here
        fclose(fp);
        if(print_debug){
            printf("\nSD Print done...\n");
        }
        sd.deinit();
        return 0;
    }
}
// Function: SDCard class copy_file
// Reads data from a file and copies it to an array
int SDCard::copy_file(char* filename, char* dest, int dest_size,bool print_debug){
    if(print_debug){
        printf("Initialise and copy file to array\n");
    }

    // call the SDBlockDevice instance initialisation method.
    if ( 0 != sd.init()) {
        if(print_debug){
            printf("Init failed \n");
        }
        return -1;
    }
    
    FATFileSystem fs("sd", &sd);
    char file_path[128];
    sprintf(file_path,"/sd/%s",filename);
    FILE *fp = fopen(file_path,"r");
    if(fp == NULL) {
        error("Could not open or find file for read\n");
        sd.deinit();
        return -1;
    } else {
        //Check for text in the file...
        char buff[dest_size];
        int idx=0;
        while (!feof(fp)) {
            fgets(dest, dest_size, fp);
        }

        printf("Datafrom file %s:\n%s\n", file_path,dest);

        //Tidy up here
        fclose(fp);
        if(print_debug){
            printf("SD Copy done...\n");
        }
        sd.deinit();
        return 0;
    }
}

