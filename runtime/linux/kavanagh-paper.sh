../Release/kavanagh-testset.exe semi-infinite-plate bts700-semi-infinite-plate.dat receivers-kavanagh-semi-infinite-paper.dat
mkdir -p ../matlab/raytrace/kavanagh
mv ../matlab/building-data/story*.txt* ../matlab/raytrace/kavanagh/
mv ../matlab/building-data/floor*.txt* ../matlab/raytrace/kavanagh/
mv ../matlab/raytrace/*.txt* ../matlab/raytrace/kavanagh/
mv ./totalfield-kavanagh-*.txt ../matlab/raytrace/kavanagh/

