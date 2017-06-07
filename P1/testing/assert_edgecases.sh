# Run a set of edgecases.

for file in ./testing/cases/edgecases/*
do
    echo "$file :"
    ./bin/main "$file"
    echo ""
done
./bin/main "does_not_exist"