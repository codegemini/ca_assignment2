#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <math.h> 
#include <string>

using namespace std;

#define ADDRESS_SIZE = 32;
int numSets =0;
int numIndexBits = 0;
int numOffsetBits = 0;
int numTagBits =0;


/* Usage */ 
void usage(void){
   printf("Usage:\n");
   printf(" -s <size of the cache in KB>\n");
   printf(" -b <size of the block in Bytes>\n");
   printf(" -f <trace file - containing memory accesses i.e. Loads and Stores>\n");
}

void calculateCacheValues(int s, int b, int w){
    numSets= s/(b*w);
    numIndexBits = log2(numSets);
    numOffsetBits = log2(b);
    numTagBits = ADDRESS_SIZE - (numIndexBits+numOffsetBits);
}

string hexToBinary(string hex){

  string bin ;
  for(int i=0;i<hex.length(); i++){
      switch(hex[i]){
        case '0': bin += "0000";
        case '1': bin += "0001";
        case '2': bin += "0010";
        case '3': bin += "0011";
        case '4': bin += "0100";
        case '5': bin += "0101";
        case '6': bin += "0110";
        case '7': bin += "0111";
        case '8': bin += "1000";
        case '9': bin += "1001";
        case 'A': bin += "1010";
        case 'B': bin += "1011";
        case 'C': bin += "1100";
        case 'D': bin += "1101";
        case 'E': bin += "1110";
        case 'F': bin += "1111";
        default : return "Invalid Address"; 
        
      }
  }

  return bin;

}


void splitAddress(string address, string &tag, int &offset, int &index ){



}




int main(int argc, char *argv[]){
  int c = 0;	
  opterr = 0;
  


  while ((c = getopt (argc, argv, "s:b:f:")) != -1)
    switch (c)
      {
      case 's':
        break;
      case 'b':
        break;
      case 'f':
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
    if(c == -1){
        usage();
    }


    calculateCacheValues(s,b,w);










}