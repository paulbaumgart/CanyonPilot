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
    
    FILE* f = fopen(file, "r");

    if (file) {
      fscanf(f, "%u", &highscore);
      cerr << "Current highscore: " << highscore << endl;
    }
    else {
      highscore = 0;
    }

    offset = ((int)(log(highscore)/log(10)) + 1) * 8;
    score = 0;

    fclose(f);
  }

  void draw() {
    char s[20];
    snprintf(s, 20, "Score: %u ", (int)score);
    drawText(5, 15, s, kTopLeft);
    snprintf(s, 20, "High Score: %u", highscore);
    drawText(104+offset, 15, s, kTopRight);
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
