#include "source.h"

transforml::transforml(std::string input){
    this->input = input;
    this->output = "";
    this->type = transformType::normal;
}

void transforml::outputl(){
    std::cout << this->input;
}

size_t transforml::getLen(){
    return this->input.length();
}

std::string transforml::getOutput() const{
    return this->output;
}
