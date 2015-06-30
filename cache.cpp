#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <math.h> 
#include <string>
#include <fstream>
#include <string>
#include <ostream>

using namespace std;


const int ADDRESS_SIZE = 32;
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

/* Block Structure */
typedef struct _str_block {
   string *cachedBytes;  
} cacheBlock;

void calculateCacheValues(int s, int b, int w){
    numSets= s/(b*w);
    numIndexBits = log2(numSets);
    numOffsetBits = log2(b);
    numTagBits = ADDRESS_SIZE - (numIndexBits + numOffsetBits);
    
}

string hexToBinary(string hex){

  string bin ;
  for(int i=0;i<hex.length(); i++){
      switch(hex[i]){
        case '0': bin += "0000";  break;
        case '1': bin += "0001";  break;
        case '2': bin += "0010";  break; 
        case '3': bin += "0011";  break;
        case '4': bin += "0100";  break; 
        case '5': bin += "0101";  break;
        case '6': bin += "0110";  break;
        case '7': bin += "0111";  break;
        case '8': bin += "1000";  break;
        case '9': bin += "1001";  break;
        case 'a': bin += "1010";  break;
        case 'b': bin += "1011";  break;
        case 'c': bin += "1100";  break;
        case 'd': bin += "1101";  break;
        case 'e': bin += "1110";  break;
        case 'f': bin += "1111";  break;
        default : return "Invalid Address"; 
        
      }
  }

  return bin;

}
void splitAddress(string address, string *tag, int *offset, int *index ){

  string addressInBinary =  hexToBinary(address);

  *tag = addressInBinary.substr(0,numTagBits);

  string s_index = addressInBinary.substr(numTagBits,numIndexBits);
  char *ptr;
  *index = (int) strtol(s_index.c_str(), &ptr,2);

  string s_offset = addressInBinary.substr(numTagBits+numIndexBits, numOffsetBits);
  *offset = (int) strtol(s_offset.c_str(), &ptr,2);

}
/* Helper Fucntions */
bool fexists(const char *filename)
{
  ifstream ifile(filename);
  if(ifile){
  	ifile.close();
    return true; 
  }else{
  	ifile.close();
  	return false;
  }
}


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

 
  // string address, instruct;
  // ifstream infile(filename.c_str());

  // while(infile >> address >> instruct){
    
  // }

	if (!fexists(filename.c_str())){
		cout<<"Error: The trace file "<<filename<<" does not exist! \n";
		return 1;
	}else{
        ifstream infile(filename.c_str());
        if(infile.peek() == EOF)
        {
        	cout<<"Error: The trace file is empty! Try Again \n";  	
        	infile.close();
            return 1;
        }else{
        	infile.close();
        }
        
	} 

   /*Sets the cache properties by setting global variables*/ 
   calculateCacheValues(cacheSize,blockSize,ways);
   
    /*Initialize the blocks using the cache properties*/
    cacheBlock **blocks = new cacheBlock* [numSets];
    for(int i=0;i<numSets;i++)
        blocks[i] = new cacheBlock[ways]; 
   
    /*Initilialize each block's cache bytes*/
    for(int i=0;i<numSets;i++)
    	  for(int j=0;j<ways;j++)
    	  	  blocks[i][j].cachedBytes = new string [1];//new string [blockSize];

	

	


   /*Read the trace file*/

      string tag;
      int offset;
      int index;

	   string address, instruct;
	   ifstream infile(filename.c_str());
       ofstream outfile("results.txt");

	   while(infile >> address >> instruct){
	     splitAddress(address, &tag, &offset, &index);
	     for(int w=0;w<ways;w++){
	         if(blocks[index][w].cachedBytes[0] == tag){
	         	cout<<"hit";
	         }
	         else{	
	         	cout<<"miss";
	         	blocks[index][w].cachedBytes[0] = tag;
             }
         }    
	   }
      
	  




	  /*Output current state of cache*/
      for(int i=0;i<numSets;i++){
      	for(int y=0;y<ways;y++){
      		outfile<<"[Set "<<i<<": {Way "<<y<<":"<<blocks[i][y].cachedBytes[0]<<", C} LRU: 0] ";
      	}
      }
	  outfile<< " | "<< address << "\t" << instruct << "\t" << status << "\n"; //Not handling dirty/clean right now 
	  }
	  infile.close();
	  outfile.close();
	}
      
	  

}