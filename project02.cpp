/*
Name - Harshad Bhandwaldar
University - SUNY Binghamton
SCourse - Computer Organization and Architecture
*/

#include "project02.h"
#include <fstream>
#include <vector>

using namespace std;

/*====================================
  ******** GLOBAL VARIBALES ********
=====================================*/
int tempIndex;
int Ry = 0, Rz = 0, Rx = 0, destReg = 0, i = 3;
bool EXEC_FLAG = false;
std::vector<char> buffer;
size_t BUFFER_SIZE;
char INSTRUCTION_REG[4], OPCODE_OP; // 0 : OPCODE 1 : Destination Address, 2 : Left Operand, 3 : Right Operand
int DECODER_QUEUE[3];


/*====================================
  ******** DECODE(ID) STAGE ********
=====================================*/
int Simple_Pipe ::ID_INSTRUCTION()
{
    // cout<<"\nindide ID_INST";
    DECODER_QUEUE[0] = INSTRUCTION_REG[1] & 0xff; // Convert destination to integer
    DECODER_QUEUE[1] = INSTRUCTION_REG[2] & 0xff; // Convert Left Opernad to integer
    DECODER_QUEUE[2] = INSTRUCTION_REG[3] & 0xff; // Convert Right Operand to integer
    OPCODE_OP = INSTRUCTION_REG[0];               // Assign HE value of OPCODE to comparision variable
    return 0;
}

/*====================================
  ******** FECTH(IF) STAGE ********
=====================================*/
int Simple_Pipe ::IF_FETCH()
{

    tempIndex = i;
    INSTRUCTION_REG[0] = buffer[tempIndex];
    INSTRUCTION_REG[1] = buffer[--tempIndex];
    INSTRUCTION_REG[2] = buffer[--tempIndex];
    INSTRUCTION_REG[3] = buffer[--tempIndex];
    i = i + 4;
    return 0;
}

/*======================================================================
  ******** EXECUTION(EX) STAGE - EX:1 - Move values to Registers ********
========================================================================*/
void Simple_Pipe ::EX_ALU(int *Rx, int *Ry, int *Rz)
{
    *Rx = DECODER_QUEUE[0]; // Assign decoded value of destination register
    *Ry = DECODER_QUEUE[1]; // Assign decoder value of left operand to/or register
    *Rz = DECODER_QUEUE[2]; // Assign decoder value of right operand to/or register

    //**********If not fetch change Execution flag to TRUE and process the FETHCH_COUNTER***********
    for (int FETCH_COUNTER = 0; FETCH_COUNTER < 3 && i >= BUFFER_SIZE; FETCH_COUNTER++, execution_time++)
    {
        EXEC_FLAG = true;
    }
}

/*====================================
  ******** EXECUTE(EF) STAGE ********
=====================================*/
void Simple_Pipe ::EX_EXECUTE()
{
    request_done++;

    // *** Compare OPCODE is SET ***//
    if (OPCODE_OP == '\x00')
    {
        simple_pipe.OPCODE_x00(Rx, Ry);
    }
    // *** Compare OPCODE is ADD ***//
    else if (OPCODE_OP == '\x10')
    {
        simple_pipe.OPCODE_x10(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is ADD with Immediate value ***//
    else if (OPCODE_OP == '\x11')
    {
        simple_pipe.OPCODE_x11(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is SUB ***//
    else if (OPCODE_OP == '\x20')
    {
        simple_pipe.OPCODE_x20(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is SUB with Immediate value ***//
    else if (OPCODE_OP == '\x21')
    {
        simple_pipe.OPCODE_x21(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is MUL ***//
    else if (OPCODE_OP == '\x30')
    {
        simple_pipe.OPCODE_x30(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is MUL with Immediate value ***//
    else if (OPCODE_OP == '\x31')
    {
        simple_pipe.OPCODE_x31(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is DIV ***//
    else if (OPCODE_OP == '\x40')
    {
        simple_pipe.OPCODE_x40(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is DIV with Immediate value ***//
    else if (OPCODE_OP == '\x41')
    {
        simple_pipe.OPCODE_x41(Rx, Ry, Rz);
    }
}


/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.1 - SET ********
=====================================================*/
int Simple_Pipe ::OPCODE_x00(int Rx, int Ry)
{
    int Rz = 0; // Dummy value
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = Ry;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.2 - ADD ********
=====================================================*/
int Simple_Pipe ::OPCODE_x10(int Rx, int Ry, int Rz) // OPCODE ADD
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] + simple_pipe.registers[Rz];
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.3 - ADD ********
=====================================================*/
int Simple_Pipe ::OPCODE_x11(int Rx, int Ry, int Rz) // OPCODE ADD with Immediate value
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] + Rz;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.4 - SUB ********
=====================================================*/
int Simple_Pipe ::OPCODE_x20(int Rx, int Ry, int Rz) // OPCODE SUB
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - simple_pipe.registers[Rz];
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.5 - SUB ********
=====================================================*/
int Simple_Pipe ::OPCODE_x21(int Rx, int Ry, int Rz) // OPCODE SUB with immediate value
{
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - Rz;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.6 - MUL ********
=====================================================*/
int Simple_Pipe ::OPCODE_x30(int Rx, int Ry, int Rz) // OPCODE MUL
{
    execution_time = execution_time + 1;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * simple_pipe.registers[Rz];
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.7 - MUL ********
=====================================================*/
int Simple_Pipe ::OPCODE_x31(int Rx, int Ry, int Rz) // OPCODE MUL with Immediate value
{
    execution_time = execution_time + 1;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * Rz;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.8 - DIV ********
=====================================================*/
int Simple_Pipe ::OPCODE_x40(int Rx, int Ry, int Rz) // OPCODE DIV
{
    execution_time = execution_time + 3;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] / simple_pipe.registers[Rz];

    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.9 - DIV ********
=====================================================*/
int Simple_Pipe ::OPCODE_x41(int Rx, int Ry, int Rz) // OPCODE DIV with Immediate value
{
    execution_time = execution_time + 3;
    simple_pipe.EX_ALU(&Rx, &Ry, &Rz);
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] / Rz;
    return 0;
}


/*================================
  ******** INIT FUNCTION  ********
=================================*/
int main(int argc, char *argv[])
{
    Simple_Pipe simple_pipe;
    for (int i = 0; i < REG_COUNT; i++)
    {
        simple_pipe.registers[i] = 0;
    }

    // **** Open file usinf stream object **** // 
    std::ifstream infile(argv[1], ios::in | ios::binary);

    bool OPCODE_SET = false;

    // **** Get the length of file to read ****//
    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);

    // **** Read file the file as per the length **** //
    if (length > 0)
    {
        buffer.resize(length);
        infile.read(&buffer[0], length);
    }

    // **** Get the size of file in varuable BUFFER_SIZE **** //
    BUFFER_SIZE = buffer.size();
    execution_time = -1;
    while (EXEC_FLAG == false)
    {
        ++execution_time;
        //=====================================//
        // ****** STAGE 1 ----- FETCH ******* //
        //=====================================//
        simple_pipe.IF_FETCH();
        //=====================================//
        // ****** STAGE 2 ----- DECODE ******* //
        //=====================================//
        simple_pipe.ID_INSTRUCTION();
        //=====================================//
        // ****** STAGE 3 ----- EXECUTE ****** //
        //=====================================//
        simple_pipe.EX_EXECUTE();
    };

    // **** Close the file after completion **** //
    infile.close();

    // **** Print final status **** //
    simple_pipe.print_regs();
    std::cout << "Total execution cycles: " << execution_time << std::endl;
    std::cout << "\nIPC: " << (request_done / (double)execution_time) << std::endl
              << std::endl;

    return 0;
}

/*========================================
  ******** PRINTING FINAL STATUS  ********
=========================================*/
void Simple_Pipe::print_regs()
{
    printf("\nRegisters: \n");
    std::cout << "----------------------------------------" << std::endl;
    for (int i = 0; i < REG_COUNT; i += 2)
    {
        std::string regl("R");
        regl.append(std::to_string(i));
        regl.append(": ");

        std::string regr("R");
        regr.append(std::to_string(i + 1));
        regr.append(": ");
        if (i < 15)
        {
            std::cout << "  " << std::setiosflags(std::ios::left)
                      << std::setw(5) << regl << std::setw(10) << simple_pipe.registers[i] << " |   "
                      << std::setw(5) << regr << std::setw(10) << simple_pipe.registers[i + 1] << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
        else
        {
            std::cout << "  " << std::setiosflags(std::ios::left)
                      << std::setw(5) << regl << std::setw(10) << simple_pipe.registers[i] << " |   " << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    }
    printf("\n");
}
