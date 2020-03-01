#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

FILE *pFile;
char buffer[100];
char *filename;

/// Implement RException as an child class of std::runtime_exception 

/// TODO: Generalize RStatus for different types of return values, e.g. int, FILE *, bool (question: by templates or by inheritance)

template <class T>
class RStatus { 
   private:
      FILE *fValue;
     // FILE *fValue;
      //bool isChecked;
      bool isChecked;

/*    private: //It should be impossible to create RStatus objects on the heap (i.e. like ``new RStatus(...)``).  RStatus should only ever exist on the stack in order to catch errors close to where they appeared.
      void* operator new(size_t, ...) = delete;
      void* operator new[](size_t, ...) = delete;
*/
 // bool IsError() {
      bool IsError() {
         return fValue == nullptr;
      }

   public:
      RStatus() : fValue(nullptr), isChecked(false) {} //constr list
      explicit RStatus(FILE *val) : fValue(val), isChecked(false) {}
      ~RStatus() noexcept(false)
      {
          if (IsError() && !isChecked)  // must not throw if in uncaught_exception
             throw std::runtime_error("return value not checked");  // TODO: use RException
      }

   // Cast operator: if we use RStatus like an FILE *, it "behaves" like fValue
      operator const FILE *() {
          isChecked = true;
          return fValue;
      }
};
  /*  
   FILE *FileOpen(char *filename){
      if(!pFile)
         if (isChecked == true) { //FileOpen() fails and its return value is queried/checked.  Nothing should happen
            return 0;
         }
         else{
            cout << "Failed to open the file\n";	//FileOpen() fails (returns nullptr) and its return value (the RStatus object) is ignored.	
		      return nullptr;
         }
      else {
         if(isChecked == false)  //FileOpen() succeeds and it's return value is not checked.  Nothing should happen.
         {
            return 0;
         } 
      } 
   } 
   */
   

class RException : public std::runtime_error
{
public:    
   RException(const RException& exception)
        : std::runtime_error(exception.what()),
        exceptionId(exception.id()),
        innerException(NULL)
   {
        if (exception.inner() != NULL)
        {
            innerException = new RException(*exception.inner());
        }
        else
        {
            innerException = NULL;
        }
    } 
   RException(const std::string& _Message)
        : std::runtime_error(_Message),
            exceptionId(0),
        innerException(NULL)
        {}
    
   RException(const std::string& _Message, unsigned int id, RException* innerException)
        : std::runtime_error(_Message),
        exceptionId(id),
        innerException(new RException(*innerException)){}

    virtual ~RException()
    {
        delete innerException;
    }

    unsigned int id() const { return exceptionId; }
    const RException* inner() const { return innerException; }
   
private:
    unsigned int exceptionId;
    const RException* innerException;
};

void handleException(RException *ex)
{
    cout << "exception " << ex->id() << " - " << ex->what() << endl;
        const RException* innerException = ex->inner();
    int t = 1;
    while (innerException != NULL)
    {
        for (int i=0; i<t; ++i)
        {
            cout << "\t";
        }
        ++t;
        cout << "inner exception " << innerException->id() << " - " << innerException->what() << endl;
        innerException = innerException->inner();
    }
}

void throwRecursive(int temp)
{
    if (temp == 0)
    {
        throw runtime_error("std::runtime_error");
    }

    try
    {
        throwRecursive(--temp);
    }
    catch (RException &ex)
    {
        throw RException("RException", (temp+1), &ex);
    }
/* catch (exception &ex)
    {
        throw RException(ex.what(), (temp+1));
    } */
}

void RExceptionTest()
{
    try
    {
       //
        throwRecursive(3);
    }
    catch (RException &ex)
    {
        handleException(&ex);
    }
}


//RStatis<FILE *>OpenFile(char *filename){
FILE *OpenFile(char *filename){


   /// Everything ok: in this case return a valid file pointer (FILE *)
   /// Otherwise, error: return nullptr
	pFile = fopen(filename,"r");

// 1. return null ptr if not opened
	if (!pFile)
	{ 
		cout << "Failed to open the file\n";	
		return nullptr;
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


/**
1) FileOpen() fails (returns RStatus wrapped nullptr) and its return
value (the RStatus object) is ignored.  This scenario should fire an
exception as soon as the block in which FileOpen() was called ends.

  2) FileOpen() fails and its return value is queried/checked.  Nothing
else should happen.

  3) FileOpen() succeeds and it's return value is not checked.  Nothing
should happen.

  4) Same as 1), but right after the FileOpen() call another exception
is thrown.  In this case, the RStatus exception would fire while already
an exception is on its way.  That should be prevented.  RStatus can use
std::uncaught_exceptions in its destructor to do so:

  5) It should be impossible to create RStatus objects on the heap (i.e.
like ``new RStatus(...)``).  RStatus should only ever exist on the stack
in order to catch errors close to where they appeared.  To make RStatus
non-creatable on the heap, please overwrite the new operator.  See, for
instance,

 */
void Test001() {
   bool exceptionThrown = false;
   try {
     auto fp = OpenFile("no/such/file");
     // Failure occurred and fp is not checked: expect exception
   } catch (RException) {
      exceptionThrown = true;
   }

   // test code
   if (!exceptionThrown) {
      std::cerr << "Test001 Fail" << std::endl;
      return;
   }

   std::cout << "Test001 OK" << std::endl;
}

void Test002() {
   bool exceptionThrown = false;
   try {
     auto fp = OpenFile("no/such/file");
     // Failure occurred and fp is checked
   } catch (RException) {
      exceptionThrown = false;
   }

   // test code
   if (exceptionThrown) {
      std::cerr << "Test002 Fail" << std::endl;
      return;
   }

   std::cout << "Test002 OK" << std::endl;

}

void Test003() {
   bool exceptionThrown = false;
   try{
      auto fp = OpenFile (filename);
   } catch(RException) {
      exceptionThrown = false;
   }
   if (exceptionThrown) {
      std::cerr << "Test003 Fail" << std::endl;
      return;
   }

   std::cout << "Test003 OK" << std::endl;
}


class AnotherException : public std::runtime_error {
 
};
/* 
void Test004() {
   try {
      auto fp = OpenFile("no/such/file");
      // fp wraps null because the call failed
      throw AnotherExcpetion();
   } catch (AnotherException) {

   }

   std::cout << "Test004 OK" << std::endl;
}

void Test005() {
   auto status = new RStatus<int, -1>(0);  // <-- This should not compile (heap allocated)
   RStatus<int, -1> status(0);  // <-- This, however, should compile (stack allocated)
}
*/
/* 
void Test004{
  // struct Test{
      int count = std::uncaught_exceptions();
      //~Test() 
      //{
      std::cout << (count == std::uncaught_exceptions())
     // }
}*/

  /*// Test f;
      try {
         Test f;
         std::cout << "Exception thrown\n";
         throw std::runtime_error("test exception");
      } 
      catch (const std::exception& e) {
         std::cout << "Exception caught: " << e.what() << '\n';
      }*/
//};

/* 
void Test005() {
      void* operator new(size_t)= delete;    
      void* operator new(size_t, void*)= delete;   
      void* operator new[](size_t)= delete;  
      void* operator new[](size_t, void*)= delete;
}
*/
/* class Test005 {
    private:
        Test005();
        Test005(Test005& );
    public:
        static Test005 GenerateInstance() { 
            Test005 a ; 
            return a; 
        }
   
};
*/
int main(int argc, char** argv)
{
   Test001();
   Test002();
   Test003();
//   Test004();
//   Test005();
   
   if (argc<2) {
      Usage();
   }
 auto fp = OpenFile(argv[1]);
//   FILE* fp=OpenFile(argv[1]);

if (fp == nullptr) {
       std::cerr << "There was an error" << std::endl;
       return 1;
   }


   //if (fp == nullptr) {
      /// Error...
      if((pFile = fopen(argv[1],"r")) == 0)
         {
    	    cout<<endl<<__FUNCTION__<<" : Error encountered in opening file\n"; 
    	    exit(-1);
	  }   

/*
   // General idea: I/O functions provide return values indicating I/O errors.  They throw and excpetion if, and only if, the return value is not checked
\
   auto fp = OpenFile(argv[1]);
   // fp is of type RStatus

   // case 1: do nothing
   return 0;

   // case 2:
   if (fp == nullptr) {
       std::cerr << "There was an error" << std::endl;
       return 1;
   }
   
   
   //FILE* fp=OpenFile(argv[1]);
   //if (fp == nullptr) {
      /// Error...
   
   
   
   /*   if((pFile = fopen(argv[1],"r")) == 0)
         {
    	    cout<<endl<<__FUNCTION__<<" : Error encountered in opening file\n"; 
    	    exit(-1);
	  }   
 */

   int x=ReadFile(fp,100, buffer);
   cout<<endl<<"The number of bytes in file is :"<<x<<endl;
  

   CloseFile(fp);
return 0;	
}




