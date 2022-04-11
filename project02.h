#include <iostream>
#include <string>
#include <iomanip>

#define REG_COUNT 16
#define INVALID_REG 17

unsigned long long execution_time;
unsigned long long request_get;
unsigned long long request_done;
unsigned long long line_num;


class Simple_Pipe{
public:
    int registers[16];
    void print_regs();
    int ID_INSTRUCTION();
    int IF_FETCH();
    void EX_EXECUTE();
    void EX_ALU(int*,int*,int*);
    int OPCODE_x00(int, int);
    int OPCODE_x10(int, int, int);
    int OPCODE_x11(int, int, int);
    int OPCODE_x20(int, int, int);
    int OPCODE_x21(int, int, int);
    int OPCODE_x30(int, int, int);
    int OPCODE_x31(int, int, int);
    int OPCODE_x40(int, int, int);
    int OPCODE_x41(int, int, int);
    void EXEC_COUNT();
};
Simple_Pipe simple_pipe;