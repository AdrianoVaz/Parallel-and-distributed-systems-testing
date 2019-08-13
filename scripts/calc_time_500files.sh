for i in {1..10}; 
do
{
	(time ./sequencial) &>> dados/sequencial500.txt	
	(time ./openmp) &>> dados/openmp500.txt
}
done