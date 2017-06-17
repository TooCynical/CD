for filepath in ./data/floorplans/*
do
    filename=$(basename $filepath)
    echo "Making image for $filename..."
    python ./testing/drawer.py $filepath ./data/images/$filename
done
echo "done."
