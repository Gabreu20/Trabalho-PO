
#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(void)
{
	float cidades[5][8][2]; //[5]-> index da cidade
							//[8]-> index do produto
							//[2]-> 0 = preço semente e 1 = preço de venda unitário

	float tabelaFinal[8][3]; //[8] -> index do produto
						     //[3] -> coluna [0]: preço semente mais barato; coluna [1]: preço sem. mais caro; coluna [2]: preço venda unitário mais caro


	int R[8] = { 6,6,7,8,9,10,11,12 };
	int S[8] = { 30,20,13,11,6,4,3,2 };

	

	/*Lê os valores do arquivo csv*/
	ifstream file;

	file.open("dados1.csv", ifstream::in);
	if (!file.is_open()) {
		cerr << "Incapaz de abrir o arquivo dados1.csv" << endl;
		return 1;
	}
	for (int cidade = 0; cidade < 5; cidade++) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 2; j++) {
				char t[10];
				file.getline(t, 10, ',');
				string saa = t;

				cidades[cidade][i][j] = stof(saa);
			}
		}
	}
	file.close();

	//preenche a tabelaFinal com os melhores valores
	float aux = 0;
	for (int i = 0; i < 8; i++) {//sementes mais baratas
		aux = cidades[0][i][0]; 
		tabelaFinal[i][0] = aux;
		for (int cidade = 0; cidade < 5; cidade++) {
			if (cidades[cidade][i][0] < aux) {
				aux = cidades[cidade][i][0];
				tabelaFinal[i][0] = aux;
			}
		}
	}
	for (int i = 0; i < 8; i++) {//sementes mais caras
		aux = cidades[0][i][0]; 
		tabelaFinal[i][1] = aux;
		for (int cidade = 0; cidade < 5; cidade++) {
			if (cidades[cidade][i][0] > aux) {
				aux = cidades[cidade][i][0];
				tabelaFinal[i][1] = aux;
			}
		}
	}
	for (int i = 0; i < 8; i++) {//venda unitaria mais cara
		aux = cidades[0][i][1]; 
		tabelaFinal[i][2] = aux;
		for (int cidade = 0; cidade < 5; cidade++) {
			if (cidades[cidade][i][1] > aux) {
				aux = cidades[cidade][i][1];
				tabelaFinal[i][2] = aux;
			}
		}
	}

	//Printa os valores da tabela final
	/*
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			cout << tabelaFinal[i][j] << " ";
		}
		cout << endl;
	}*/

	/* Printa os valores das cidades
	for (int cidade = 0; cidade < 5; cidade++) {
		cout << "Cidade: " << cidade + 1 << ": " << endl;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 2; j++) {
				cout << cidades[cidade][i][j] << " ";
			}
			cout << endl;
		}
	}*/

	/* declara as variaveis */
	glp_prob* lp;
	int ia[1 + 1000], ja[1 + 1000];
	double ar[1 + 1000]; 
	double z;
	double variaveis[16];

	/* cria o problema */
	lp = glp_create_prob();
	glp_set_prob_name(lp, "short");
	glp_set_obj_dir(lp, GLP_MAX);
	

	//adiciona as linhas lendo os valores das restrições do arquivo
	glp_add_rows(lp, 15);

	ifstream restricoes;

	restricoes.open("restricoes.csv", ifstream::in);
	if (!restricoes.is_open()) {
		cerr << "Incapaz de abrir o arquivo restricoes.csv" << endl;
		return 1;
	}
	for (int i = 1; i <= 15; i++) {
		
		char *n = new char[10];
		restricoes.getline(n, 10, ',');
		string num_t = n;

		glp_set_row_bnds(lp, i, GLP_UP, 0.0, stof(num_t));
		delete n;
	}
	restricoes.close();

	//adiciona as colunas
	glp_add_cols(lp, 16);

	for (int i = 1; i <= 8; i++) {
		glp_set_col_bnds(lp, i, GLP_LO, 0, 0);
		glp_set_obj_coef(lp, i, ((30 * R[i-1] * tabelaFinal[i-1][2]) - tabelaFinal[i-1][0])); //Xi((Ri*Ui)-Ci)
	}
	for (int i = 9; i <= 16; i++) {
		glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0);
		glp_set_obj_coef(lp, i, S[i-9] * tabelaFinal[i-9][1]); //Ai((Si*Vi)
	}


	//determina o coeficiente de cada restrição
	ifstream file_matriz;

	file_matriz.open("coef_matriz.csv", ifstream::in);
	if (!file_matriz.is_open()) {
		cerr << "Incapaz de abrir o arquivo coef_matriz.csv" << endl;
		return 1;
	}

	int index = 0;
	for (int i = 1; i <= 15; i++) {
		for (int j = 1; j <= 16; j++) {
			index++;
			char *n = new char[10];
			file_matriz.getline(n, 10, ',');
			string num_t = n;

			ia[index] = i, ja[index] = j, ar[index] = stof(num_t);
			delete n;
		}
	}
	file_matriz.close();

	//linha 1
	for (int i = 1; i <= 8; i++) {
		ia[i] = 1, ja[i] = i, ar[i] = tabelaFinal[i - 1][0];
	}


	glp_load_matrix(lp, 240, ia, ja, ar);


	/* resolve o problema */
	glp_simplex(lp, NULL);


	/* recover and display results */
	z = glp_get_obj_val(lp);
	for (int i = 1; i <= 16; i++) {
		variaveis[i - 1] = glp_get_col_prim(lp, i);
	}

	// Z - summation((Xi - Ai) * Si * Ci
	for (int i = 0; i < 8; i++) {
		z = z - ((variaveis[i] - variaveis[i+8]) * S[i] * tabelaFinal[i][0]); 
	}

	cout << "Z: " << z << endl;
	for (int i = 0; i < 8; i++) {
		cout << "x" << i + 1 << ": " << round(variaveis[i]) << "|| ";
	}
	cout << endl;
	for (int i = 8; i < 16; i++) {
		cout << "a" << i - 7 << ": " << round(variaveis[i]) << "|| ";
	}

	/* housekeeping */
	glp_delete_prob(lp);
	glp_free_env();

	ofstream myFile;
	myFile.open("resultado.csv");
	for (int i = 0; i < 8; i++) {
		myFile << "x" << i + 1 << ": " << round(variaveis[i]) << ",";
	}
	myFile << "\n";
	for (int i = 8; i < 16; i++) {
		myFile << "a" << i - 7 << ": " << round(variaveis[i]) << ",";
	}
	myFile << "\n";
	myFile << "Z: " << z << endl;
	myFile.close();

	return 0;
}