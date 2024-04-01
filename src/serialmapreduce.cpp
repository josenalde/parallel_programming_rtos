/* by Josenalde Oliveira april2024
   a serial mapreduce - to make it parallel
   */

#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <string>

using namespace std;

#define T 4

int gotoLine(fstream& file, string tosearch, int id, int chunk) {
    string str;
    int x;
    int my_count = 0;
    int total_count = 0;
    int start = id * chunk;
    int end = start + chunk;
    file.seekg(ios::beg);

    //skip lines from the beginning
    for(int i=0; i<start; ++i) {
      file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    for(int i=start; i<end; ++i){
      getline(file, str);
      //cout << "ignoring line #: " << i << " with: " << str << endl; 
      size_t nPos = str.find(tosearch, 0); // first occurrence
      while(nPos != string::npos) {
        my_count++;
        nPos = str.find(tosearch, nPos + 1);
      }
      //cout << "Found " << my_count << " here...\n";
      total_count += my_count;
      my_count = 0;
      }
      return total_count;
}

int main() {
 
  fstream fin;
  fin.open("draculastoker.txt");
  
  if(!fin) {
    cout << "error in reading the file";
    return 1;
  }
  string linetext;
  int lines = 0;
  
  while (getline(fin, linetext)) {
    lines++;
    //cout << linetext << endl;
  }
  fin.close();
  
  cout << lines << " lines in this file...\n";
  int chunk = lines / T;

  fin.open("draculastoker.txt");

  int word_count = 0;
  for (int i = 0; i < T; i++) {
    word_count += gotoLine(fin, "DRACULA", i, chunk);
  }
  cout << "Word count: " << word_count << endl;

  /* get length of file: only for lines with the same size
  fin2.seekg (0, fin2.beg);
  fin2.seekg (0, fin2.end);
  int length = fin2.tellg();
  fin2.seekg (0, fin2.beg);
  
  cout << length << " bytes is the length of this file...\n";
  cout << chunk << " chunk size\n";
  */

  fin.close();

  return 0;
}