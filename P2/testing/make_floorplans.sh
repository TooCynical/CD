for filepath in ./testing/cases/*
do
    filename=$(basename $filepath)
    echo "Writing floorplan for $filename..."
    ./bin/main "$filepath" > "./data/floorplans/$filename.floorplan"
done
echo "done."
