#include<iostream>
#include<vector>
#include<cstddef>
#include<string.h>

using Byte = std::string;

/*Control Unit*/
class CU{
    public:
    Byte opcode;
    int signal=0;
    /*reads signal from MAR to see if should read instruction from RAM*/
    void read_signal(int MAR_signal){ 
        if(MAR_signal==1){
            signal=1;
        }
    }
    /*gets the opcode from current instruction*/
    void get_opcode(std::vector<Byte>&ram,int row){
        opcode=ram[row].substr(5,4);/*from the 5th character extract 4 characters*/
    }
    
    
    

};
/*Memory data register*/
class MAR{
    public:
    int signal=0;  
    int row; 
    int coloumn;
    Byte copied_address;
    /*sends signal to CU weather it should read instruction from RAM*/
    void get_contents(){ 
         signal=1;
    }
    /*gets copied address from program counter*/
    void copy_address(Byte address){
        copied_address=address;
    }

};
/*Memory Data Register*/
class MDR{
    public:
    Byte contents;
    void memory_read(std::vector<Byte> &ram,int row){
            contents=ram[row][1];
        
    }
    

};
/*Program Counter*/
class PC{
    public:
    Byte address;
    void increment(std::vector<Byte>&ram,int row){
        row=row+1;
        address=ram[row][0];
    }
    PC(std::vector<Byte>&ram,int row){
        address= ram[row][0];
    }
};
/*Current Instruction Register*/
class CIR{
    public:
    Byte copied_contents;
    void copy_contents(Byte contents){
        copied_contents=contents;
    }

};


/*Accumulator*/
class ACC{
    public:
    Byte contents;
    void copy(Byte data){
        contents=data;
    }

};
/*Arithmetic logical unit*/
class ALU{

};

void main(){
    int row=0;
    std::vector<Byte> ram={{"0xC760","100010101"},{"0xH890","0011010"},{"0x5I21","11110000"},{"0xP782","10010101"},{"0x901F","11001111"}};
    PC pc(ram,row);
    MAR mar;
    MDR mdr;
    CIR cir;
    CU cu;
    ACC acc;
    /*Fetching*/
    mar.copy_address(pc.address);
    mar.get_contents();
    cu.read_signal(mar.signal);
    if(cu.signal==1){
        mdr.memory_read(ram,row);
        cir.copy_contents(mdr.contents);
        pc.increment(ram,row);

        /*Decoding*/
        cu.get_opcode(ram,row);
        Byte operand=ram[row].substr(9,4); /*from the 9th character get 4 characters*/
        /*Executing*/
        mar.copy_address(operand);
        for(int i=0; i<sizeof(ram)/sizeof(Byte);i++){
            if(ram[i].substr(0,4)==mar.copied_address){
                mdr.memory_read(ram,i);
                break;
            }
            acc.copy(mdr.contents);

        }
        
    }

}



