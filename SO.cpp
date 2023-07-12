/* short.c */

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

	cidades[0][0][0] = 20.24;

	float tabelaFinal[8][3]; //[8] -> index do produto
						     //[3] -> coluna [0]: preço semente mais barato; coluna [1]: preço sem. mais caro; coluna [2]: preço venda unitário mais caro

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

	/*Printa os valores da tabela final
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

	/* declare variables */
	glp_prob* lp;
	int ia[1 + 1000], ja[1 + 1000];
	double ar[1 + 1000], z, x1, x2, x3, x4, x5, x6, x7, x8;

	/* create problem */
	lp = glp_create_prob();
	glp_set_prob_name(lp, "short");
	glp_set_obj_dir(lp, GLP_MAX);

	/* fill problem */
	glp_add_rows(lp, 8);
	glp_set_row_name(lp, 1, "s1");
	glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 2500);

	glp_set_row_name(lp, 2, "s2");
	glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 27);

	glp_set_row_name(lp, 3, "s3");
	glp_set_row_bnds(lp, 3, GLP_UP, 0.0, 15);

	glp_set_row_name(lp, 4, "s4");
	glp_set_row_bnds(lp, 4, GLP_UP, 0.0, 0);

	glp_set_row_name(lp, 5, "s5");
	glp_set_row_bnds(lp, 5, GLP_UP, 0.0, 60);

	glp_set_row_name(lp, 6, "s6");
	glp_set_row_bnds(lp, 6, GLP_UP, 0.0, 40);

	glp_set_row_name(lp, 7, "s7");
	glp_set_row_bnds(lp, 7, GLP_UP, 0.0, 63);

	glp_set_row_name(lp, 8, "s8");
	glp_set_row_bnds(lp, 8, GLP_UP, 0.0, 66);

	glp_add_cols(lp, 8);
	glp_set_col_name(lp, 1, "x1");
	glp_set_col_bnds(lp, 1, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 1, (6*tabelaFinal[0][2]) - tabelaFinal[0][0]); //Xi((Ri*Ui)-Ci)

	glp_set_col_name(lp, 2, "x2");
	glp_set_col_bnds(lp, 2, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 2, (6 * tabelaFinal[1][2]) - tabelaFinal[1][0]);

	glp_set_col_name(lp, 3, "x3");
	glp_set_col_bnds(lp, 3, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 3, (7 * tabelaFinal[2][2]) - tabelaFinal[2][0]);

	glp_set_col_name(lp, 4, "x4");
	glp_set_col_bnds(lp, 4, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 4, (8 * tabelaFinal[3][2]) - tabelaFinal[3][0]);

	glp_set_col_name(lp, 5, "x5");
	glp_set_col_bnds(lp, 5, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 5, (9 * tabelaFinal[4][2]) - tabelaFinal[4][0]);

	glp_set_col_name(lp, 6, "x6");
	glp_set_col_bnds(lp, 6, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 6, (10 * tabelaFinal[5][2]) - tabelaFinal[5][0]);

	glp_set_col_name(lp, 7, "x7");
	glp_set_col_bnds(lp, 7, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 7, (11 * tabelaFinal[6][2]) - tabelaFinal[6][0]);
	
	glp_set_col_name(lp, 8, "x8");
	glp_set_col_bnds(lp, 8, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, 8, (12 * tabelaFinal[7][2]) - tabelaFinal[7][0]);

	//determina o coeficiente de cada restrição
	//linha 1
	for (int i = 1; i <= 8; i++) { 
		ia[i] = 1, ja[i] = i, ar[i] = tabelaFinal[i-1][0];
	}
	//linha 2
	for (int i = 9; i <= 16; i++) {
		ia[i] = 2, ja[i] = i-8, ar[i] = 1;
	}
	//linha 3
	for (int i = 17; i <= 24; i++) {
		ia[i] = 3, ja[i] = i-16, ar[i] = 0;
	}
	//linha 4
	for (int i = 25; i <= 32; i++) {
		ia[i] = 4, ja[i] = i-24, ar[i] = -1;
	}
	//linha 5
	ia[33] = 5, ja[33] = 1, ar[33] = 6;
	ia[34] = 5, ja[34] = 2, ar[34] = 6;
	ia[35] = 5, ja[35] = 3, ar[35] = 0.0;
	ia[36] = 5, ja[36] = 4, ar[36] = 0.0;
	ia[37] = 5, ja[37] = 5, ar[37] = 0.0;
	ia[38] = 5, ja[38] = 6, ar[38] = 0.0;
	ia[39] = 5, ja[39] = 7, ar[39] = 0.0;
	ia[40] = 5, ja[40] = 8, ar[40] = 12.0;
	
	ia[41] = 6, ja[41] = 1, ar[41] = 0;
	ia[42] = 6, ja[42] = 2, ar[42] = 0;
	ia[43] = 6, ja[43] = 3, ar[43] = 0.0;
	ia[44] = 6, ja[44] = 4, ar[44] = 8.0;
	ia[45] = 6, ja[45] = 5, ar[45] = 0.0;
	ia[46] = 6, ja[46] = 6, ar[46] = 10.0;
	ia[47] = 6, ja[47] = 7, ar[47] = 0.0;
	ia[48] = 6, ja[48] = 8, ar[48] = 0.0;

	ia[49] = 7, ja[49] = 1, ar[49] = 0;
	ia[50] = 7, ja[50] = 2, ar[50] = 0;
	ia[51] = 7, ja[51] = 3, ar[51] = 7.0;
	ia[52] = 7, ja[52] = 4, ar[52] = 0.0;
	ia[53] = 7, ja[53] = 5, ar[53] = 9.0;
	ia[54] = 7, ja[54] = 6, ar[54] = 0.0;
	ia[55] = 7, ja[55] = 7, ar[55] = 0.0;
	ia[56] = 7, ja[56] = 8, ar[56] = 0.0;

	ia[57] = 8, ja[57] = 1, ar[57] = 0;
	ia[58] = 8, ja[58] = 2, ar[58] = 0;
	ia[59] = 8, ja[59] = 3, ar[59] = 0.0;
	ia[60] = 8, ja[60] = 4, ar[60] = 0.0;
	ia[61] = 8, ja[61] = 5, ar[61] = 0.0;
	ia[62] = 8, ja[62] = 6, ar[62] = 0.0;
	ia[63] = 8, ja[63] = 7, ar[63] = 11.0;
	ia[64] = 8, ja[64] = 8, ar[64] = 0.0;

	glp_load_matrix(lp, 64, ia, ja, ar);


	/* solve problem */
	glp_simplex(lp, NULL);
	/* recover and display results */
	z = glp_get_obj_val(lp);
	x1 = glp_get_col_prim(lp, 1);
	x2 = glp_get_col_prim(lp, 2);
	x3 = glp_get_col_prim(lp, 3);
	x4 = glp_get_col_prim(lp, 4);
	x5 = glp_get_col_prim(lp, 5);
	x6 = glp_get_col_prim(lp, 6);
	x7 = glp_get_col_prim(lp, 7);
	x8 = glp_get_col_prim(lp, 8);
	printf("z = %g; x1 = %g; x2 = %g; x3 = %g; x4 = %g; x5 = %g; x6 = %g; x7 = %g; x8 = %g;\n", z, x1, x2, x3,x4,x5,x6,x7,x8);
	/* housekeeping */
	glp_delete_prob(lp);
	glp_free_env();
	return 0;
}