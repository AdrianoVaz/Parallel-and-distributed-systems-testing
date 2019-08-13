
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#ifndef DEBUG 
#define DEBUG 0
#endif

#define WORKTAG 1
#define DIETAG 2
#define MASTER 0

void master()
{
	int  numtasks,n,result;
	char *dir="avaliacao/Answer";
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	//------ Recolher os .txt existentes na pasta das respostas --------

	chdir(dir);
	system("ls *.txt >../../list.txt");
	dir="../../";
	chdir(dir);
	system("cat list.txt | wc -l > count.txt");
	FILE *list = fopen("list.txt","r");
	FILE *list_size = fopen("count.txt","r");
	fscanf(list_size,"%d",&n);
	char **filename = malloc(n * sizeof(char *));

	//------- guardar a info dos ficheiros num array ---------

	for(int i=0;i<n;i++)
	{
		filename[i]=malloc(256*sizeof(char));
		fscanf(list,"%s",filename[i]);
	}
	int x=0;

	//--------- O Master atribui a primeira tarefa a cada slave ----------

	for(int i=1;i<numtasks;i++)
	{
		 	MPI_Send(
		 	filename[x],
		 	strlen(filename[x])+1,
		 	MPI_CHAR,
		 	i,
		 	WORKTAG,
		 	MPI_COMM_WORLD);
		 	x++;
	}		

	//---------- O Master aguarda que os slaves acabem as tarefas, assim que acabarem atribui uma nova ------

	while(x<n)
	{
		MPI_Recv(
			&result,
			1,
			MPI_INT,
			MPI_ANY_SOURCE,
			MPI_ANY_TAG,
			MPI_COMM_WORLD,
			&status);
		MPI_Send(
			filename[x],
			strlen(filename[x])+1,
			MPI_CHAR,
			status.MPI_SOURCE,
			WORKTAG,
			MPI_COMM_WORLD);
	 	x++;
	}	

	//--------------- O Master aguarda que todos os slaves terminem a sua tarefa actual ------------
	for(int i=1;i<numtasks;i++)
	{
		MPI_Recv(&result,
			1,
			MPI_INT,
			MPI_ANY_SOURCE,
			MPI_ANY_TAG,
			MPI_COMM_WORLD,
			&status);
	}

	//---------------- O Master mata todos os slaves porque ja nao vao ser necessarios ----------------
	for(int i=1;i<numtasks;i++)
	{
		MPI_Send("die",
			4,
			MPI_CHAR,
			i,
			DIETAG,
			MPI_COMM_WORLD);
	}
}

void slave()
{
	char *filename_aluno= (char*)malloc(sizeof(char));
	char filename[20];
	MPI_Status status;	
	int result,taskid;
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

	//------- O Slave espera que o Master lhe atribue tarefas, se vierem com DIETAG retorna, se não realiza a tarefa
	for(;;)
	{
		MPI_Recv(filename,
			20,
			MPI_CHAR,
			0,
			MPI_ANY_TAG,
			MPI_COMM_WORLD,
			&status);
		if(status.MPI_TAG == DIETAG)
			return;
		strcpy(filename_aluno,filename);
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
		/*char filename_aluno[25];
		strcpy(filename_aluno, argv[1]);*/
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
			fclose(aluno);
			fclose(quiz);
			if (DEBUG) printf("ponto 5 \n");
		} 
		result=1;
		MPI_Send(&result,
			1,
			MPI_INT,
			0,
			0,
			MPI_COMM_WORLD);
	}
}

int main (int argc, char *argv[]) 
{
	int taskid;
	MPI_Init(&argc,&argv);
	double start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	if (taskid == MASTER)
		master();
	else
		slave();
	double end = MPI_Wtime();
	if(taskid == MASTER)
		printf("real %f\n",end-start);
	MPI_Finalize();
}