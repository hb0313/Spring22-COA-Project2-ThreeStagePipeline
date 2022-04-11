#include "project02.h"
#include <fstream>
#include <vector>


using namespace std;

int tempIndex;
int Ry = 0, Rz = 0 , destReg = 0;
std::vector<char> buffer;

int Simple_Pipe :: decode(int tempIndex)
{
    return buffer[tempIndex]&0xff;
}

int Simple_Pipe ::regAssignment(int index, int flag, bool OPCODE_SET)
{
    tempIndex = index;
    if (flag == 1 && OPCODE_SET == false) // to assign immediate value
    {
    destReg = simple_pipe.decode(--tempIndex); //assign Destination register value
    cout <<"\t\nValue of Destination "<<destReg;
    Ry = simple_pipe.decode(--tempIndex);     //assign Left operand, typecast to integer
    cout <<"\t\nValue of Ry "<<Ry;
    Rz = simple_pipe.decode(--tempIndex);
    //cout <<"\t\nValue of Rz "<<Rz;
    }
    else if(flag == 1 && OPCODE_SET == true)
    {
        destReg = simple_pipe.decode(--tempIndex); //assign Destination register value
        //cout<<endl<<"Destn R: "<<destReg;
        //cout <<tempIndex<< " Value of Destination "<<destReg<<endl;
        Ry = simple_pipe.decode(--tempIndex);     //assign Left operand, typecast to integer
        //cout <<tempIndex<< " Value of Ry "<<Ry<<endl;
    }
    flag = 0;
    return flag;
}

int Simple_Pipe ::OPCODE_x00(int Rx, int Ry) //OPCODE SET
{
    cout<<endl<<"R"<<Rx;
    simple_pipe.registers[Rx] = Ry;
    cout<<"---"<<simple_pipe.registers[9];
    return 0;
}

int Simple_Pipe ::OPCODE_x10(int Rx, int Ry, int Rz) //OPCODE ADD 
{
    cout<<"\nDestination - R"<<Rx<<"  Left OP - "<<simple_pipe.registers[Ry]<<" Right OP - "<<simple_pipe.registers[Rz];
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]+simple_pipe.registers[Rz];
    cout<<"\nAddition "<<simple_pipe.registers[Rx]<<endl;
    return 0;
}

int Simple_Pipe ::OPCODE_x11(int Rx, int Ry, int Rz) //OPCODE ADD with Immediate value
{
    cout<<"\nDestination - "<<Rx<<"  Left OP - "<<simple_pipe.registers[Ry]<<" Right OP - "<<Rz;
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]+Rz;
    cout<<"\nAddition "<<simple_pipe.registers[Rx]<<endl;
    return 0;
}

int Simple_Pipe ::OPCODE_x20(int Rx, int Ry, int Rz) //OPCODE SUB
{
    cout<<"\nDestination - R"<<Rx<<"  Left OP - "<<simple_pipe.registers[Ry]<<" Right OP - "<<simple_pipe.registers[Rz];
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - simple_pipe.registers[Rz];
    cout<<"\nAddition "<<simple_pipe.registers[Rx]<<endl;
    return 0;
}

int Simple_Pipe ::OPCODE_x21(int Rx, int Ry, int Rz) //OPCODE SUB with immediate value
{
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] - Rz;
    return 0;
}

int Simple_Pipe ::OPCODE_x30(int Rx, int Ry, int Rz) //OPCODE MUL
{
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * simple_pipe.registers[Rz];
    return 0;
}

int Simple_Pipe ::OPCODE_x31(int Rx, int Ry, int Rz) //OPCODE MUL with Immediate value
{
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry] * Rz;
    return 0;
}

int Simple_Pipe ::OPCODE_x40(int Rx, int Ry, int Rz) //OPCODE DIV
{
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]/simple_pipe.registers[Rz];
    return 0;
}

int Simple_Pipe ::OPCODE_x41(int Rx, int Ry, int Rz) //OPCODE DIV with Immediate value
{
    simple_pipe.registers[Rx] = simple_pipe.registers[Ry]/Rz;
    return 0;
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
size_t bufSize = buffer.size();
//cout<<bufSize<<endl;
int d = 1;
for (int i=0; i <=bufSize; i++,d++) {
    //printf(" Buffer at %d : %X \n",i, buffer[i]);
    if(d%4 == 0 && d != 0) {
        if(buffer[i] == '\x00') 
        {
            printf("\nSET#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i, 1, true);
            simple_pipe.OPCODE_x00(destReg, Ry);
        }
        else if(buffer[i] == '\x10')
        {
            printf("\nADD - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i, 1, false);
            cout<<endl<<destReg<<" "<<Ry<<" "<<Rz;
            simple_pipe.OPCODE_x10(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x11')
        {
            printf("\nADD#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
            simple_pipe.OPCODE_x11(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x20')
        {
            printf("SUB - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
            simple_pipe.OPCODE_x20(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x21')
        {
            printf("SUB#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
            simple_pipe.OPCODE_x21(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x30')
        {
            printf("MUL - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
            simple_pipe.OPCODE_x30(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x31')
        {
            printf("MUL#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
            simple_pipe.OPCODE_x31(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x40')
        {
            printf("DIV - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
            simple_pipe.OPCODE_x40(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x41')
        {
            printf("DIV#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = simple_pipe.regAssignment(i,1,false);
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