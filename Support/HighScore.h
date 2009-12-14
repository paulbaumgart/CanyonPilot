#include <iostream>
#include <stdio.h>

using namespace std;

void drawText(int, int, char*);

class HighScore {
  private:
  unsigned int highscore;
  double score;
  int offset;
  char* file;

  public:
  HighScore(char* filename) {
    file = filename;
    
    if (access(file, R_OK) != -1) {
      FILE* f = fopen(file, "r");
      fscanf(f, "%u", &highscore);
      cerr << "Current highscore: " << highscore << endl;
      fclose(f);
    }
    else {
      highscore = 0;
    }

    offset = ((int)(log(highscore)/log(10)) + 1) * 8;
    score = 0;
  }

  void draw() {
    char s[20];
    snprintf(s, 20, "Score: %u ", (int)score);
    drawText(s, kTopLeft);
    snprintf(s, 20, "High Score: %u", highscore);
    drawText(s, kTopRight);
  }
  
  void resetScore() {
    score = 0;
  }

  void updateScore(double elapsed) {
    score += elapsed * 10;
  }

  void finalize() {
    if ((int)score < highscore)
      return;
    
    FILE* f = fopen(file, "w");
    fprintf(f, "%u", (int)score);
    fclose(f);
    
    highscore = (int)score;
  }
};
