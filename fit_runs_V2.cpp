#include "string"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SRCFit_V4.cpp"
#include "SPEFit.cpp"
using namespace std; 

const string input_file = "testing.txt";


void extract_vales(string str, int &run);

void fit_runs_V2(){
    ifstream file(input_file); //Change this if input file is named diffrently
    string str;
    string file_loc;
    int current_run;
    float fit(0);
    ofstream outfile ("ouput_"+input_file); //output file name

    //keep track of previous two runs
    string word = "";
    string my_run = "";
    string run_prior = "";
    string run_priorto = "";
    string my_pmt = "";
    string type = "";
    string type_prior = "";
    string type_priorto = "";
    string pmt_prior = "";
    string pmt_priorto = "";
    
    string my_chanel = "";
    string chanel_prior = "";
    string chanel_priorto = "";

    
    while (getline(file,str)){ //read file line by line
        
        int my_counter(0);
        for (auto x : str){
            if (x == '\t'){
                    my_counter++;
                    if(my_counter == 2){
                        run_priorto = run_prior;
                        run_prior = my_run;
                        my_run = word;
                        //cout << word << endl;
                    }
                    else if (my_counter == 3){
                        pmt_priorto = pmt_prior;
                        pmt_prior = my_pmt;
                        my_pmt = word;
                        //cout << word << endl;
                
                    }
                    else if (my_counter == 4){
                        if(word == "1" || word == "2"){
                            chanel_priorto = chanel_prior;
                            chanel_prior = my_chanel;
                            my_chanel = word;
                            //cout << word << endl;
                        }
                        else{word = "";}              
                    }
                    else if (my_counter==10)
                    {
                        type_priorto = type_prior;
                        type_prior = type;
                        type = word;
                        //cout << word << endl;
                    }
                    //cout << my_counter << endl;
                    word = "";
            }
            else {
                    word = word + x;
            }
        }

        //fit the data
        if(type == "No"){
            try{
                //CODE FOR NO SOURCE FIT
                //outfile <<" fit " << my_run << " to no source ";
                outfile << SPEFit(my_run, my_chanel)<<endl;
            }
            catch(...){outfile << "FAILED\n";}
        }
        else if(type == "Cd" && type_prior == "No" && my_pmt ==pmt_prior){
            try{
                //SOURCE FIT
                //outfile <<" fit " << my_run << " to source 1 ";
                outfile << SRCFit_V4(run_prior, chanel_prior, my_run, my_chanel)<<endl;
            }
            catch(...){outfile << "FAILED\n";}
        }
        else if(type == "Cd" && type_priorto == "No" && my_pmt ==pmt_priorto){
            try{
                //SOURCE FIT
                //outfile <<" fit " << my_run << " to source 2 ";
                outfile<< SRCFit_V4(run_priorto, chanel_priorto, my_run, my_chanel)<<endl;
            }
            catch(...){outfile << "FAILED\n";}
        }
        else{
            outfile << "RUN " + my_run + " was not fitted \n";
        }
        /*
        if(current_run > 1000){
            outfile << fit<< endl;
            //delete file
            }*/

    
    }
    outfile.close();
    return;
}


