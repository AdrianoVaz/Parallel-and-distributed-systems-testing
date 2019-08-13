for i in {1..10}; 
do
{
	(time ./sequencial) &>> dados/sequencial100.txt	
	(time ./openmp) &>> dados/openmp100.txt
}
done