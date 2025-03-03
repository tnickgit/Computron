#include "computron.h"

#include <fstream>
#include <iomanip>

void load_from_file(std::array<int, memorySize>& memory, const std::string& filename)
{
	constexpr int sentinel{ -99999 };
	size_t i{ 0 };
	std::string line;
	int instruction;

    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        // throw runtime_error exception with string "invalid_input"
        throw std::runtime_error("invalid_input");
    }
    while (std::getline(inputFile, line)) {
        instruction = std::stoi(line);
        if (instruction == sentinel)
        {
            break;
        }
        // Check if the instruction is valid using the validWord function
        // If the instruction is valid, store it in memory at position 'i' and increment 'i'
        // If the instruction is invalid, throw a runtime error with the message "invalid_input"
        int operandNum = instruction / 100; //get the first two digits in the instruction
        if (validWord(operandNum))
        {
            memory[i] = instruction; // place the instruction in the memroy
            i++;
        }
        else
        {
            throw std::runtime_error("invalid_input");
        }
        if (operandNum == 43)
            break;
    }
}

Command opCodeToCommand(size_t opCode) {
    switch (opCode) {
    case 10: return Command::read;
    case 11: return Command::write;
    case 20: return Command::load;
    case 21: return Command::store;
    case 30: return Command::add;
    case 31: return Command::subtract;
    case 32: return Command::divide;
    case 33: return Command::multiply;
    case 40: return Command::branch;
    case 41: return Command::branchNeg;
    case 42: return Command::branchZero;
    case 43: return Command::halt;
    default: throw std::runtime_error("not a valid opcode");
    };
}

void execute(std::array<int, memorySize>& memory,
    int* const acPtr, size_t* const icPtr,
    int* const irPtr, size_t* const opCodePtr,
    size_t* const opPtr,
    const std::vector<int>& inputs) {

    size_t inputIndex{ 0 }; // Tracks input

    do {
        //instruction counter to register
        *irPtr = memory[*icPtr];
        *opCodePtr = *irPtr / 100; // divide
        *opPtr = *irPtr % 100; // remainder

        switch (int word{}; opCodeToCommand(*opCodePtr)) {
        case Command::read:
            word = inputs[inputIndex];
            // Assign the value of 'word' to the memory location pointed to by 'opPtr'
            // Increment the instruction counter (icPtr) to point to the next instruction
            memory[*opPtr] = word;
            (*icPtr)++;
            inputIndex++;
            break;
        case Command::write:
            //Dereference 'icPtr' to access the instruction counter and increment its value by 1
            (*icPtr)++;
             // use the below cout if needed but comment before submission
            //std::cout << "Contents of " << std::setfill('0') << std::setw(2) 
            //          << *opPtr << " : " << memory[*opPtr] << "\n";
            break;
        case Command::load:
            //Load the value from the memory location pointed to by 'opPtr' into the accumulator (acPtr)
            *acPtr = memory[*opPtr];
            //Increment the instruction counter (icPtr) to point to the next instruction
            (*icPtr)++;
            break;
        case Command::store:
            // Store the value in the accumulator (acPtr) into the memory location pointed to by 'opPtr'
            memory[*opPtr] = *acPtr;
            // Increment the instruction counter (icPtr) to move to the next instruction
            (*icPtr)++;
            break;
        case Command::add:
            // Add the value in the accumulator (acPtr) to the value in memory at the location pointed to by 'opPtr' and store the result in 'word'
            word = *acPtr + memory[*opPtr];
            // If the result is valid, store it in the accumulator and increment the instruction counter
            if ((minWord <= word) && (maxWord >= word))
            {
                *acPtr = word;
            }
            else // / If the result is invalid, throw a runtime error
            {
                throw std::runtime_error("Reached limit");
            }
            (*icPtr)++;
            break;
        case Command::subtract:
            // Subtract the value in memory at the location pointed to by 'opPtr' from the value in the accumulator (acPtr) and store the result in 'word'
            word = memory[*opPtr] - *acPtr;
            // If the result is valid, store it in the accumulator and increment the instruction counter
            if ((minWord <= word) && (maxWord >= word))
            {
                *acPtr = word;
            }
            else // / If the result is invalid, throw a runtime error
            {
                throw std::runtime_error("Reached limit");
            }
            (*icPtr)++;
            break;
        case Command::multiply:
            // as above do it for multiplication
            word = memory[*opPtr] * *acPtr;
            // If the result is valid, store it in the accumulator and increment the instruction counter
            if ((minWord <= word) && (maxWord >= word))
            {
                *acPtr = word;
            }
            else // / If the result is invalid, throw a runtime error
            {
                throw std::runtime_error("Reached limit");
            }
            (*icPtr)++;
            break;
        case Command::divide:
            // as above do it for division
            word = *acPtr / memory[*opPtr]; // divide the accumulator by the memory of opPtr
            // If the result is valid, store it in the accumulator and increment the instruction counter
            if ((minWord <= word) && (maxWord >= word))
            {
                *acPtr = word;
            }
            else // / If the result is invalid, throw a runtime error
            {
                throw std::runtime_error("Reached limit");
            }
            (*icPtr)++;
            break;
        case Command::branch:
            *icPtr = *opPtr;
            break;
        case Command::branchNeg:
            *acPtr < 0 ? *icPtr = *opPtr : ++(*icPtr);
            break;
        case Command::branchZero:
            *acPtr == 0 ? *icPtr = *opPtr : ++(*icPtr);
            break;
        case Command::halt:
            break;
        default:
            // any instruction required
            break;
        };
        // You may modify the below while condition if required
    } while (opCodeToCommand(*opCodePtr) != Command::halt);
}

bool validWord(int word) {
    return word == static_cast<int>(Command::read) ||
        word == static_cast<int>(Command::write) ||
        word == static_cast<int>(Command::load) ||
        word == static_cast<int>(Command::store) ||
        word == static_cast<int>(Command::add) ||
        word == static_cast<int>(Command::subtract) ||
        word == static_cast<int>(Command::divide) ||
        word == static_cast<int>(Command::multiply) ||
        word == static_cast<int>(Command::branch) ||
        word == static_cast<int>(Command::branchNeg) ||
        word == static_cast<int>(Command::branchZero) ||
        word == static_cast<int>(Command::halt);
}

void dump(std::array<int, memorySize>& memory, int accumulator,
    size_t instructionCounter, size_t instructionRegister,
    size_t operationCode, size_t operand) {
    
    std::cout << "Registers" << std::endl;
    output("accumulator", 5, accumulator, true);
    output("instructionCounter", 2, instructionCounter, false);
    output("instructionRegister", 5, instructionRegister, true);
    output("operationCode", 2, operationCode, false);
    output("operand", 2, operand, false);

    std::cout << std::endl;
    std::cout << std::setfill(' ');
    std::cout << std::setw(2) << " ";
    for (size_t colNum = 0; colNum < 10; colNum++)
    {
        std::cout << std::setw(5) << colNum;
    }
    std::cout << std::endl; // after printing the column numbers index the cursor down

    //printing the row numbers
    for (size_t row = 0; row < memory.size(); row += 10) //memory.size is used to print out the exact amount of rows in memory
    {
        std::cout << std::setw(3) << row << " ";
        for (size_t col = 0; col < 10; col++)
        {
            std::cout << std::internal << std::showpos << std::setw(5) << std::setfill('0') << memory[row + col] << std::noshowpos;
        }
        std::cout << std::setfill(' ') << std::endl;
    }

}

void output(std::string label, int width, int value, bool sign)
{
    //std::cout << std::left << std::setw(20) << std::setfill(' ') << label << " " << (sign ? std::showpos : std::noshowpos)
    //    << std::setw(width);
    //std::cout << std::setfill('0');
    //    std::cout << value << std::noshowpos
    //    << std::endl;
    
    std::cout << std::left << std::setw(20) << std::setfill(' ') << label << " ";
    std::cout << std::internal << (sign ? std::showpos : std::noshowpos) << std::setw(width)
        << std::setfill('0') << value << "\n";

}