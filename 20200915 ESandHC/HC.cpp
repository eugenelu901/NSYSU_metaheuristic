#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define BITSNUM 100
#define RUN 30
#define ITERATION 1000


using namespace std;
vector<float> fitness_Record_List;

bitset<BITSNUM> Initialization() {

  bitset<BITSNUM> iniset;

  for (int i = 0; i < BITSNUM; i++)
    iniset[i] = rand() % 2;

  return iniset;

}

bitset<BITSNUM> Transition( bitset<BITSNUM> bits ) {

  int num = (rand() % BITSNUM);

  if (bits[num] == 0)
    bits[num] = 1;
  else
    bits[num] = 0;

  return bits;

}

int Evaluation( bitset<BITSNUM> solution ) {

  int fitness = 0;

  for (int i = 0; i < BITSNUM; i++) {
    if (solution[i] == 1)
      fitness++;
  }

  return fitness;

}

void Determination( int &nowfitness, int &bestfitness, bitset<BITSNUM> &bestsolution, bitset<BITSNUM> &bits ) {
  if ( nowfitness > bestfitness ) {
    bestfitness = nowfitness;
    bestsolution = bits;
  }
}

bool ifDone ( bitset<BITSNUM> solution ) {

  int count = 0;

  for (int i = 0; i < BITSNUM; i++) {
    if (solution[i] == 1)
      count++;

    if (count == BITSNUM)
      return true;
  }

  return false;

}

void recordFitness( int bestfitness, int iteration ) {  //紀錄每個iteration的fitness

  if (fitness_Record_List.size() <
      iteration + 1)  //如果還沒有該iteration的紀錄 新增到vector裡
    fitness_Record_List.push_back(bestfitness);
  else  //如果有的話 把它加進應該加進的iteration裡 最後輸出時會平均
    fitness_Record_List[iteration] =
        fitness_Record_List[iteration] + bestfitness;
}

int main() {

  FILE *fp;
  fp = fopen("HCoutput.txt", "w");
  srand(time(0));

  int runtime = 0;

  while (runtime < RUN ) {

    bitset<BITSNUM> bits, bestsolution = Initialization(); //  I

    int nowfitness = 0, count = 0, bestfitness = Evaluation(bestsolution); //  E
    int countfitness = 0;

    while (count < ITERATION ) {

      bits = Transition(bestsolution);                            //  T
      nowfitness = Evaluation(bits);                              //  E
      Determination(nowfitness, bestfitness, bestsolution, bits); //  D

      recordFitness(bestfitness, count);                          //紀錄每iteration的fitness

      count++;

      //   cout << "iteration:" << count << " " << bestsolution << " " << "fitness:" << bestfitness << endl; // O
      countfitness = countfitness + bestfitness;
    }

    runtime++;

  }

  for (int i = 0 ; i < ITERATION ; i++)
    fprintf(fp, "%d\t%f\n", (i + 1), (fitness_Record_List[i] / RUN ));

  fclose(fp);

  cout << "done! output exported!" << endl;
}
