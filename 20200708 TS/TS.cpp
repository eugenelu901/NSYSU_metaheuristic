#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define BITSNUM 100  // bit的數量

//呂季澤 7/8 TS 作業

using namespace std;
typedef bitset<BITSNUM> bits_set;

vector<float> fitness_Record_List;  //紀錄最佳解 畫圖用
vector<bits_set> tabuList;          //禁忌的陣列

bool ifInTabu(bits_set solution) {  //判斷這個新解是否在禁忌的陣列裡

  for (int i = 0; i < tabuList.size(); i++) {
    if (solution == tabuList[i]) return true;
  }

  return false;
}

bits_set Initialization() {  //做一個隨機的初始解

  bits_set iniset;

  for (int i = 0; i < BITSNUM; i++)  // 用判斷隨機數為奇數偶數來隨機賦予值
    iniset[i] = rand() % 2;

  tabuList.push_back(iniset);  //將起點的解加入禁忌陣列裡
  return iniset;
}

bits_set Transition(bits_set solution) {
  while (ifInTabu(
      solution)) {  //如果產生的解是在禁忌陣列裡的就繼續找到沒在裡面的解為止
    int num = (rand() % BITSNUM);  //取隨機數

    if (solution[num] == 0)  // 把隨機選到的那個改掉，0改1，1改0
      solution[num] = 1;
    else
      solution[num] = 0;
  }

  return solution;
}

int Evaluation(bits_set solution) {
  int fitness = 0;

  for (int i = 0; i < BITSNUM; i++) {
    if (solution[i] == 1) fitness++;  // 數看看1有幾個，看fitness多少
  }

  return fitness;  //回傳 fitness
}

void Determination(int &nowfitness, int &bestfitness, bits_set &bestsolution, bits_set &bits) {
  if (nowfitness >
      bestfitness) {  //去比較最新產生的解的fitness有沒有比目前最佳解好
    bestfitness = nowfitness;
    bestsolution = bits;
    if (tabuList.size() <
        100) {  //陣列超過100個就把陣列第一個(比較舊的)刪掉  queue的上限為100
      tabuList.push_back(bestsolution);  //將走到的這個解加入禁忌陣列
    } else {
      tabuList.erase(tabuList.begin());
      tabuList.push_back(bestsolution);
    }
  }
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
  fp = fopen("TSoutput.txt", "w");
  srand(time(0));

  int runtime = 0;

  while (runtime < 30) {

    bits_set bits, bestsolution = Initialization();  //  I

    int nowfitness = 0, count = 0,
        bestfitness = Evaluation(bestsolution);  //  E

    while (count < 1000) {  //跑 iteration 1000

      bits = Transition(bestsolution);                             //  T
      nowfitness = Evaluation(bits);                               //  E
      Determination(nowfitness, bestfitness, bestsolution, bits);  //  D

      recordFitness(bestfitness, count);  //紀錄每iteration的fitness

      count++;
      // cout << "iteration: " << count << "   fitness: " << bestfitness << endl
      // << "solution: " << bestsolution << endl ;   // O
    }

    tabuList.clear();  // 將禁忌陣列清空 以進行下一次run
    runtime++;
  }

  for (int i = 0; i < 1000; i++)
    fprintf(fp, "%d\t%f\n", (i + 1), (fitness_Record_List[i] / 30));

  fclose(fp);

  cout << "done! output exported!" << endl;
}
