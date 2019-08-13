for i in {1..10}; 
do
{
	(time ./sequencial) &>> dados/sequencial200.txt	
	(time ./openmp) &>> dados/openmp200.txt
}
done