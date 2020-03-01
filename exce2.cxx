#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

FILE *pFile;
char buffer[100];

FILE *OpenFile(char* filename){

   /// Everything ok: in this case return a valid file pointer (FILE *)
   /// Otherwise, error: return nullptr
	pFile = fopen(filename,"r");

// 1. return null ptr if not opened
	if (!pFile)
	{ 
		cout << "Failed to open the file\n";		
		return 0;
	}

	else
	{		
	    	cout<<"Successfully opened the file\n";
	    	return pFile;
	}
}

//void ReadFile(FILE* pFile, unsigned int nBytes)

   /// Returns -1 on error  
//1. change to int 2. give no of bytes read else -1 if error                                       
//     		char buffer[100];
      

    
int ReadFile(FILE* pFILE, unsigned int nBytes, void *dst)
{
   if (pFile == 0) {
	return -1;
	}

   else if(feof(pFile)) {
	cout<<endl<<__FUNCTION__<<" : File Opened but no text found in file\n"; 
	return -1;
	}
    
   else{
            //FILE *pFILE = fopen(filename, "r");
            if (pFILE)
            {
             string contents;
             fseek(pFILE, 0, SEEK_END);
             contents.resize(ftell(pFILE));
             rewind(pFILE);
             fread(&contents[0], 1, contents.size(), pFILE);
   			//test print for contents
             //delete next when program stable
             cout<<"The content"<<endl<<contents<<endl<<"END PRINT";


             //code for file size
             fseek(pFILE, 0, SEEK_END);
             int size_file=(ftell(pFILE));
  			 cout<<endl<<"The size of the file is "<<size_file;
             return size_file;
             //close(pFILE);
             //return -1;
            }
    } 
}


//return number of bytes read
//   	}

	bool CloseFile(FILE *fp){  
//root_exce2.cxx: In function ‘bool CloseFile(FILE*)’:root_exce2.cxx:54:5: error: ‘else’ without a previous ‘if’else

//1. make bool
	   if(fclose(fp)==0) {
              cout<<"File Handler successfully closed\n";
		return true;
           }
	   else { 
              cout<<endl<<__FUNCTION__<<" : Error Closing File from the File Handler\n";
	   return false;
	   }
}

int Usage() {
      cout<<"Please enter filename from root directory int the command line interface itself";
      cout<<endl<<"eg. >> ./a.out /home/user/Downloads/abc.txt"<<endl;
      cout<<"Exiting the program!!"<<endl;
      return 1;
   }
int main(int argc, char** argv)
{
   if (argc<2) {
      Usage();
   }

   FILE* fp=OpenFile(argv[1]);
   //if (fp == nullptr) {
      /// Error...
      if((pFile = fopen(argv[1],"r")) == 0)
         {
    	    cout<<endl<<__FUNCTION__<<" : Error encountered in opening file\n"; 
    	    exit(-1);
	  }   
   int x=ReadFile(fp,100, buffer);
   cout<<endl<<"The number of bytes in file is :"<<x<<endl;
  

   CloseFile(fp);
return 0;	
}

