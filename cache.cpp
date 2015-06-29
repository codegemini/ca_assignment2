#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <ostream>

using namespace std;

/* Usage */ 
void usage(void){
   printf("Usage:\n");
   printf(" -s <size of the cache in KB>\n");
   printf(" -b <size of the block in Bytes>\n");
   printf(" -f <trace file - containing memory accesses i.e. Loads and Stores>\n");
}

/* Block Structure */
typedef struct _str_block {
   string chacheLine[];  
} cacheBlock;

int main(int argc, char *argv[]){
  int c = -2;	// Default value if no arguments are passed
  opterr = 0;
  unsigned int cacheSize;
  unsigned int blockSize;
  unsigned int ways = 1; // Default number of ways
  string filename;

  while ((c = getopt (argc, argv, "s:b:w:f:")) != -1)
    switch (c)
      {
      case 's':
        cacheSize = atoi(optarg);
        break;
      case 'b':
        blockSize = atoi(optarg);
        break;
      case 'w':
        ways = atoi(optarg);
        break;
      case 'f':
        filename = optarg;
        break;
      case '?':
        if (optopt == 's' || optopt == 'b'){
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          usage();
       }
        else if (isprint (optopt)){
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          usage(); 
       }
        else{
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
       }
        return 1;
      default:
        usage();
      }
    if(c == -2){ // if no arguments are passed display usage
        usage();
        return 1;
    }

//Reading the trace file
string address, instruct;
ifstream infile(filename.c_str());

while(infile >> address >> instruct){
    
}

}