#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;

// creates a file
void create_file(string file_name, string contents){
  ofstream file(file_name);
  file << contents;
  file.close();
}

// removes the ending file extension of a file name
string rm_file_exten(string input){
  string output = "";
  int i = 0;
  char c = input[i];
  while (c != '.'){
    c = input[i++];
    if (c != '.')
      output += c;
  }
  return output;
}

// creates a bash rendering file
void create_render_file(){
  string contents =
    "#!/bin/bash\n"
    "file_pov=$1\n"
    "frame_num=$2\n"
    "width=$3\n"
    "height=$4\n"
    "frame_num_padded=$(printf \"%03d\" \"$frame_num\")\n"
    "chmod -x ${file_pov}\n"
    "./povray +I${file_pov} +Oframe${frame_num_padded}.png +K.${frame_num} +W${width} +H${height}\n";
  create_file("rend.sh", contents);

  string command = "chmod +x rend.sh";
  system(command.c_str());
}

// creates the submit file
void create_submit_file(string file_pov, int frames, string width, string height, string log_file, string out_file, string err_file, string out_folder){
  string command  = "mkdir " + out_folder;
  system(command.c_str());

  string contents =
    "executable = rend.sh\n"
    "arguments = " + file_pov + " $(Process) " + width + " " + height + "\n" 
    "log = ./" + out_folder + "/" + log_file + "\n"
    "output = ./" + out_folder + "/" + out_file + "\n"
    "error = ./" + out_folder + "/" + err_file + "\n"
    "\n"
    "transfer_input_files = povray, WRC_RubiksCube.inc, " + file_pov + "\n"
    "should_transfer_files = Yes\n"
    "when_to_transfer_output = ON_EXIT\n"
    "\n"
    "request_cpus = 1\n"
    "request_memory = 512M\n"
    "request_disk = 1G\n"
    "queue " + to_string(frames) + "\n";
  create_file("rend.sub", contents);
}

// creates bash file to compile all rendered frames into a video
void create_compile_file(){
    string contents =
    "#!/bin/bash\n"
    "log_folder=$1\n"
    "log_name=$2\n"
    "file_mp4=$3\n"
    "condor_wait ./${log_folder}/${log_name}\n"
    "./ffmpeg -r 10 -start_number 0 -i frame%03d.png -c:v libx264 -pix_fmt yuv420p ${file_mp4}\n"
    "rm rend.sh rend.sub compile.sh *.png\n";
    create_file("compile.sh", contents);

  string command = "chmod +x compile.sh";
  system(command.c_str());
}

// renders all frames
void render_frame(int num){
  string command = "condor_submit rend.sub";
  system(command.c_str());
}


int main(int argc, char* args[]){
  if (argc != 6) {
    cout << "condor_povray usage requires 5 inputs" << endl;
    return 0;
  }
 
  // receiving command line arguments 
  int frames = atoi(args[3]);  
  string file_pov = args[1];
  string file_mp4 = args[2];
  string width = args[4];
  string height = args[5];
  
  // creating log and error file names
  string pov_raw = rm_file_exten(file_pov);
  string log_name = pov_raw + ".log";
  string out_name = pov_raw + ".out";
  string err_name = pov_raw + ".err";
  string log_folder_name = pov_raw + "_outputs"; 

  cout << log_name + " " + out_name + " " + err_name + " " + log_folder_name << endl;

  // removing any old logs and results folders 
  string command = "rm -rf " + log_folder_name;
  system(command.c_str());

  // creates necessary files and submits job
  create_render_file();
  create_submit_file(file_pov, frames, width, height, log_name, out_name, err_name, log_folder_name);
  create_compile_file();

  // submitting the jobs
  command = "condor_submit rend.sub";
  system(command.c_str());
  
  // wait until all images rendered and then compiles final image
  command = "./compile.sh " + log_folder_name + " " + log_name + " " + file_mp4 + " &";
  system(command.c_str());
  
  //  command = "condor_wait " + log_folder_name + "/" + log_name + " &"; 
  //system(command.c_str());

  // running the main command
  //system(("./ffmpeg -r 10 -start_number 0 -i frame%03d.png -c:v libx264 -pix_fmt yuv420p " + file_mp4).c_str());

  return 0;
}
