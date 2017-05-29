#!/bin/bash
for i in {1..25};
do
    ./../bin/main /cases/bigtest_$i;
done