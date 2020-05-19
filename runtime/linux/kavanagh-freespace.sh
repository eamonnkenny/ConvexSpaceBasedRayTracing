../Release/kavanagh-testset.exe freespace bts900-freespace.dat receivers-kavanagh-free-xdir.dat
../Release/kavanagh-testset.exe freespace bts900-freespace.dat receivers-kavanagh-free-ydir.dat
../Release/kavanagh-testset.exe freespace bts900-freespace.dat receivers-kavanagh-free-zdir.dat
mkdir -p ../matlab/raytrace/kavanagh
mv ../matlab/building-data/story*.txt* ../matlab/raytrace/kavanagh/
mv ../matlab/building-data/floor*.txt* ../matlab/raytrace/kavanagh/
mv ../matlab/raytrace/*.txt* ../matlab/raytrace/kavanagh/
mv ./totalfield-kavanagh-*.txt ../matlab/raytrace/kavanagh/

