#pragma warning(disable : 4996)
#include "windows.h"
#include "time.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <sstream>
using namespace std;


int** matrix_inc(int** mass, int** num_reb, int count, int* num);
int** zagruzka(int n);
int** matrix(int n);
int most(int i, int** mass, int* vis, int* set, int* num, int* most_step, int start, int step, vector<vector<int>>& most_set, int* vec_iter);
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	vector<vector<int>> most_set;
	int max = 0, n,sled = 0, max_shag = -1, x = 0, l = 0;
	int* vis, * set;
	int** vertex = 0, ** max_mas, ** num_reb;
	while (l != 1 && l!=2) {
		printf("Выберите вид загрузки матрицы(1/2)");
		cin >> l;
		
	}
	if (l == 1) {
		printf("\nВведите порядок исходного графа: ");
		scanf_s("%d", &n);
		vertex = (int**)malloc(n * sizeof(int*));
		for (int i = 0; i < n; i++) {
			vertex[i] = (int*)malloc(n * sizeof(int));
		}
		vertex = matrix(n);
	}
	 if (l == 2) {
		char buff;
		int a;
		ifstream fin("filename.txt");
		fin >> buff;
		a = buff - '0';
		n = a;
		vertex = (int**)malloc(n * sizeof(int*));
		for (int i = 0; i < n; i++) {
			vertex[i] = (int*)malloc(n * sizeof(int));
		}
		fin.close();
		vertex = zagruzka(n);
	}
	
	
	printf("\nМатрица смежности :\n\n");


	// Вывод матрицы
	ofstream fout;
	fout.open("data.txt");
	fout << "Матрица смежности:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", vertex[i][j]);
			fout << vertex[i][j]<< " ";
		}
		fout << "\n";
		printf("\n");

	}
	
	
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			if (vertex[i][j] == 1) {
				max++;  // кол 1
			}
		}
	}
	//Выделяем память под матрицу инц
	num_reb = (int**)malloc(3 * sizeof(int*));
	for (int i = 0; i < 3; i++) {
		num_reb[i] = (int*)malloc(max * sizeof(int));
	}
	max_mas = matrix_inc(vertex, num_reb, n, &max);

	
	fout << "Матрица смежности рёбер:\n";
	printf("\nМатрица смежности рёбер\n");
	for (int i = 0; i < max; i++) {
		for (int j = 0; j < max; j++) {
			printf("%d ", max_mas[i][j]);
			fout << max_mas[i][j] << " ";
		}
		fout << "\n";
		printf("\n");

	}
	
	vis = (int*)malloc(max * sizeof(int));
	set = (int*)malloc(max * sizeof(int));
	for (int i = 0; i < max; i++) {
		for (int j = 0; j < max; j++) {
			vis[j] = 0;
		}
		vis[i] = 1;  // отмечаем стартовую вершину i = 1
		set[sled] = i; // 1
		sled = most(i, max_mas, vis, set, &max, &max_shag, i, sled, most_set, &x);
		sled = 0;
	} 
	
	
		
	printf("\n\nМаксимальное паросочетание графа:\n\n");
	
	vector<vector<int>>::iterator i = most_set.begin();
		//Выводим паросочетания из функции
	fout << "\nМаксимальные паросочетания:\n";
		for (vector<int>::iterator j = (*i).begin(); j != (*i).end(); ++j) {
			printf("%d-%d ", num_reb[1][(*j)] + 1, num_reb[2][(*j)] + 1);
			fout<< num_reb[1][(*j)] + 1<<"-"; 
			fout <<  num_reb[2][(*j)] + 1<<"\n";
		}

		cout << "\nНомера рёбер: ";
		for (vector<int>::iterator j = (*i).begin(); j != (*i).end(); ++j) {
			cout  << *j + 1<< " ; ";
		}

		
		fout.close();
}
int** matrix(int n) {
	int** mass, test_nulls = 0;
	mass = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		mass[i] = (int*)malloc(n * sizeof(int));
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j > i) {
				mass[i][j] = rand() % 2;
				mass[j][i] = mass[i][j];
			}
			else if (i == j) {
				mass[i][j] = 0;
			}
		}
	}
	return mass;
}

int ** zagruzka(int n) {
	int** vertex;
	char buff;
	int a;
	ifstream fin("filename.txt");
	fin >> buff;
	a = buff - '0';
	n = a;
	cout << "\nРазмерность матрицы " << n;
	
	vertex = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		vertex[i] = (int*)malloc(n * sizeof(int));
	} 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {

			fin >> buff;
			a = buff - '0';
			vertex[i][j] = a;
		}
	}



	fin.close();

	return vertex;

}

int** matrix_inc(int** mass, int** num_reb, int count, int* num) {
	int** mass_edge;

	mass_edge = (int**)malloc(*num * sizeof(int*));
	for (int i = 0; i < *num; i++) {
		mass_edge[i] = (int*)malloc(*num * sizeof(int));
	}
	for (int i = 0; i < *num; i++) {
		for (int j = 0; j < *num; j++) {
			mass_edge[i][j] = 0;
		}
	}
	cout << "\nКоординаты рёбер\n";
	int iter = 0;
	for (int i = 0; i < count; i++) {
		for (int j = i; j < count; j++) {
			if (mass[i][j] == 1) {
				num_reb[0][iter] = iter; //номер столбца
				num_reb[1][iter] = i; // координаты наличия рёбер между вершинами
				num_reb[2][iter] = j;
				iter++;
			
				cout << " " << iter << " " << i << " " << j << " \n";
			}
			
		}
		
	}
	// в пустую матрицу инц записываем получ 1
	for (int i = 0; i < *num; i++) {
		for (int j = i + 1; j < *num; j++) {
			if ((num_reb[1][i] == num_reb[1][j]) || (num_reb[1][i] == num_reb[2][j]) || (num_reb[2][i] == num_reb[1][j]) || (num_reb[2][i] == num_reb[2][j])) {
				mass_edge[i][j] = 1;
				mass_edge[j][i] = 1;
			}
		}
	}
	return mass_edge;
}

int most(int i, int** mass, int* vis, int* set, int* num, int* most_step, int start, int sled, vector<vector<int>>& most_set, int* vec_iter) {
	// поиск наиб паросоч
	
	for (i=0; i < *num; i++) {  //4
		if (mass[start][i] == 0 && vis[i] == 0) { //если в строке встречается 0 и вершина не посещена  0
			int schet = 0;
			for (int j = 0; j < sled + 1; j++) {
				if (mass[set[j]][i] == 0) {  // для каждой строки i-го элемента ищем 0  
					schet++;   //2
				}
			}
			// если вершина  
			if (schet == sled + 1) {   //1 u 1
				sled++;  //переходим к след элементу массива set 1
				set[sled] = i;  //помещаем не смежную другим(вершинам в set) 
				vis[i] = 1;  
				
			}
		}
	}
	
	
	//поиск максимального паросочетания
	if (sled > *most_step) {
		most_set.clear();
		*vec_iter = 0;
		most_set.push_back(vector<int>());
		for (int i = 0; i < sled + 1; i++) {
			most_set[*vec_iter].push_back(set[i]);  
		}
		*most_step = sled; //1
	}
	vis[set[sled]] = 0; //переходим к другой строке
	sled--;  
	return sled;
}
