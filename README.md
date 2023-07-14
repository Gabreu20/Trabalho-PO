# TRABALHO PESQUISA OPERACIONAL

**MODELO DO TRABALHO**

Em uma fazenda, o dono é agricultor e pode produzir cenoura, feijão, trigo, nabo, repolho, batata, milho, 
abóbora em seus 27m² de terra, sendo que cada semente vai ser plantada em 1m². A colheita de todas as 
sementes é feita a cada um mês após o plantio e cada cultura tem seu respectivo rendimento em legumes 
por semente. O fazendeiro não pode sobrecarregar o mercado de um determinado produto, assim ele notou 
que não consegue vender mais de uma certa quantidade de unidades do mesmo produto em um mês, pois eles 
apodrecem antes de serem vendidos. Além disso, cada semente vai gerar uma nova semente em determinado 
período dependendo do seu tipo e se for usado ou não o adubo. Ele tem adubo disponível para adubar 
apenas 15 pedaços de 1m² de terra, que serão os mesmos durante todo o período de tempo. Cada pedaço de 
terra, após inicialmente usado, não pode mudar de cultura. O objetivo do agricultor é maximizar seu lucro 
em um período de 30 meses sendo que suas economias totais para investir são de R$2.500,00.

**Índices**: 1-cenoura, 2-feijão, 3-trigo, 4-nabo, 5-repolho, 6-batata, 7-milho, 8-abóbora

Inicialmente, o programa escolhe os valores mais baratos das sementes para comprar e mais caros para 
vender tanto semente quanto legumes nos dados fornecidos.

	Ci = valor de compra de cada semente
	Vi = valor de venda de cada semente
	Ui = valor de venda unitário de cada legume

Depois disso, o programa deve maximizar o lucro do agricultor nesses 30 meses.

	Xi = número de sementes de cada cultura (cada semente é plantado em 1 pedaço de terra)
	Si = número de sementes sobressalientes/que devem ser compradas de cada cultura quando adubado ou não
	Ri = rendimento de cada cultura
	Ai = número de pedaços de terra de cada cultura que foi adubado

**Função Objetivo**:
	*Maximizar*: Z = - ∑XiCi - ∑(Xi-Ai)Si + ∑AiSiVi + ∑XiRiUi

	*Sujeito à*:
		∑XiCi <= 2500
		∑Xi <= 27
		∑Ai <= 15
		Ai <= Xi
		XiRi <= 60, para cenouras, feijões e abóboras (por mês)
		XiRi <= 40 para nabo e batata (por mês)
		XiRi <= 63 para repolho e trigo (por mês)
		X7R7 <= 66 para milho (por mês)

		Todas as variáveis são não negativas.

**ESPECIFICAÇÕES DO CÓDIGO**

Esse trabalho foi feito em linguegem C++ através da IDE Visual Studio 2022 e utilizamos a biblioteca 
GNU Linear Programming Kit (GLPK).