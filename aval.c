
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#ifndef DEBUG 
#define DEBUG 0
#endif

/* -----------------------Main Program---------------------- */
int main (int argc, char *argv[])
{	

	if (DEBUG) printf("ponto 0 \n");
	// Necessary variables
		char *num_aluno=(char*)malloc(20*sizeof(char));
				char *resposta_quiz= (char*)malloc(20*sizeof(char));
		char *resposta_aluno= (char*)malloc(20*sizeof(char));

		char *filename=(char*)malloc(25*sizeof(char));
		int id_aluno_quiz, id_form_quiz;
		int n,total_certas=0,pontos_obtidos=0,pontos_totais=0;
		int *valores=(int*)malloc(sizeof(int));

	// Main Program Code
		if (DEBUG) printf("ponto 1 \n");

	if ( argc == 2 ) /* argc should be 2 for correct execution */
	{
		/*char filename_aluno[25];
		strcpy(filename_aluno, argv[1]);*/
		char* filename_aluno = argv[1];
		int x=0;

		// ---- copiar a parte correspondente ao ID do questionario do nome do ficheiro do aluno ----

		char *filename_quiz = (char*)malloc(sizeof(char));
		for(int i=1;i<strlen(filename_aluno);i++)
			if(filename_aluno[i]!='a')
				filename_quiz[x++]=filename_aluno[i];
			else
				break;

		//------------------------------------ ficheiro do aluno ------------------------------------

		char *src ="avaliacao/Answer/";
		char *temp=malloc(strlen(src)+strlen(filename_aluno)+1);
		strcpy(temp,src);
		strcat(temp,filename_aluno);
		filename_aluno=temp;
		FILE *aluno = fopen(filename_aluno, "r" );
		if (DEBUG && (errno)) perror("Ealun");


		//------------------------------------ ficheiro do questionario ------------------------------------

		src ="avaliacao/Question/Q";
		temp=malloc(strlen(src)+strlen(filename_quiz)+1);
		strcpy(temp,src);
		strcat(temp,filename_quiz);
		filename_quiz=temp;
		strcat(filename_quiz,".txt");
		FILE *quiz = fopen(filename_quiz, "r" );
		if (DEBUG && errno) perror("Equiz");

		// ------------ Recolha do nº de aluno, id do quiz do aluno, id do quiz das respostas -----------------

		fscanf(aluno,"%s",num_aluno);
				if (DEBUG) printf("num_aluno=%s \n",num_aluno);

		fscanf(aluno,"%d",&id_aluno_quiz);
		if (DEBUG) printf("id_aluno_quiz=%d\n",id_aluno_quiz);
		fscanf(quiz,"%d",&id_form_quiz);

		if (DEBUG) printf("ponto 2 \n");

		if(id_aluno_quiz==id_form_quiz) // se forem iguais procedimos
		{

		//----------------------- Obter o nº de perguntas e de seguida ignorar o ID das perguntas( nao vai ser preciso )-----

			fscanf(quiz,"%d",&n);
			for(int i=0;i<n;i++)
				fscanf(quiz,"%*d");
			for(int i=0;i<n;i++)// recebe o valor das cotacoes totais
			{
				fscanf(quiz,"%d",&valores[i]);
				pontos_totais+=valores[i];
			}
if (DEBUG) printf("ponto 3 \n");
			for(int i=0;i<n;i++) // atribui o valor e incrementa o nº de resposta certas por cada pergunta acertada
			{
				if (DEBUG) printf("i=%d \n", i);



				fscanf(aluno,"%s",resposta_aluno);
				if (DEBUG) printf("resposta_aluno=%s \n", resposta_aluno);

								int a =fscanf(quiz,"%s",resposta_quiz);
				if (DEBUG) printf("resposta_quiz=%s %d\n", resposta_quiz,a );

				if(strcmp(resposta_quiz,resposta_aluno)==0)
				{
					pontos_obtidos+=valores[i];
					total_certas++;
				}
			}

			//---- Preparar o caminho do ficheiro final das notas e criar ----

if (DEBUG) printf("ponto 4 \n");
		sprintf(filename, "avaliacao/Result/av%d",id_aluno_quiz);
		strcat(filename,num_aluno);
		strcat(filename,".txt");
		FILE *final= fopen(filename,"w");
		//escreve com o formato:
		// nº de perguntas certas 		nº total de perguntas		% de respostas certas
		// nº de valores obtidos 		nº de valores totais 		nota final
			
		fprintf(final,"%d %d %d\n%d %d %d\n",total_certas,n,total_certas*100/n,pontos_obtidos,pontos_totais,pontos_obtidos*20/pontos_totais);
		fclose(final);


	 	}
		fclose(aluno);
		fclose(quiz);
		if (DEBUG) printf("ponto 5 \n");
	}
	return 0;
}
/* ----------------------End of Main Program--------------------- */