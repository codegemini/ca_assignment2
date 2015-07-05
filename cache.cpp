#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <math.h> 
#include <string>
#include <fstream>
#include <string>
#include <ostream>
#include <cstring>

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
string binaryToHex(string bin){
  string hex;
  int len = bin.length();
  int rem;
  if(rem = (len % 4) != 0){
     switch(rem){
     	case 1: bin = "000" + bin; 
              len += 3;
     	break;
     	case 2: bin = "00" + bin;
              len += 2; 
     	break;
     	case 3: bin = "0" + bin;
              len += 1;
     	break;
     }  
  }

  for(int i=len-4;i>=0;i-=4){
     switch(atoi((bin.substr(i,4)).c_str())) {

        case 0: hex += "0";  break;
        case 1: hex += "1";  break;
        case 10: hex += "2";  break; 
        case 11: hex += "3";  break;
        case 100: hex += "4";  break; 
        case 101: hex += "5";  break;
        case 110: hex += "6";  break;
        case 111: hex += "7";  break;
        case 1000: hex += "8";  break;
        case 1001: hex += "9";  break;
        case 1010: hex += "a";  break;
        case 1011: hex += "b";  break;
        case 1100: hex += "c";  break;
        case 1101: hex += "d";  break;
        case 1110: hex += "e";  break;
        case 1111: hex += "f";  break;
        default : return "Invalid Address"; 
        
      }
  }
  if (hex[0] == '0')
  	 hex = "0000000"; 
  string temp;
  for(int i=hex.length()-1;i>=0;i--){
     temp += hex[i];
  }
  hex = temp;
  return hex;
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
  int numMiss =0;
  int numHit =0;

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
  for(int i=0;i<numSets;i++){
      for(int j=0;j<ways;j++){
          blocks[i][j].cachedBytes = new string [2];//new string [blockSize];
          blocks[i][j].cachedBytes[0] = "0000000";
          blocks[i][j].cachedBytes[1] = "C";
        }
  }
   
   /*Read the trace file*/
  string tag;
  int offset;
  int index;
  string status;

  string address, instruct;
  ifstream infile(filename.c_str());
  ofstream outfile("results.txt");
        
  if(outfile.is_open()){ 
    
    while(infile >> address >> instruct){
      splitAddress(address, &tag, &offset, &index);

      /*Output current state of cache*/

      /*
      for(int i=0;i<numSets;i++){
        for(int y=0;y<ways;y++){
          outfile<<"[Set "<<i<<": {Way "<<y<<":"<<binaryToHex(blocks[i][y].cachedBytes[0])<<", "<<blocks[i][y].cachedBytes[1] <<"} LRU: 0] ";
        }
      }

      */
      
      for(int w=0;w<ways;w++){
          if(blocks[index][w].cachedBytes[0] == tag){
            status = "hit";
            numHit++;
            if(strcmp(instruct.c_str(),"S")==0){
                blocks[index][w].cachedBytes[1] = "D";
            }
          
          }
          else{ 
            status = "miss";
            blocks[index][w].cachedBytes[0] = tag;
            numMiss++;
            if(strcmp(instruct.c_str(),"L")==0){
                blocks[index][w].cachedBytes[1] = "C";
            }
            else{
                blocks[index][w].cachedBytes[1] = "D";  // assuming we are making a write-allocate cache, since we are already bringing the missed data to cache
            }
          }
      }

   // outfile<< " | "<< address << "\t" << instruct << "\t" << status << "\n"; //Not handling dirty/clean right now 
    }
    infile.close();
    outfile.close();
  }

  cout << "Total number of memory accesses: " << numMiss+numHit <<endl;
  cout << "Total number of Hits: " << numHit <<endl;
  cout << "Total number of Misses: " << numMiss <<endl;

      
    
}