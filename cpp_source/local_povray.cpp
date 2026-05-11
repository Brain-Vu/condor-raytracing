#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc, char* args[]){
  if (argc != 6) {
    cout << "local_povray usage requires 5 inputs" << endl;
    return 0;
  }
  
  string filePov = args[1];
  string fileMp4 = args[2];
  int frames = atoi(args[3]);
  string width = args[4];
  string height = args[5];

  // creating all the pngs
  string command = "";
  string num_str;
  for (int i = 0; i < frames; i++){
    if (i < 10)
      num_str = "00" + to_string(i);
    else if (i >= 10 && i < 100)
      num_str = "0" + to_string(i);
    else if (i >= 100 && i < 1000)
      num_str = to_string(i);
    else {
      cout << "Frame count too high! Did not render" << endl;
      return 0;
    }

    command = "./povray +I" + filePov + " +Oframe" + num_str + ".png +K." + to_string(i) + " +W" + width + " +H" + height; 
    cout << "running: " << command << endl;
    system(command.c_str());
  }
  
  // running the main command
  system(("./ffmpeg -r 10 -start_number 0 -i frame%03d.png -c:v libx264 -pix_fmt yuv420p " + fileMp4).c_str());

  // cleaning up all rendered pngs
  system("rm *.png");
    
  return 0;
}
