#include <bitset>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define ANTSNUM 30       //每一迭代螞蟻數量
#define PHENUM 0.9       //每隻螞蟻攜帶的費洛蒙數量
#define DONTCHOOSE 0.3   //不做輪盤的機率
#define ITERATION 1250   //迭代數量
#define alpha 1
#define beta 2
#define rho 0.1   //揮發率
#define RUN 30

using namespace std;

typedef vector<int> travel_set ;

typedef vector<double> double_set ;
typedef vector<travel_set> ants_List ;
typedef vector<double_set> double_Table ;

ants_List antsList;       //每一迭代的螞蟻陣列
double_Table dTable;      //距離表格
double_Table pTable;      //費洛蒙表格
double_set distanceList;  //每隻螞蟻該迭代走的距離

vector<int> Map_X;       //每個點的x座標
vector<int> Map_Y;       //每個點的y座標

bool ifaNewBestSolCreate = false ;
int citySize = 0;         //總城市數
double a = rho;           //費洛蒙揮發係數

void outputTable( double_Table table ){               //印出輸入參數之表格
	for (int i = 0; i < table.size(); i++ ){
		for (int j = 0; j < table[i].size(); j++ ){
			cout << table[i][j] << "\t";
		}
		cout << endl;
	}
}

void inputdata() {              //讀取地圖座標資料

	FILE *dataset ;
	dataset = fopen( "dataset.txt", "r") ;

	int tempvalue = 0 ;
	int count = 0;

	while(!feof(dataset)){
		count++;

		fscanf(dataset, "%d", &tempvalue) ;
		fscanf(dataset, "%d", &tempvalue) ;
		Map_X.push_back(tempvalue);
		fscanf(dataset, "%d", &tempvalue) ;
		Map_Y.push_back(tempvalue);

	}
	citySize = count;
	fclose(dataset);
}

double calDistance( int startX, int startY, int targetX, int targetY ){    //輸入兩點座標參數 計算兩點距離

	int disX = startX - targetX;
	int disY = startY - targetY;

	if( disX < 0 ) disX = disX * -1 ;               // 自己手動取個絕對值
	if( disY < 0 ) disY = disY * -1 ;

	return sqrt((pow(disX, 2) + pow(disY,2))) ;     //兩點距離公式

}

double randomR(){                        // 隨機產生0~1小數
	double r = rand() % 100 / (double)101 ;
	return r ;
}

void createDistanceTable(){             // 根據讀入的座標陣列去計算出每點的距離表格

	dTable.clear() ;

	double_set cleanset;

	for (int i = 0; i < citySize ; i++) {
		dTable.push_back(cleanset);
		for (int j = 0; j < citySize; j++ ){
			if( i == j ){
				dTable[i].push_back(0);
			} else {
				dTable[i].push_back(calDistance(Map_X[i], Map_Y[i], Map_X[j], Map_Y[j] ));
				// cout << "from " << Map_X[i] << " , " << Map_Y[i] << " to " << Map_X[j] << " , " << Map_Y[j] << endl ;
				// cout << calDistance(Map_X[i], Map_Y[i], Map_X[j], Map_Y[j]) << endl ;
			}

		}
	}
}

void iniPheromoneTable() {                 //初始化費洛蒙表格

	pTable.clear() ;

	double_set cleanset;

	for (int i = 0; i < citySize ; i++) {
		pTable.push_back(cleanset);
		for (int j = 0; j < citySize; j++ ){
				pTable[i].push_back(0.001);            //起始皆為1
		}
	}

}

void spreadPhermone( travel_set antHis, double tempPheromone ){     //根據輸入的行走紀錄去散播費洛蒙 (費洛蒙根據行走距離去計算)
	for (int i = 0; i < antHis.size()-1 ; i++ ){
		pTable[antHis[i]][antHis[i+1]] = pTable[antHis[i]][antHis[i+1]] + tempPheromone;        //散播費洛蒙
		pTable[antHis[i+1]][antHis[i]] = pTable[antHis[i+1]][antHis[i]] + tempPheromone;        //散播費洛蒙
		// cout << antHis[i]<<" to "<< antHis[i+1] << " " << pTable[antHis[i]][antHis[i+1]] << endl ;
	}
}

void updatePheromoneTable(){                         //呼叫這個function來依照一批新解去做費洛蒙表格的更新

	for (int i = 0; i < pTable.size(); i++ ){        //每次更新費洛蒙都進行費洛蒙揮發
		for (int j = 0; j < pTable[i].size(); j++ ){
			pTable[i][j] = pTable[i][j] * a ;
		}
	}

	double tempPheromone = 0;

	for (int i = 0; i < ANTSNUM; i++){               //依每隻螞蟻走的路徑 去計算他們每步會遺留下來的費洛蒙
		tempPheromone = PHENUM / distanceList[i];
		spreadPhermone( antsList[i], tempPheromone );
	}

}

void Initialization() {                 //初始化兩個table
	antsList.clear();
	distanceList.clear() ;
	createDistanceTable();
	iniPheromoneTable();
}

bool ifPassed( int &targetpoint, travel_set antRoadhis ){          //這個function用來判斷傳入點螞蟻是否走過

	for (int i = 0; i < antRoadhis.size(); i++ ){
		if(targetpoint == antRoadhis[i])
			return true;
	}

	return false ;
}

double calAllProbability( int startpoint, travel_set antRoadhis ) {                //計算公式中的分母

	double total = 0;

	for(int i = 0 ; i < citySize ; i++ ){
		if( i != startpoint && !ifPassed( i, antRoadhis ) ){
			total = total + (pow(pTable[startpoint][i], alpha ) * pow((1 / dTable[startpoint][i]), beta ));      // 機率公式
		}
	}

	return total;

}


double calProbability( int startpoint, int endpoint, travel_set antRoadhis, double AllProbability ) {              //利用公式計算選到這點的機率並回傳
	// cout << ((pTable[startpoint][endpoint]) * (1 / dTable[startpoint][endpoint])) / calAllProbability(startpoint) << endl;
	return ((pow(pTable[startpoint][endpoint], alpha) * pow((1 / dTable[startpoint][endpoint]), beta )) / AllProbability );
}

void chooseRoad( int startpoint, int &targetpoint, travel_set antRoadhis ){         //使用輪盤法則去選擇一個新的目的地

	double_set Probabilitylist ;
	vector<double> intProbabilityList;

	double AllProbability = calAllProbability( startpoint, antRoadhis ) ;
	double temp = 0;
	double totalproperty = 0;

	for (int i = 0; i < citySize; i++ ){                    // 計算每個點被選到機率 並加入Probabilitylist陣列裡
		if(startpoint == i || ifPassed( i, antRoadhis )){     // 如果是自己或是已經走過的點 就把機率設為零(不可能會走到了)
			Probabilitylist.push_back( 0 );
		}
		else {
			Probabilitylist.push_back( calProbability(startpoint, i, antRoadhis, AllProbability ) );       // 如果沒走過，就把機率加入
		}
	}

	// for (int i = 0; i < citySize; i++ ){          // 這個for迴圈把目前所有機率都變成interger
	// 	temp = Probabilitylist[i];
	// 	intProbabilityList.push_back(temp);
	// 	totalproperty = totalproperty + intProbabilityList[i];   //計算總共的機率和
	// 	// cout << intProbabilityList[i] << endl ;
	// }

	// 所以i機率是 intProbabilityList[i] / totalproperty  若走過分子為0 加在分母內也是0 不可能會選到

	// cout << totalproperty << endl << endl ;

	double dart = randomR() ;   // 射飛鏢

	// cout << dart << endl ;
	temp = 0.0 ;

	for(int i = 0 ; i < citySize ; i++ ){            // 這個迴圈判斷上面射的飛鏢射在哪點的機率範圍內
		temp = temp + Probabilitylist[i] ;
		if( dart < temp ){                            //找到射在哪裡了 停止迴圈判斷 並存到targetpoint 作為螞蟻下一點的目的地
			// cout << temp << "--" << dart << endl << i << endl ;
			targetpoint = i;
			break;
		}
	}

}

void chooseCloseRoad(	int startpoint, int &targetpoint, travel_set antRoadhis){     //選最近的路
	double closest = -1 ;

	for ( int i = 0 ; i < citySize ; i++ ){
		if(startpoint != i && !ifPassed( i, antRoadhis ) ){
			if( closest == -1 || dTable[startpoint][i] < closest ){
				closest = dTable[startpoint][i];
				targetpoint = i;
			}
		}
	}

}

travel_set SolutionConstruction( int startpoint ){      //回傳一隻螞蟻找到的新解

	travel_set antRoadhis;
	int targetpoint = 0;
	double ifchoose = 0;

	antRoadhis.push_back(startpoint);                   // 第一站一定是起點

	for (int i = 0; i < citySize-1; i++ ) {
		ifchoose = randomR();                              //隨機取得一個0~1的小數

		if(ifchoose < DONTCHOOSE ) {                       //有機率不做輪盤 上面可以define可以設定
			chooseCloseRoad(antRoadhis[i], targetpoint, antRoadhis);
		}
		else{
			chooseRoad( antRoadhis[i], targetpoint, antRoadhis );       //藉for迴圈使用輪盤法則去選擇一個新的目的地，直到所有點都走過為止
		}

		antRoadhis.push_back(targetpoint);           //將要走的這點加入歷史
	}

	antRoadhis.push_back(startpoint);                   // 最後一站也是起點

	// for (int i = 0; i < antRoadhis.size() ; i++ )
	// 	cout << antRoadhis[i]+1 << " " ;

	// cout << endl ;

	return antRoadhis;

}

ants_List antsGoToTravel(){                      // 讓全部螞蟻出發 去尋找新的解 並回傳

	ants_List newAntsTravelList;

	for (int i = 0; i < ANTSNUM ; i++){
		newAntsTravelList.push_back(SolutionConstruction(0));
	}

	return newAntsTravelList;

}

double calDistance( travel_set antHis ){         //計算一隻螞蟻的行走距離
	double totaldis = 0;

	for (int i = 0; i < antHis.size()-1 ; i++ ){
		totaldis = totaldis + dTable[antHis[i]][antHis[i+1]];
		// cout << antHis[i]<<"to"<< antHis[i+1] << " " << dTable[antHis[i]][antHis[i+1]] << endl ;
	}

	return totaldis;
}

double_set getNewDistanceList() {               // 去計算並回傳該批螞蟻的所有距離

	double_set tempDistanceList;

		for(int i = 0; i < ANTSNUM ; i++)
			tempDistanceList.push_back(calDistance( antsList[i] ));

	return tempDistanceList;

}

int chooseBestSolution( int BestDistance , travel_set &BestSolution){       //尋找螞蟻中的最佳解並回傳

	for (int i = 0; i < distanceList.size(); i++ ){
		if(BestDistance == -1 || distanceList[i] < BestDistance){
			BestDistance = distanceList[i];
			BestSolution = antsList[i];
			ifaNewBestSolCreate = true ;
		}
	}

	return BestDistance;

}

void twoOpt( int &BestDistance , travel_set &BestSolution){       //解2-opt

	travel_set tempsolution = BestSolution ;
	int tempcity = 0 ;
	int tempdistance = BestDistance ;
	double tempPheromone = 0;

	for (int i = 0; i < BestSolution.size(); i++ ){
		for( int j = 0 ; j < BestSolution.size(); j++ ) {

			if( i < j ) {

				int tempi = i ;
				int tempj = j ;

				while ( tempi != tempj-1 && tempi != tempj ) {
					tempi++;
					tempj--;

					tempcity = tempsolution[tempi];                           //倒序試試看
					tempsolution[tempi] = tempsolution[tempj];
					tempsolution[tempj] = tempcity;

				}

			}
			else if ( i > j ) {

				int tempi = i ;
				int tempj = j ;

				while ( tempj != tempi-1 && tempj != tempi ) {

					tempj++;
					tempi--;

					tempcity = tempsolution[tempi];
					tempsolution[tempi] = tempsolution[tempj];
					tempsolution[tempj] = tempcity;

				}

			}

			tempdistance = calDistance(tempsolution);

			if( tempdistance < BestDistance ) {                       //如果比目前最佳解更好 就取代最佳解
				BestDistance = tempdistance ;
				BestSolution = tempsolution ;

				tempPheromone = PHENUM / BestDistance ;
				spreadPhermone( BestSolution, tempPheromone ) ;

			}

			tempsolution = BestSolution ;                             //如果沒有比較好就還原原來的最佳解

		}
	}

	ifaNewBestSolCreate = false ;

}

int GetXinMap( int targetpoint ) {
	return Map_X[targetpoint] ;
}

int GetYinMap( int targetpoint ) {
	return Map_Y[targetpoint] ;
}


int main() {
	srand(time(NULL));
	inputdata() ;

	FILE *fp;

	fp = fopen("ACO_Route.txt", "w");
	int averageDistance = 0 ;
	int runcount = 0 ;
	int globalBestDistance = -1 ;
	travel_set BestSolution;
	travel_set globalBestSolution;

	while ( runcount < RUN ) {

		Initialization();
		int count = 0 ;
		int BestDistance = -1 ;

		antsList = antsGoToTravel() ;            //創造一組新解
		distanceList = getNewDistanceList();     //更新每隻螞蟻的路徑距離
		BestDistance = chooseBestSolution(BestDistance, BestSolution ) ;   // 找該批螞蟻中最短距離
		updatePheromoneTable();                  // 更新費洛蒙表格

		while( count < ITERATION ){

			antsList = antsGoToTravel() ;          //創造一組新解
			distanceList = getNewDistanceList();   //更新每隻螞蟻的路徑距離
			BestDistance = chooseBestSolution(BestDistance, BestSolution );  // 找該批螞蟻中最短距離
			updatePheromoneTable();                // 更新費洛蒙表格

			if( ifaNewBestSolCreate ) {
				twoOpt( BestDistance , BestSolution);
			}

			if( globalBestDistance == -1 || BestDistance < globalBestDistance ) {
				globalBestSolution = BestSolution ;
				globalBestDistance = BestDistance ;
			}

			// cout << "iteration: "<< count+1 << "   BestDistance: " << BestDistance << endl ;    // 輸出當前最佳解
			// for (int i = 0; i < BestSolution.size() ; i++ )
			// 	cout << BestSolution[i]+1 << " " ;
			// cout << endl ;
			// cout << BestSolution.size() << endl ;

			count++;

		}

		cout << "RUN " << runcount+1 << "DONE!!   bestdistance : " << BestDistance << endl ;

		averageDistance = averageDistance + BestDistance ;
		runcount++ ;

	}

	// outputTable(pTable);         //輸出表格 debug 用
	int printx = 0 ;
	int printy = 0 ;

	for ( int i = 0; i < globalBestSolution.size() ; i++ ) {
		fprintf(fp, "%d\t%d\n",GetXinMap(globalBestSolution[i]) , GetYinMap(globalBestSolution[i]) );       // O
	}

	fclose(fp);

	cout << "---------done----------" << endl ;
	cout << "bestDistance : " << globalBestDistance << endl ;
	cout << "averageDistance : " << averageDistance / 30 << endl ;
	cout << "bestSolution : " << endl ;
	for (int i = 0; i < globalBestSolution.size() ; i++ )
		cout << globalBestSolution[i]+1 << " " ;
	cout << endl ;

	system("pause") ;

}
