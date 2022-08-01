#include<iostream>
#include<vector>
#include<cstddef>
#include<string.h>
#include<stdint.h>

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
    public:
    std::vector<Byte>instruction={};
    uint8_t num_one=0;
    uint8_t num_two=0;
    std::vector<uint8_t>weights={1,2,4,8};
    std::vector<Byte>binary_one={};
    std::vector<Byte>binary_two={};
    std::vector<Byte> letters={{"10","A"},{"11","B"},{"12","C"},{"13","D"},{"14","E"},{"15","F"}};
    /*converting data into hexedecimal*/
    void convert(Byte contents){
        /*spliting binary numbers in half*/
        int i;
        for(i=0;i<5;i++){
            binary_one[i]=contents.substr(i,1);    
        }
        for(i=4;i<9;i++){
            binary_one[i]=contents.substr(i,1);
        }
        /*convereting each half of the number to deneray*/
        for( i=0;i<=sizeof(binary_one)/sizeof(Byte);i++){
            if(binary_one[i]=="1"){
                num_one=num_one+weights[i];
            }
        }
        for( i=0;i<=sizeof(binary_two)/sizeof(Byte);i++){
            if(binary_two[i]=="1"){
                num_two=num_two+weights[i];
            }
        }
        /*checking if the deneray number is greater than 9 so we use hex letters and adding the hex digit into the instruction array*/
        if(num_one>9){
            for(i=0;i<6;i++){
                if(num_one==(uint8_t)letters[i][0]){
                    instruction[0]=letters[i][1];
                }
            }
        } else{
            instruction[0]=num_one;
        }
        
        if(num_two>9){
            for(i=0;i<6;i++){
                if(num_two==(uint8_t)letters[i][0]){
                    instruction[1]=letters[i][1];
                }
            }

        } else{instruction[1]=num_two;}


    }
};

int main(){
    int row=0;
    std::vector<Byte> ram={{"0xC760","10001010"},{"0xH890","0011010"},{"0x5I21","11110000"},{"0xP782","10010101"},{"0x901F","11001111"}};
    PC pc(ram,0);
    MAR mar;
    MDR mdr;
    CIR cir;
    CU cu;
    ACC acc;
    ALU alu;
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
        for(int i=0; i<=sizeof(ram)/sizeof(Byte);i++){
            if(ram[i].substr(0,4)==mar.copied_address){
                mdr.memory_read(ram,i);
                break;
            }
            

        }
        acc.copy(mdr.contents);
        alu.convert(acc.contents);
        std::cout<<"Processed";
      
    } else{
        std::cout<<"failed process";
    }
    return 0;

}



