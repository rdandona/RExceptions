#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

using FileHandle_t = std::ifstream;
char* filename;
FileHandle_t OpenFile(const std::string &pathName)
{
   std::ifstream fileStream(pathName.c_str(), std::ios::in | std::ios::binary);
   if (fileStream.good())
      std::cout << "Opened file!" << std::endl;
   else
      throw std::runtime_error("Error opening file!");
   return std::move(fileStream);
}


unsigned int ReadFile(FileHandle_t &fileHandle, unsigned int nBytes, void *dst)
{
   int n = 0;
   std::ifstream file;
   file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
   try {
      file.open (filename);
       while (!file.eof()) file.get();
       file.close();
   }
   catch (std::ifstream::failure e) {
       std::cerr << "Exception opening/reading/closing file\n";
   }
       
   fileHandle.seekg (0, fileHandle.end);
   int length = fileHandle.tellg();
   fileHandle.seekg (0, fileHandle.beg);

   char * buffer = new char [length];
   std::cout << "Reading " << length << " characters ";
   fileHandle.read (buffer,length);


   if (fileHandle) {
         std::cout << "all characters read successfully.";
         } else {
            return -1;
         }
}


void CloseFile(FileHandle_t &fileHandle) {
   try{
      fileHandle.close();
   }
   catch(std::exception& e){
      std::cout<< "Error Closing File from the File Handler";
   }
}

void Usage(int argc, char** argv) {
   FILE *fp=fopen(argv[1],"r");
   auto fileHandle_t = OpenFile(argv[1]);
   //const auto& fileHandle = *fp;
   constexpr unsigned int N = 128;
   char buffer[N]; 
   ReadFile(fileHandle_t,128, buffer);
   auto nBytes = ReadFile(fileHandle_t, N, buffer);

}

int main (int argc, char** argv) {
   if (argc < 2) {
      Usage(argc, argv);
      return 1;
   }
    if (argc > 1) {
        std::cout << "argv[1] = " << argv[1] << std::endl; 
    } else {
        std::cout << "No file name entered. ";
        return -1;
    }
    std::ifstream infile(argv[1]); 
    int byte_traversed=0;
    if (infile.is_open() && infile.good()) {
        std::cout << "File is now open!\nContains:\n";
        std::string line = "";
        while (getline(infile, line)){
            byte_traversed=(int)infile.tellg();
            std::cout << line << '\n';
        } 
        std::cout<<"Size of the file is: "<<byte_traversed;
    } else {
        std::cout << "Failed to open file..";
    }

    //CloseFile(fp);   
    return 0;
}