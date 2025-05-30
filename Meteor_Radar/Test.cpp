#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

//#include "ioda/C/ioda_group_c.hpp"
//#include "ioda/C/ioda_engines_c.hpp"
//#include "ioda/C/ioda_vecstring_c.hpp"
std::vector<std::string> tokenizer(std::string inputString) {
    //tolkenize words between whitespace 
    std::istringstream iss(inputString);
    std::string word;
    std::vector<std::string> words;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

int maxrowsMPD(std::string file_name){
    //coun the number of lines in the file and return it as an int
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
    //open file 
    std::ifstream file(file_name);
    std::string line;
    // set pu conting for index
    int count = 0;
    while (std::getline(file, line)) {
        count ++;
        int index = count-data_start;
        if (index >= max_rows){
            std::cout << "exceeding the number of max rows, breaking" << std::endl;
            break;};
        if (count >= data_start){
            std::vector<std::string> words;
            words = tokenizer(line);// split line into words (split between blank spaces)

            //retreve nessisary values and put convert them into floats
            std::string amb(words[9]);
            float range = std::stof(words[3]);
            float height = std::stof(words[4]);
            float Vrad = std::stof(words[5]);
            float delVr = std::stof(words[6]);
            float Theta = std::stof(words[7]);
            float Phi0 = std::stof(words[8]);
            float Ambiguity = std::stof(words[9]);
            if(amb.find('.') != std::string::npos){
                range = std::stof(words[2].substr(5,10));
                height = std::stof(words[3]);
                Vrad = std::stof(words[4]);
                delVr = std::stof(words[5]);
                Theta = std::stof(words[6]);
                Phi0 = std::stof(words[7]);
                Ambiguity = std::stof(words[8]);
            }
                
            // Insert nessisary values output array
            (*oupt)[index][0] = range;
            (*oupt)[index][1] = height;
            (*oupt)[index][2] = Vrad;
            (*oupt)[index][3] = delVr;
            (*oupt)[index][4] = Theta;
            (*oupt)[index][5] = Phi0;
            (*oupt)[index][6] = Ambiguity;
            // if(amb.find('.') != std::string::npos){
            //     std::cout << range << "\t" << Phi0 << "\t" << Ambiguity << std::endl;
            // }
        };
        
    };
    file.close();
}

void alt_boundries(float* alts_boundaries, int N_alts, float lower, float upper){
    float Delt = upper-lower;
    float delt = Delt/N_alts;
    alts_boundaries[0] = lower;
    alts_boundaries[N_alts] = upper;
    for (int i = 1; i<N_alts; i++){
        alts_boundaries[i]=lower+(i)*delt;
    }
}
void swap(float (* arr)[7] , int i1, int i2){
    float temp [7];
    int val = 7;
    for (int i = 0; i<val; i++){
        temp[i] = arr[i1][i];
    }
    for (int i = 0; i<val; i++){
        arr[i1][i] = arr[i2][i];
    }
    for (int i = 0; i<val; i++){
        arr[i2][i] = temp[i];
    }

    
}
int partition(float (* arr)[7] , int low, int high) {
    float pivot = arr[high][1];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j][1] <= pivot) {
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, i+1, high);
    return i + 1;
}

void quickSort(float (* arr)[7],  int low, int high) {
    //if (low ==0) std::cout << arr[0][1] << std::endl;
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void print_first_and_last(float (* vals)[7], int rows_max){
      for (int i = 0; i<=10; i++){
        std::cout << vals[i][0] << "\t" << vals[i][1] <<"\t" << vals[i][2] <<"\t" << vals[i][3] <<"\t"  << vals[i][4]<<"\t"  << vals[i][5]<<"\t"  << vals[i][6] <<std::endl;
    }
    std::cout << "..." << std::endl;
    int i = rows_max -30 ;
    std::cout << vals[i][0] << "\t" << vals[i][1] <<"\t" << vals[i][2] <<"\t" << vals[i][3] <<"\t"  << vals[i][4]<<"\t"  << vals[i][5]<<"\t"  << vals[i][6] <<std::endl;
}

void Get_Wind_Velo_at_Alts(float (* vals)[7],float(* Wind_Velo_at_alt)[3], float* alt_midpoints,float* alts_boundaries,int N_alts, int index_max){
    int lower_i = 0;
    float lower;
    float upper;
    float V;
    float sigma;
    float theta;
    float phi;
    float amb;
    float den;
    float weight;
    float V_n;
    float V_e;
    float V_v;
    for (int i = 0; i<N_alts; i++){
        lower = alts_boundaries[i];
        upper = alts_boundaries[i+1];
        while(vals[lower_i][1]<lower){
            lower_i ++;
        }
        weight = 0;
        V_n = 0;
        V_e = 0;
        V_v = 0;
        //std::cout << lower_i << std::endl;
        while(lower_i<index_max && vals[lower_i][1]<upper){
            //std::cout << vals[lower_i][1] << "\t" << upper << std::endl;
            V = vals[lower_i][2];
            sigma = vals[lower_i][3]*M_PI/180;
            theta = vals[lower_i][4]*M_PI/180;
            phi = vals[lower_i][5];
            amb = vals[lower_i][6];
            den = amb*sigma;
            weight += 1;
            V_n += V*std::sin(theta)*std::sin(phi);
            V_e += V*std::sin(theta)*std::cos(phi);
            V_v += V*std::cos(theta);
            lower_i ++;
        }
        alt_midpoints[i] = (upper-lower)/2+lower;
        Wind_Velo_at_alt[i][0] = V_n/weight;
        Wind_Velo_at_alt[i][1] = V_e/weight;
        Wind_Velo_at_alt[i][2] = V_v/weight;
    }
}
int main(){
    // ==================== definitions ====================
    int N_alts = 5;
    int data_start = 30;
    float lower = 70;
    float upper = 110;
    
    int rows_max = maxrowsMPD("mp20230106.riogrande.mpd");// get the number of rows in the file 
    int index_max = rows_max - data_start;// define maximum index for data 

    // ==================== allocations ====================
    float (*vals)[7] = (float (*)[7])malloc(sizeof(float[rows_max][7]));// allocate data array
    float alts_boundaries[N_alts+1]; // allocate altidude boundary array
    float Wind_Velo_at_alt[N_alts][3];
    float alt_midpoints[N_alts];
    // ==================== main body ====================
    readMPD("mp20230106.riogrande.mpd",&vals,rows_max, data_start);//get data from file 
    alt_boundries(alts_boundaries,N_alts,lower,upper);// find the boundary altitudes 
    quickSort(vals,  0, index_max);// sort data by height 
    Get_Wind_Velo_at_Alts(vals,Wind_Velo_at_alt,alt_midpoints,alts_boundaries,N_alts,index_max );
    std::cout << "alt-midpoin(km),   V_n \t \t V_e \t\t V_v "  << std::endl;
    for (int i = 0; i<N_alts; i++){
        std::cout << "\t"  << alt_midpoints[i]<< "\t" << Wind_Velo_at_alt[i][0] << "  \t" << Wind_Velo_at_alt[i][1] << "  \t" << Wind_Velo_at_alt[i][2]  << std::endl;
    }
    free(vals);
    return(0);
}
