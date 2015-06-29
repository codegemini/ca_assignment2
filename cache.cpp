#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>

/* Usage */ 
void usage(void){
   printf("Usage:\n");
   printf(" -s <size of the cache in KB>\n");
   printf(" -b <size of the block in Bytes>\n");
   printf(" -f <trace file - containing memory accesses i.e. Loads and Stores>\n");
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

}