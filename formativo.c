
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main (int argc, char *argv[])
{	
	// Necessary variables
	char *num_aluno=(char*)malloc(sizeof(char)); //ID Aluno
	char *resposta_aluno= (char*)malloc(sizeof(char)); // Resposta dada pelo aluno
	char *resposta_quiz= (char*)malloc(sizeof(char)); // Resposta certa da pergunta
	char *filename=(char*)malloc(sizeof(char)); // Nome do ficheiro final com as notas
	int id_aluno_quiz, id_form_quiz; // ID do quiz do aluno  /  ID do quiz das respostas certas
	int n,total_certas=0,pontos_obtidos=0,pontos_totais=0;  // n = nº de perguntas   resto = pontos obtidos ao longo do quiz
	int *valores=(int*)malloc(sizeof(int*)); // nota final
	int y; // nº de perguntas realizadas até ao momento da interrupcao
	int count; // incrementa cada vez que acaba uma pergunta.

	// Main Code

	if ( argc == 2 ) // programa só funciona se houver apenas um argumento
	{
		char *filename_aluno = argv[1];
		int x=0;
		char *filename_quiz = (char*)malloc(sizeof(char));
		char *quiz_filename = (char*)malloc(sizeof(char));
		char *filename_result = (char*)malloc(sizeof(char));

		// ---- copiar a parte correspondente ao ID do questionario do nome do ficheiro do aluno ----

		for(int i=1;i<strlen(filename_aluno);i++)//
			if(filename_aluno[i]!='a')
				filename_quiz[x++]=filename_aluno[i];
			else
				break;
		quiz_filename = filename_quiz; 

		//------------------------------------ ficheiro do aluno ------------------------------------

		char *src ="formacao/Answer/";
		char *temp=malloc(strlen(src)+strlen(filename_aluno)+1);
		strcpy(temp,src);
		strcat(temp,filename_aluno);
		filename_aluno=temp;
		FILE *aluno = fopen(filename_aluno, "r" );

		//------------------------------------ ficheiro do questionario ------------------------------------
		src ="formacao/Question/Q";
		temp=malloc(strlen(src)+strlen(filename_aluno)+1);
		strcpy(temp,src);
		strcat(temp,filename_quiz);
		filename_quiz=temp;
		strcat(filename_quiz,".txt");
		FILE *quiz = fopen(filename_quiz, "r" );


		// ------------ Recolha do nº de aluno, id do quiz do aluno, id do quiz das respostas -----------------

		fscanf(aluno,"%s",num_aluno);
		fscanf(aluno,"%d",&id_aluno_quiz);
		fscanf(quiz,"%d",&id_form_quiz);

		if(id_aluno_quiz==id_form_quiz) // se ambos os ID's forem iguais prosseguimos
		{

		//----------------------- Obter o nº de perguntas e de seguida ignorar o ID das perguntas( nao vai ser preciso )-----

			fscanf(quiz,"%d",&n);
			for(int i=0;i<n;i++)
				fscanf(quiz,"%*d");

			//-------------------  ficheiro final com as notas -------------------

			src ="formacao/Result/av";
			temp=malloc(strlen(src)+strlen(filename_aluno)+1);
			strcpy(temp, src);
			strcat(temp, quiz_filename);
			strcat(temp, num_aluno);
			filename_result = temp;
			strcat(filename_result,".txt");
			FILE *result;
			printf("%s\n",filename_result);
			// se existir um ficheiro de resultados, utilizamos a informação do mesmo para continuar
			if (result = fopen(filename_result, "r"))
			{
				/*
				estrutura de result:
				nperguntasrespondidas
				p.obtidos p.possiveis 
				certas totais
				*/
				fscanf(result, "%d", &y);
				fscanf(result, "%d", &pontos_obtidos);
				fscanf(result, "%d", &pontos_totais);
				fscanf(result, "%d", &total_certas);
				if(y!=n) // se o nº de perguntas respondidas for diferente ao nº de perguntas total prosseguimos
				{
					for (int i=0;i<y;i++) // ignora o que foi respondido até agora
						fscanf(quiz, "%*d");

					for(int i=0;i<n-y;i++) // recebe os valores das cotaçoes das perguntas
						fscanf(quiz,"%d",&valores[i]); 

					for (int i=0; i<y; i++)	//ignora as perguntas que já foram respondidas
					{
						fscanf(quiz,"%*s");
						fscanf(aluno,"%*s");
					}
					count = 0;
					int j = 0;
					while(fscanf(aluno, "%s", resposta_aluno) != EOF)
					{
						fscanf(quiz,"%s",resposta_quiz); // recebe a resposta certa
						if(strcmp(resposta_quiz,resposta_aluno)==0) // verifica se sao iguais se sim,
						{
							pontos_obtidos+=valores[j++]; // atribui o valor da pergunta aos pontos do aluno
							total_certas++; // incrementa o nº de respostas certas
							printf("Certo!\n");
						}
						else {
							printf("Errado! A resposta certa é: %s\n", resposta_aluno);
						}
						printf("Pontos obtidos: %d/%d\n", pontos_obtidos, pontos_totais);
						printf("Respostas certas: %d/%d\n", total_certas, n);
						count++;
						printf("%d\n",count);
					}
					//numero de perguntas que foram respondidas
					count = count + y;
				}
				else
					printf("Este quiz já se encontra completado!\n");
				fclose(result);
			}
			// se nao existir ficheiro vamos buscar os valores das cotações das perguntas totais
			else
			{
				y = 0;
				count = 0;
				for(int i=0;i<n;i++) // recebe o valor das cotacoes totais
				{
					fscanf(quiz,"%d",&valores[i]);
					pontos_totais+=valores[i];
				}
				int j = 0;
				while(fscanf(aluno, "%s", resposta_aluno) != EOF) // atribui o valor e incrementa o nº de resposta certas por cada pergunta acertada
				{
					fscanf(quiz,"%s",resposta_quiz); 
					if(strcmp(resposta_quiz,resposta_aluno)==0)
					{
						pontos_obtidos+=valores[j++];
						total_certas++;
						printf("Certo!\n");
					}
					else {
						printf("Errado! A resposta certa é: %s\n", resposta_aluno);
					}
					printf("Pontos obtidos: %d/%d\n", pontos_obtidos, pontos_totais);
					printf("Respostas certas: %d/%d\n", total_certas, n);
					count++;
				}
			}
			//---- Preparar o caminho do ficheiro final das notas e criar ----

			sprintf(filename, "formacao/Result/av%d",id_aluno_quiz);
			strcat(filename,num_aluno);
			strcat(filename,".txt");
			FILE *final= fopen(filename,"w");

			fprintf(final, "%d\n%d %d\n%d %d", count, pontos_obtidos, pontos_totais, total_certas, n);

			fclose(final);
		}
		else
			printf("It is not the same quiz\n"); // o ID do quiz não é o mesmo logo não faz sentido comparar resultados.
			fclose(aluno);
			fclose(quiz);
	}
	return 0; //fim do programa
}
