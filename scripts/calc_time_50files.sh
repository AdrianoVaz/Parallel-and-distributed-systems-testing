for i in {1..10}; 
do
{
	(time ./sequencial) &>> dados/sequencial50.txt	
	(time ./openmp) &>> dados/openmp50.txt
}
done