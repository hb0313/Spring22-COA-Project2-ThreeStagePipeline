#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

int reg[16];
int tempIndex;
int Ry = 0, Rz = 0 , destReg = 0;
std::vector<char> buffer;

int decode(int tempIndex)
{
    return buffer[tempIndex]&0xff;
}

int regAssignment(int index, int flag, bool OPCODE_SET)
{
    tempIndex = index;
    if (flag == 1 && OPCODE_SET == false) // to assign immediate value
    {
    destReg = decode(--tempIndex); //assign Destination register value
    cout <<"\t\nValue of Destination "<<destReg;
    Ry = decode(--tempIndex);     //assign Left operand, typecast to integer
    cout <<"\t\nValue of Ry "<<Ry;
    Rz = decode(--tempIndex);
    cout <<"\t\nValue of Rz "<<Rz;
    }
    else if(flag == 1 && OPCODE_SET == true)
    {
        destReg = decode(--tempIndex); //assign Destination register value
        //cout<<endl<<"Destn R: "<<destReg;
        //cout <<tempIndex<< " Value of Destination "<<destReg<<endl;
        Ry = decode(--tempIndex);     //assign Left operand, typecast to integer
        //cout <<tempIndex<< " Value of Ry "<<Ry<<endl;
    }
    // else // assign Ry and Rz
    // {
    // destReg = decode(tempIndex); //assign Destination register value
    // Ry = decode(tempIndex); //assign Left operand, typecast to integer
    // cout << "Value of Ry "<<Ry<<" is "<<reg[Ry]<<endl;
    // Rz = decode(tempIndex);
    // cout << "Value of Rz "<<Rz<<" is "<<reg[Rz]<<endl;
    // }
    flag = 0;
    return flag;
}

int OPCODE_x00(int Rx, int Ry) //OPCODE SET
{
    cout<<endl<<"R"<<Rx;
    reg[Rx] = Ry;
    cout<<"---"<<reg[Rx];
    return 0;
}

int OPCODE_x10(int Rx, int Ry, int Rz) //OPCODE ADD 
{
    cout<<"\nDestination - R"<<Rx<<"  Left OP - "<<reg[Ry]<<" Right OP - "<<reg[Rz];
    reg[Rx] = reg[Ry]+reg[Rz];
    cout<<"\nAddition "<<reg[Rx]<<endl;
    return 0;
}

int OPCODE_x11(int Rx, int Ry, int Rz) //OPCODE ADD with Immediate value
{
    cout<<"\nDestination - "<<Rx<<"  Left OP - "<<reg[Ry]<<" Right OP - "<<Rz;
    reg[Rx] = reg[Ry]+Rz;
    cout<<"\nAddition "<<reg[Rx]<<endl;
    return 0;
}

int OPCODE_x20(int Rx, int Ry, int Rz) //OPCODE SUB
{
    cout<<"\nDestination - R"<<Rx<<"  Left OP - "<<reg[Ry]<<" Right OP - "<<reg[Rz];
    reg[Rx] = reg[Ry] - reg[Rz];
    cout<<"\nAddition "<<reg[Rx]<<endl;
    return 0;
}

int OPCODE_x21(int Rx, int Ry, int Rz) //OPCODE SUB with immediate value
{
    reg[Rx] = reg[Ry] - Rz;
    return 0;
}

int OPCODE_x30(int Rx, int Ry, int Rz) //OPCODE MUL
{
    reg[Rx] = reg[Ry] * reg[Rz];
    return 0;
}

int OPCODE_x31(int Rx, int Ry, int Rz) //OPCODE MUL with Immediate value
{
    reg[Rx] = reg[Ry] * Rz;
    return 0;
}

int OPCODE_x40(int Rx, int Ry, int Rz) //OPCODE DIV
{
    reg[Rx] = reg[Ry]/reg[Rz];
    return 0;
}

int OPCODE_x41(int Rx, int Ry, int Rz) //OPCODE DIV with Immediate value
{
    reg[Rx] = reg[Ry]/Rz;
    return 0;
}

int main(int argc, char *argv[]) {

for(int i = 0; i<16; i++)
    reg[i] = 0;

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
            flag = regAssignment(i, 1, true);
            OPCODE_x00(destReg, Ry);
        }
        else if(buffer[i] == '\x10')
        {
            printf("\nADD - Found at %d : %2X\n", i, buffer[i]);
            regAssignment(i, 1, false);
            cout<<endl<<destReg<<" "<<Ry<<" "<<Rz;
            OPCODE_x10(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x11')
        {
            printf("\nADD#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x11(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x20')
        {
            printf("SUB - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x20(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x21')
        {
            printf("SUB#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x21(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x30')
        {
            printf("MUL - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x30(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x31')
        {
            printf("MUL#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x31(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x40')
        {
            printf("DIV - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x40(destReg, Ry, Rz);
        }
        else if(buffer[i] == '\x41')
        {
            printf("DIV#Imm - Found at %d : %2X\n", i, buffer[i]);
            flag = regAssignment(i,1,false);
            OPCODE_x41(destReg, Ry, Rz);
        }
        else    
            cout<<"No Match!\n";
        }
    }
//size_t size = hex.size();

// for (int i = 0; i <= 8 ; i++)
//     if(i%4 == 0 && i != 0)
//         cout<<endl;
//     else
//         printf("%X ", hex[i]);

//if(hex[0] == '\x00')
  //  cout<<"\nFound\n";

cout<<endl;
// cout.setf(ios::hex, ios::basefield);
//     std::cout << std::showbase << std::hex << a <<endl;
// cout.unsetf(ios::hex);
//printf("%x\n",buffer[1]);


        // for (int i = 0; i < 100 ; i++) {
        //     if(i%4==0 && i!=0)
        //         cout<<endl;
        //     else
        //         printf("%X ", hex[i]);
        // }

// for (int i=0; i<100; i++)
//     printf("%X",buffer[i]);

infile.close();

//print reg
cout<<"\n*****Register Final Values****"<<endl;
for (int k=0 ; k <=15; k++)
    cout<<"R"<<k<<" : "<<reg[k]<<endl;

return 0;
}