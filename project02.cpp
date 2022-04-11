#include "project02.h"
#include <fstream>
#include <vector>
using namespace std;

//Simple_Pipe simple_pipe;
int tempIndex;
int Ry = 0, Rz = 0 , Rx = 0, destReg = 0, i = 3;
bool IF_END = false, CLK_FLAG = false, DECODE_FLAG = false, EXEC_FLAG = false;
std::vector<char> buffer;
size_t BUFFER_SIZE;
char INSTRUCTION_REG[4], OPCODE_OP; // 0 : OPCODE 1 : Destination Address, 2 : Left Operand, 3 : Right Operand
int DECODER_QUEUE[3];

int Simple_Pipe :: ID_INSTRUCTION()
{
    //cout<<"\nindide ID_INST";
    DECODER_QUEUE[0] = INSTRUCTION_REG[1]&0xff; // 
    DECODER_QUEUE[1] = INSTRUCTION_REG[2]&0xff;
    DECODER_QUEUE[2] = INSTRUCTION_REG[3]&0xff;
    OPCODE_OP = INSTRUCTION_REG[0];

    return 0;
}

int Simple_Pipe :: IF_FETCH()
{
    
    tempIndex = i;

        INSTRUCTION_REG[0] = buffer[tempIndex]; 
        INSTRUCTION_REG[1] = buffer[--tempIndex];
        INSTRUCTION_REG[2] = buffer[--tempIndex]; 
        INSTRUCTION_REG[3] = buffer[--tempIndex];

        i = i+4;
    return 0;
}

void Simple_Pipe :: EX_ALU(int* Rx, int* Ry, int* Rz)
{
    *Rx = DECODER_QUEUE[0];
    *Ry = DECODER_QUEUE[1];
    *Rz = DECODER_QUEUE[2];
    for (int FETCH_COUNTER = 0; FETCH_COUNTER < 3 && i >= BUFFER_SIZE; FETCH_COUNTER++, execution_time++)
    {
        EXEC_FLAG = true;
    }
}

void Simple_Pipe :: EX_EXECUTE()
{
        request_done++;
        if(OPCODE_OP == '\x00') 
        {
            //flag = simple_pipe.EX_REG_FETCH(i, 1, false);
            simple_pipe.OPCODE_x00(Rx, Ry);
        }
        else if(OPCODE_OP == '\x10')
        {
            //flag = simple_pipe.EX_REG_FETCH(i, 1, false);
            simple_pipe.OPCODE_x10(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x11')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x11(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x20')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x20(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x21')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x21(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x30')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x30(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x31')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x31(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x40')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x40(Rx, Ry, Rz);
        }
        else if(OPCODE_OP == '\x41')
        {
            //flag = simple_pipe.EX_REG_FETCH(i,1,false);
            simple_pipe.OPCODE_x41(Rx, Ry, Rz);
        }
}

int Simple_Pipe ::OPCODE_x00(int Rx, int Ry) //OPCODE SET
{
    int Rz = 0; //Dummy value
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);     //cout<<endl << &Rx<<&Ry;
    simple_pipe.registers[Rx] = Ry;
    return 0;
}

int Simple_Pipe ::OPCODE_x10(int Rx, int Ry, int Rz) //OPCODE ADD 
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]+simple_pipe.registers[Rz];
    return 0;
}

int Simple_Pipe ::OPCODE_x11(int Rx, int Ry, int Rz) //OPCODE ADD with Immediate value
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]+Rz;
    return 0;
}

int Simple_Pipe ::OPCODE_x20(int Rx, int Ry, int Rz) //OPCODE SUB
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - simple_pipe.registers[Rz];
    return 0;
}

int Simple_Pipe ::OPCODE_x21(int Rx, int Ry, int Rz) //OPCODE SUB with immediate value
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - Rz;
    return 0;
}

int Simple_Pipe ::OPCODE_x30(int Rx, int Ry, int Rz) //OPCODE MUL
{
    execution_time = execution_time+1;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * simple_pipe.registers[Rz];
    return 0;
}

int Simple_Pipe ::OPCODE_x31(int Rx, int Ry, int Rz) //OPCODE MUL with Immediate value
{
    execution_time = execution_time+1;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * Rz;
    return 0;
}

int Simple_Pipe ::OPCODE_x40(int Rx, int Ry, int Rz) //OPCODE DIV
{
    execution_time = execution_time+3;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] / simple_pipe.registers[Rz];

    return 0;
}

int Simple_Pipe ::OPCODE_x41(int Rx, int Ry, int Rz) //OPCODE DIV with Immediate value
{
    execution_time = execution_time+3;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] / Rz;
    return 0;
}

void Simple_Pipe :: EXEC_COUNT()
{
        execution_time++; 
}

int main(int argc, char *argv[]){
    Simple_Pipe simple_pipe;
    for(int i = 0; i < REG_COUNT; i++)
    {
        simple_pipe.registers[i] = 0;
    }

//open file
std::ifstream infile(argv[1], ios::in | ios::binary);

bool OPCODE_SET = false;

//get length of file
infile.seekg(0, infile.end);
size_t length = infile.tellg();
infile.seekg(0, infile.beg);

//read file
if (length > 0) 
{
    buffer.resize(length);    
    infile.read(&buffer[0], length);
}

BUFFER_SIZE = buffer.size();
execution_time = -1;

//for (i = 3; i <= BUFFER_SIZE; i=i+4) 
while(EXEC_FLAG == false)
{
    ++execution_time;
    //=====================================//
    // ******* STAGE 1 ----- FETCH ******* //
    //=====================================//
    simple_pipe.IF_FETCH();
    //=====================================//
    // ****** STAGE 2 ----- DECODE ******* //
    //=====================================//
    simple_pipe.ID_INSTRUCTION();
     //=====================================//
    // ******* STAGE 3 ----- EXECUTE ****** //
    //=====================================//
    simple_pipe.EX_EXECUTE();
};

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