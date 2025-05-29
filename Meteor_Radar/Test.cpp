#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//#include "ioda/C/ioda_group_c.hpp"
//#include "ioda/C/ioda_engines_c.hpp"
//#include "ioda/C/ioda_vecstring_c.hpp"
std::vector<std::string> tokenizer(std::string inputString) {
    std::istringstream iss(inputString);
    std::string word;
    std::vector<std::string> words;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

int maxrowsMPD(std::string file_name){
    //std::cout << "here" << std::endl;
    std::ifstream file(file_name);
    std::string line;
    int count = 0;
    int excess_count = 0;
    while (std::getline(file, line)) {
        count ++;
    };
    file.close();
    return(count);
}

void readMPD(std::string file_name, float (** oupt)[7] , int max_rows, int data_start){
    //std::cout << "here" << std::endl;
    //float (*oupt)[7] = (float (*)[7])malloc(sizeof(float[max_rows][7]));
    std::ifstream file(file_name);
    std::string line;
    int count = 0;
    int excess_count = 0;
    while (std::getline(file, line)) {
        count ++;
        int index = count-data_start;
        if (index >= max_rows){
            std::cout << "exceeding the number of max rows, breaking" << std::endl;
            break;};
        if (count >= data_start){
            std::vector<std::string> words;
            //std::cout << line << std::endl;
            words = tokenizer(line);
            //std::cout << words[0] << "\t" << words[1] <<"\t" << words[2] <<"\t" << words[3] <<std::endl;
            float range = std::stof(words[3]);
            float height = std::stof(words[4]);
            float Vrad = std::stof(words[5]);
            float delVr = std::stof(words[6]);
            float Theta = std::stof(words[7]);
            float Phi0 = std::stof(words[8]);
            float Ambiguity = std::stof(words[9]);
            //std::cout << range << "\t" << height << "\t" << Vrad << "\t" << delVr << "\t" << Theta << "\t" << Phi0 << "\t" << Ambiguity << std::endl;
            (*oupt)[index][0] = range;
            (*oupt)[index][1] = height;
            (*oupt)[index][2] = Vrad;
            (*oupt)[index][3] = delVr;
            (*oupt)[index][4] = Theta;
            (*oupt)[index][5] = Phi0;
            (*oupt)[index][6] = Ambiguity;
        };
        
    };
    file.close();
}

int main(){
    int N_alts = 3;
    int data_start = 30;
    int rows_max = maxrowsMPD("mp20230106.riogrande.mpd");
    float (*vals)[7] = (float (*)[7])malloc(sizeof(float[rows_max][7]));
    //float vals[rows_max][7];
    readMPD("mp20230106.riogrande.mpd",&vals,rows_max, data_start);
    // for (int i = 0; i<=10; i++){
    //     std::cout << vals[i][0] << "\t" << vals[i][1] <<"\t" << vals[i][2] <<"\t" << vals[i][3] <<"\t"  << vals[i][4]<<"\t"  << vals[i][5]<<"\t"  << vals[i][6] <<std::endl;
    // }
    // int i = rows_max -30 ;
    // std::cout << vals[i][0] << "\t" << vals[i][1] <<"\t" << vals[i][2] <<"\t" << vals[i][3] <<"\t"  << vals[i][4]<<"\t"  << vals[i][5]<<"\t"  << vals[i][6] <<std::endl;
    int index_max = rows_max - data_start;



    free(vals);
    return(0);
}
