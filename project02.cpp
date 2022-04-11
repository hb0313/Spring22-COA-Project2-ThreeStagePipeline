#include "project02.h"
#include <fstream>
#include <vector>
using namespace std;

//Simple_Pipe simple_pipe;
int tempIndex;
int Ry = 0, Rz = 0 , Rx = 0, destReg = 0;
bool IF_END = false, FLAG = false;
std::vector<char> buffer;
char INSTRCUTION_REG[3]; // 0 : Destination Address, 1 : Left Operand, 2 : Right Operand
int DECODER_QUEUE[3];
int Simple_Pipe :: EX_ALU()
{
    DECODER_QUEUE[0] = INSTRCUTION_REG[0]&0xff;
    DECODER_QUEUE[1] = INSTRCUTION_REG[1]&0xff;
    DECODER_QUEUE[2] = INSTRCUTION_REG[2]&0xff;
    //return buffer[tempIndex]&0xff;
    return 0;
}

int Simple_Pipe ::EX_REG_FETCH(int index)
{
    tempIndex = index;
    //execution_time = (IF_END == true) ? execution_time : ++execution_time;
    // if (flag == 1 && OPCODE_SET == false) // to assign immediate value
    // {
    INSTRCUTION_REG[0] = buffer[--tempIndex];
    INSTRCUTION_REG[1] = buffer[--tempIndex];
    INSTRCUTION_REG[2] = buffer[--tempIndex];
    // INSTRCUTION_REG[0] = simple_pipe.EX_ALU(--tempIndex);
    // INSTRCUTION_REG[1] = simple_pipe.EX_ALU(--tempIndex);     //assign Left operand, typecast to integer
    // INSTRCUTION_REG[2] = simple_pipe.EX_ALU(--tempIndex);
    // }
    // else if(flag == 1 && OPCODE_SET == true)
    // {
    //     INSTRCUTION_REG[0] = simple_pipe.EX_ALU(--tempIndex); //assign Destination register value
    //     INSTRCUTION_REG[1] = simple_pipe.EX_ALU(--tempIndex);     //assign Left operand, typecast to integer
    // }
    return 0;
}

void Simple_Pipe :: EX_REG_ASIG(int* Rx, int* Ry, int* Rz)
{
    simple_pipe.EX_ALU();
    *Rx = DECODER_QUEUE[0];
    *Ry = DECODER_QUEUE[1];
    *Rz = DECODER_QUEUE[2];
    for (int FETCH_COUNTER = 0; FETCH_COUNTER < 3 && IF_END == true; FETCH_COUNTER++)
    {
        INSTRCUTION_REG[FETCH_COUNTER] = 0;
        //FLAG = true;
        simple_pipe.EXEC_COUNT();
    }
        //cout<<"\n R"<<FETCH_COUNTER<<" - "<<INSTRCUTION_REG[FETCH_COUNTER];
}

int Simple_Pipe ::OPCODE_x00(int Rx, int Ry) //OPCODE SET
{
    int Rz = 0; //Dummy value
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);     //cout<<endl << &Rx<<&Ry;
    simple_pipe.registers[Rx] = Ry;
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x10(int Rx, int Ry, int Rz) //OPCODE ADD 
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]+simple_pipe.registers[Rz];
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x11(int Rx, int Ry, int Rz) //OPCODE ADD with Immediate value
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]+Rz;
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x20(int Rx, int Ry, int Rz) //OPCODE SUB
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - simple_pipe.registers[Rz];
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x21(int Rx, int Ry, int Rz) //OPCODE SUB with immediate value
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - Rz;
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x30(int Rx, int Ry, int Rz) //OPCODE MUL
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * simple_pipe.registers[Rz];
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x31(int Rx, int Ry, int Rz) //OPCODE MUL with Immediate value
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * Rz;
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x40(int Rx, int Ry, int Rz) //OPCODE DIV
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]/simple_pipe.registers[Rz];
    simple_pipe.EXEC_COUNT();
    return 0;
}

int Simple_Pipe ::OPCODE_x41(int Rx, int Ry, int Rz) //OPCODE DIV with Immediate value
{
    simple_pipe.EX_REG_ASIG(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]/Rz;
    simple_pipe.EXEC_COUNT();
    return 0;
}

void Simple_Pipe :: EXEC_COUNT()
{   
    if(IF_END == true)
        execution_time++;
} 

int main(int argc, char *argv[]){
    Simple_Pipe simple_pipe;
    for(int i = 0; i < REG_COUNT; i++){
        simple_pipe.registers[i] = 0;
    }

//open file
std::ifstream infile(argv[1], ios::in | ios::binary);
int flag = 0;
bool OPCODE_SET = false;

//get length of file
infile.seekg(0, infile.end);
size_t length = infile.tellg();
infile.seekg(0, infile.beg);

//read file
if (length > 0) {
    buffer.resize(length);    
    infile.read(&buffer[0], length);
}
size_t BUFFER_SIZE = buffer.size();
execution_time = 0;
int d = 1;
for (int i=0; i <= BUFFER_SIZE; i++,d++) { //Fetch Instructions
    simple_pipe.EX_REG_FETCH(i);
    if(d%4 == 0 && d != 0) { 
        IF_END = (i == BUFFER_SIZE) ? true : false;
        request_done++;
        execution_time++;
        if(buffer[i] == '\x00') 
        {
            //flag = simple_pipe.EX_REG_FETCH(i, 1, false);
            simple_pipe.OPCODE_x00(destReg, Ry);
        }
        else if(buffer[i] == '\x10')
        {
            //flag = simple_pipe.EX_REG_FETCH(i, 1, false);
            simple_pipe.OPCODE_x10(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x11')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x11(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x20')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x20(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x21')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x21(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x30')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x30(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x31')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x31(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x40')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x40(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x41')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x41(destReg, Ry, Rz);
        }
        else    
            cout<<"No Match!\n";
        }
    }
infile.close();


    simple_pipe.print_regs();
    std::cout << "Total execution cycles: " << execution_time << std::endl;
    std::cout << "\nIPC: " << (request_done/(double)execution_time) << std::endl << std::endl;

    return 0;
}

void Simple_Pipe::print_regs(){
    printf("\nRegisters: \n");
    std::cout << "----------------------------------------" << std::endl;
    for(int i = 0; i < REG_COUNT; i+=2){
        std::string regl("R");
        regl.append(std::to_string(i));
        regl.append(": ");

        std::string regr("R");
        regr.append(std::to_string(i+1));
        regr.append(": ");
        if(i < 15){
            std::cout << "  " << std::setiosflags(std::ios::left) 
            << std::setw(5) << regl  << std::setw(10) << simple_pipe.registers[i] << " |   " 
            << std::setw(5) << regr << std::setw(10) << simple_pipe.registers[i+1] << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }else{
            std::cout << "  " << std::setiosflags(std::ios::left) 
            << std::setw(5) << regl << std::setw(10) << simple_pipe.registers[i] << " |   " << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    }  
    printf("\n");
}