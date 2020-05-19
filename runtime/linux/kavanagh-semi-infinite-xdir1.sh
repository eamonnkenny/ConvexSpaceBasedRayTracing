../Release/kavanagh-testset.exe semi-infinite-plate bts900-semi-infinite-plate.dat receivers-kavanagh-semi-infinite-xdir1.dat
mkdir -p ../matlab/raytrace/kavanagh
mv ../matlab/building-data/story*.txt* ../matlab/raytrace/kavanagh/
mv ../matlab/building-data/floor*.txt* ../matlab/raytrace/kavanagh/
mv ../matlab/raytrace/*.txt* ../matlab/raytrace/kavanagh/
mv ./totalfield-kavanagh-*.txt ../matlab/raytrace/kavanagh/
mv ./*field.txt ../matlab/raytrace/kavanagh/
mv ./incident*.txt ../matlab/raytrace/kavanagh/
mv ./scatter*.txt ../matlab/raytrace/kavanagh/
echo Output Text Files all available in: ../matlab/raytrace/kavanagh

