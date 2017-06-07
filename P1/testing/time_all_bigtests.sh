for i in {1..25};
do
    echo $i
    time ./bin/main ./testing/cases/big/bigtest_$i;
done