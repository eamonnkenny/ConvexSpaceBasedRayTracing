rm ../buildings/conorhouse.sav
../Release/conorshouse-28pts.exe 
echo "Print building in matlab/coverage/paper"
mv ../matlab/building-data/story*-conorshouse.txt* ../matlab/coverage/paper/
mv ../matlab/building-data/floor*-conorshouse.txt* ../matlab/coverage/paper/
mv ../matlab/raytrace/alldiff.txt  ../matlab/coverage/paper/
mv ../matlab/raytrace/diffraction.txt  ../matlab/coverage/paper/
mv ../matlab/raytrace/fullrays.txt  ../matlab/coverage/paper/
mv ../matlab/raytrace/images.txt  ../matlab/coverage/paper/
mv ../matlab/raytrace/reflection*.txt  ../matlab/coverage/paper/
sort ./receiverFields-conorshouse.txt > ../matlab/coverage/paper/receiverFields-conorshouse-r1t3d0-sorted.txt
