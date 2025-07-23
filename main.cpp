
#include <string>
#include "module.h"



//Argv = testbench.exe INPUTFILE OUTPUTFILE ENCODING
int main(int argc, char **argv) {
    std::string fileName, encoding;
    if(argc<3) return 0;
    fileName = argv[1];
    encoding = argc==4 ? argv[3] : "UTF8";

    Module test = Module(fileName,encoding);

    FILE *outputFile = fopen(argv[2],"w");
    if(outputFile) {
        //test.CreateTestBench(outputFile);
        test.CreateInstance(outputFile);
        fclose(outputFile);
    }
    return 0;

}
