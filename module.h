//
// Created by Aweso on 12/28/2024.
//

#ifndef TESTBENCH_MODULE_H
#define TESTBENCH_MODULE_H

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <format>

#define LINEBUFFERSIZE 1024
#define NAMEBUFFERSIZE 128

#define APPEND_END(_buffer,_string) \
_string += _buffer; \
memset(_buffer,0,sizeof(_buffer)); \

enum class Direction{
    Input = 0,
    Output = 1,
    Inout = 2
};
const char* ToString(Direction d) {
    switch (d) {
        case Direction::Input: return "Input";
        case Direction::Output: return "Output";
        case Direction::Inout: return "Inout";
        default: return "Unknown";
    }
}



struct Port{
    int width;
    Direction dir;
    std::string name, comment;
    std::string parameterized_width;

    Port(int w, Direction d, std::string &n, std::string &c, std::string &p_width)
            : width(w), dir(d), name(n), comment(c), parameterized_width(p_width) {}

    Port(){
        width = 0; dir = Direction::Inout; name = ""; comment = ""; parameterized_width = "";
    }

    void Print() const {
        printf("Port: %s, Direction: %s, Width: %d, Comment: %s\n", name.c_str(),ToString(dir), width, comment.c_str());
    }

    //comparison operator
    bool operator==(const Port &other) const {
        return width == other.width && dir == other.dir && name == other.name && comment == other.comment;
    }

    [[nodiscard]] std::string GetString() const {
        std::string output;
        const int bufferSize = 1024;
        char buffer[bufferSize] = {0};
        snprintf(buffer,bufferSize,"%s[%d:0] %s //%s",
                 ToString(dir), width-1, name.c_str(), comment.c_str());
        output += buffer;
        return output;
    }
};

struct Parameter{
    std::string name, comment;
    int default_value;

    Parameter(std::string &n, int d, std::string &c )
        : name(n), default_value(d), comment(c) {}

    Parameter(char* n, int d, char* c )
            : name(n), default_value(d), comment(c) {}


    [[nodiscard]] std::string GetString() const {
        std::string output;
        const int bufferSize = 1024;
        char buffer[bufferSize] = {0};
        snprintf(buffer,bufferSize,".%s(%s) = %d //%s", name.c_str(), default_value, comment.c_str());
        output += buffer;
        return output;
    }

    void Print() const {
        printf("Parameter: %s, Default Value: %d, Comment: %s\n", name.c_str(), default_value, comment.c_str());
    }



};

enum Encoding{
    UTF8 = 0,
    UTF16LE = 1
};

class Module {
    std::vector<Port> m_Ports;
    std::string m_name;
    std::vector<Parameter> m_parameters;
public:
    void Print(){
        printf("Name: %s\n",m_name.c_str());
        for(auto &p : m_Ports){
            p.Print();
        }
    }

    void CreateTestBench(FILE *fstream){
        std::vector<Port*> inputs, outputs, inouts;
        bool clock = false;
        for(auto &p: m_Ports){
            switch(p.dir) {
                case Direction::Input:
                    inputs.push_back(&p);
                    break;
                case Direction::Output:
                    outputs.push_back(&p);
                    break;
                case Direction::Inout:
                    inouts.push_back(&p);
                    break;
            }
        }

        std::sort(inputs.begin(), inputs.end(),[](Port *a, Port *b) {
            return a->width < b->width;
        });
        std::sort(outputs.begin(), outputs.end(),[](Port *a, Port *b) {
            return a->width < b->width;
        });
        std::sort(inouts.begin(), inouts.end(),[](Port *a, Port *b) {
            return a->width < b->width;
        });

        int lastWidth = 0;
        for(auto &i : inputs){
            bool firstOnLine = false;
            if(i->width!=lastWidth){
                firstOnLine = true;
                if(lastWidth!=0) fprintf(fstream,";\n");
                lastWidth = i->width;
                if(lastWidth!=1) fprintf(fstream,"reg[%d:0]",lastWidth-1);
                else fprintf(fstream,"reg",lastWidth-1);
            }
            fprintf(fstream,"%c %s",firstOnLine ? ' ' : ',',i->name.c_str());
        }
        fprintf(fstream,";\n");
        lastWidth = 0;
        for(auto &o : outputs){
            if(o->width!=lastWidth){
                if(lastWidth!=0) fprintf(fstream,";\n");
                lastWidth = o->width;
                if(lastWidth!=1) fprintf(fstream,"wire[%d:0]",lastWidth-1);
                else fprintf(fstream,"wire",lastWidth-1);
            }
            fprintf(fstream," %s",o->name.c_str());
        }
        fprintf(fstream,";\n");

        fprintf(fstream,"\n%s %s_inst(\n",m_name.c_str(),m_name.c_str());
        bool firstPort = true;
        for(auto &p: m_Ports){
            if(p.name=="clk") clock = true;
            fprintf(fstream,"%c.%s(%s)",firstPort ? ' ' : ',',p.name.c_str(),p.name.c_str());
            firstPort = false;
        }
        fprintf(fstream,");\n");
        if(clock) fprintf(fstream,"\nalways #5 clk = ~clk;\n");
        fprintf(fstream,"\ninitial begin\n");
        if(clock) fprintf(fstream,"clk = 0;\n");
        fprintf(fstream,"end\n");


    }

    void CreateInstance(FILE *fstream){
        std::vector<Port*> inputs, outputs, inouts;
        for(auto &p: m_Ports){
            switch(p.dir) {
                case Direction::Input:
                    inputs.push_back(&p);
                    break;
                case Direction::Output:
                    outputs.push_back(&p);
                    break;
                case Direction::Inout:
                    inouts.push_back(&p);
                    break;
            }
        }

        std::sort(inputs.begin(), inputs.end(),[](Port *a, Port *b) {
            return a->width < b->width;
        });
        std::sort(outputs.begin(), outputs.end(),[](Port *a, Port *b) {
            return a->width < b->width;
        });
        std::sort(inouts.begin(), inouts.end(),[](Port *a, Port *b) {
            return a->width < b->width;
        });

        std::vector<std::pair<std::string,std::string>> FAR_pairs = {};
        FAR_pairs.emplace_back("$COMMENT_LINE$","/****************************************/");
        FAR_pairs.emplace_back("$FRONT_PAD$","/****************");
        FAR_pairs.emplace_back("$END_PAD$","****************/");
        FAR_pairs.emplace_back("$MODULE_NAME$",m_name);

        int lastWidth = 0;
        std::string input_string, output_string, inout_string;
        char input_buffer[2048] = {0}, output_buffer[2048] = {0}, inout_buffer[2048] = {0};

        for(auto &i : inputs){
            bool firstOnLine = false;
            if(i->width!=lastWidth){
                firstOnLine = true;
                if(lastWidth!=0) {
                    sprintf(input_buffer,";\n");
                    APPEND_END(input_buffer,input_string)
                }
                lastWidth = i->width;
                if(lastWidth!=1) {
                    if(lastWidth==-1){
                        sprintf(input_buffer,"reg[%s]",i->parameterized_width.c_str());
                    }
                    else {
                        sprintf(input_buffer,"reg[%d:0]",lastWidth-1);
                    }
                    APPEND_END(input_buffer,input_string)
                }
                else {
                    sprintf(input_buffer,"reg");
                    APPEND_END(input_buffer,input_string)

                }
            }
            sprintf(input_buffer,"%s%s",firstOnLine ? " " : ", ",i->name.c_str());
            APPEND_END(input_buffer,input_string)

        }
        if(inputs.size()>0) {
            sprintf(input_buffer, ";\n");
            APPEND_END(input_buffer, input_string)
        }

        FAR_pairs.emplace_back("$INPUT_NETS$",input_string);


        lastWidth = 0;
        for(auto &o : outputs){
            if(o->width!=lastWidth){
                if(lastWidth!=0) {
                    sprintf(output_buffer,";\n");
                    APPEND_END(output_buffer,output_string)
                }
                lastWidth = o->width;
                if(lastWidth!=1) {
                    sprintf(output_buffer,"wire[%d:0]",lastWidth-1);
                    APPEND_END(output_buffer,output_string)
                }
                else {
                    sprintf(output_buffer,"wire",lastWidth-1);
                    APPEND_END(output_buffer,output_string)
                }
            }
            sprintf(output_buffer," %s",o->name.c_str());
            APPEND_END(output_buffer,output_string)
        }
        if(outputs.size()>0){
            sprintf(output_buffer,";\n");
            APPEND_END(output_buffer,output_string)
        }
        FAR_pairs.emplace_back("$OUTPUT_NETS$",output_string);

        lastWidth = 0;
        for(auto &io : inouts){
            if(io->width!=lastWidth){
                if(lastWidth!=0) {
                    sprintf(inout_buffer,";\n");
                    APPEND_END(inout_buffer,inout_string)
                }
                lastWidth = io->width;
                if(lastWidth!=1) {
                    sprintf(inout_buffer,"wire[%d:0]",lastWidth-1);
                    APPEND_END(inout_buffer,inout_string)
                }
                else {
                    sprintf(inout_buffer,"wire",lastWidth-1);
                    APPEND_END(inout_buffer,inout_string)
                }
            }
            sprintf(inout_buffer," %s",io->name.c_str());
            APPEND_END(inout_buffer,inout_string)
        }
        if(inouts.size()>0){
            sprintf(inout_buffer,";\n");
            APPEND_END(inout_buffer,inout_string)
        }
        FAR_pairs.emplace_back("$INOUT_NETS$",inout_string);

        char instance_buffer[4096] = {0};
        std::string instance_string;
        sprintf(instance_buffer,"\n%s %s_inst(\n",m_name.c_str(),m_name.c_str());
        APPEND_END(instance_buffer,instance_string)
        bool lastPort = true;
        for(auto &p: m_Ports){
            if(static_cast<Port>(p) == static_cast<Port>(m_Ports[m_Ports.size() - 1])) lastPort = true;
            else lastPort = false;
            int length = p.name.length() * 2 + 4;
            char tabs[13] = "\t\t\t\t\t\t\t\t\t\t\t\t";
            if(length/4 < 12) tabs[12-length/4] = 0;
            sprintf(instance_buffer,"\t.%s(%s)%c%s//%s\n",
                    p.name.c_str(),p.name.c_str(),lastPort ? ' ' : ',',tabs,p.GetString().c_str());
            APPEND_END(instance_buffer,instance_string)

        }
        sprintf(instance_buffer,");\n");
        APPEND_END(instance_buffer,instance_string)


        FAR_pairs.emplace_back("$INSTANCE$",instance_string);

        std::string moduleFilePath = m_name + ".v";
        std::string command_prefix = "python find_and_replace.py " + moduleFilePath + " " + moduleFilePath + " ";
        //Clear the output file if it exists
        std::string clear_command = "copy /Y C:\\Users\\Aweso\\CLionProjects\\testbench\\template\\template2.txt " + moduleFilePath;
        system(clear_command.c_str());
        const char *find_file =".find_buf";
        const char *replace_file =".replace_buf";
        for(auto &p : FAR_pairs){
            auto find = fopen(find_file,"w");
            auto replace = fopen(replace_file,"w");
            if(!find || !replace) {
                if(find) fclose(find);
                if(replace) fclose(replace);
                return;
            }
            //printf("Finding: %s\nReplacing: %s\n",p.first.c_str(),p.second.c_str());
            fprintf(find,"%s\n",p.first.c_str());
            fprintf(replace,"%s\n",p.second.c_str());
            fclose(find);
            fclose(replace);
            std::string command = command_prefix + find_file + " " + replace_file;
            printf("Executing command: %s\n", command.c_str());
            system(command.c_str());
        }
        printf("Executing command: del -f .find_buf\"\n");
        printf("Executing command: del -f .replace_buf\"\n");
        system("del -f .find_buf");
        system("del -f .replace_buf");


    }


    Module(std::string &filename, std::string &fileEncoding){
        Encoding encoding = UTF8;
        if(fileEncoding=="UTF8") encoding = UTF8;
        else if(fileEncoding=="UTF16LE") encoding = UTF16LE;
        bool hasParam = false;
        FILE *file = fopen(filename.c_str(),"r");
        if(!file) return;
        {
            char firstLine[LINEBUFFERSIZE] = {0};
            char name[NAMEBUFFERSIZE] = {0};
            char *ptr, *nameptr;
            nameptr = name;
            fgets_encoding(firstLine,64,file,encoding);
            ptr = strstr(firstLine, "module");
            if (!ptr) return;
            ptr += sizeof("module");
            while (isblank(*ptr))ptr++;
            while (*ptr != '(' && *ptr != '#' && !isblank(*ptr)) {
                *nameptr++ = *ptr++;
            }
            if(*ptr == '#') hasParam = true;
            m_name = name;
        }
        while(hasParam && !feof(file)){
            char nameBuffer[NAMEBUFFERSIZE] = {0};
            int default_value = INT_MIN;
            char lineBuffer[LINEBUFFERSIZE] = {0};
            char comment = 0;
            char* commentBuffer = &comment;
            char *ptr;
            fgets_encoding(lineBuffer, LINEBUFFERSIZE, file,encoding);
            ptr = strstr(lineBuffer,"parameter");
            if(ptr){
                ptr += sizeof("parameter");
                while(isblank(*ptr))ptr++;//skips whitespace
                sscanf(ptr,"%s",nameBuffer);
                ptr += strlen(nameBuffer);
                ptr = strstr(ptr,"=");
                if(ptr){//default value
                    while(isblank(*ptr) || *ptr == '=') ptr++;
                    sscanf(ptr,"%d",&default_value);
                }
                ptr = strstr(lineBuffer,"//");
                if(ptr){
                    ptr += sizeof("/");
                    commentBuffer = ptr;
                }
                Parameter parameter(nameBuffer,default_value,commentBuffer);
                parameter.Print();
                m_parameters.push_back(parameter);

            } else{
                ptr = strstr(lineBuffer,")");
                if(ptr) hasParam = false;
            }

        }
        while(!feof(file)){
            char lineBuffer[LINEBUFFERSIZE] = {0};
            char *ptr;

            Direction lineDir;
            int lineWidth = 1;
            std::vector<std::string> names;
            std::vector<std::string> comments;
            std::string parameterized_width;

            fgets_encoding(lineBuffer, LINEBUFFERSIZE, file,encoding);
            ptr = lineBuffer;
            while(*ptr && isblank(*ptr)) ptr++; //skips whitespace
            if(*ptr == '\n' || *ptr == '\r' || *ptr == 0) continue; //skips empty lines
            //identifies in or out
            ptr = strstr(lineBuffer, "input");
            if(ptr) lineDir = Direction::Input;
            else{
                ptr = strstr(lineBuffer, "output");
                if(ptr) lineDir = Direction::Output;
                else{
                    ptr = strstr(lineBuffer, "inout");
                    if(ptr) lineDir = Direction::Inout;
                    else{
                        printf("Error: No input, output or inout found in line: %s\n",lineBuffer);
                        continue; //skips the line if no direction is found
                    }
                }
            }

            switch(lineDir){
                case Direction::Input:
                    ptr+=sizeof("input")-1; //default width for input
                    break;
                case Direction::Output:
                    ptr+=sizeof("output")-1; //default width for output
                    break;
                case Direction::Inout:
                    ptr+=sizeof("inout")-1; //default width for inout
                    break;
            }


            while(isblank(*ptr))ptr++;//skips whitespace

            if(lineDir==Direction::Output || lineDir==Direction::Inout){ //skips over reg if it is an output or inout
                auto temp = strstr(ptr,"reg");
                if(ptr==temp){
                    ptr+=sizeof("reg")-1;
                }
                while(isblank(*ptr))ptr++;//skips whitespace
            } else if(lineDir == Direction::Input || lineDir==Direction::Inout){ //skips over wire
                auto temp = strstr(ptr,"wire");
                if(ptr==temp){
                    ptr+=sizeof("wire")-1;
                }
                while(isblank(*ptr))ptr++;//skips whitespace
            }

            if(*ptr=='['){//identifies and skips width
                char upper[NAMEBUFFERSIZE], lower[NAMEBUFFERSIZE];
                char *delin = strchr(ptr,':'); //finds the colon
                if(!delin){
                    printf("Error: Invalid width in line: %s\n",lineBuffer);
                    continue; //skips the line if width is invalid
                }
                *delin = 0; //null terminates the upper width
                sscanf(++ptr,"%s",upper);
                ptr = delin + 1; //moves the pointer to the lower width
                delin = strchr(ptr,']'); //finds the closing bracket
                if(!delin){
                    printf("Error: Invalid width in line: %s\n",lineBuffer);
                    continue; //skips the line if width is invalid
                }
                *delin = 0; //null terminates the lower width
                sscanf(++ptr,"%s",lower); //reads the lower width
                if(upper[0] == 0 || lower[0] == 0) {
                    printf("Error: Invalid width in line: %s\n", lineBuffer);
                    continue; //skips the line if width is invalid
                }

                //check if upper and lower are numbers or variables
                char* endptr;
                int upperNum = strtoll(upper, &endptr, 10);
                if(endptr){
                    parameterized_width += upper;
                    parameterized_width += ":";
                    parameterized_width += lower;
                    lineWidth = -1;
                }else {
                    int lowerNum = strtoll(lower, &endptr, 10);
                    if(endptr){
                        lineWidth = upperNum - lowerNum + 1;
                    } else {
                        parameterized_width += upper;
                        parameterized_width += ":";
                        parameterized_width += lower;
                        lineWidth = -1;
                    }
                }

                while(*ptr!=']')ptr++;
                ptr++;
                while(isblank(*ptr))ptr++;//skips whitespace
            }


            while(ptr!=lineBuffer+LINEBUFFERSIZE-1){
                char nameBuffer[NAMEBUFFERSIZE] = {0};
                char *commentPtr = nullptr;
                sscanf(ptr,"%s",nameBuffer);
                int length = strlen(nameBuffer);
                if(length==0)break;
                if(nameBuffer[0]!=',') {
                    for(int i = 0; i < length; i++){
                        if(nameBuffer[i] == ','){
                            nameBuffer[i] = 0;
                            length = i;
                            break;
                        }
                    }
                    names.emplace_back(nameBuffer);
                    ptr+=length;
                    commentPtr = strstr(ptr,"//");
                    if(commentPtr){
                        comments.emplace_back(commentPtr);
                        ptr+=strlen(commentPtr);
                    } else {
                        comments.emplace_back("");

                    }
                }
                else ptr++;
            }
            for(int i = 0; i < names.size(); i++){
                if(names[i].empty() || names[i][0] == ',') continue; //skips empty names
                m_Ports.emplace_back(lineWidth,lineDir,names[i],comments[i], parameterized_width);
            }


        }
        fclose(file);

    }
private:
    void fgets_encoding(char *buffer, size_t maxCount, FILE *file, Encoding fileEncoding){
        switch(fileEncoding){
            case UTF8:
                fgets(buffer,maxCount,file);
                break;
            case UTF16LE:
                for(int i = 0; i < maxCount; i++){
                    char temp = fgetc(file);
                    if(!temp) i--;
                    else{
                        if(temp=='\n' || feof(file)){
                            buffer[i] = 0;
                            break;
                        }
                        else{
                            buffer[i] = temp;
                        }
                    }
                }
        }
    }

};


#endif //TESTBENCH_MODULE_H
