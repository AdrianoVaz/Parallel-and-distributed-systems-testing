for i in {1..10}; 
do
{
	(time ./sequencial) &>> dados/sequencial10.txt	
	(time ./openmp) &>> dados/openmp10.txt
}
done