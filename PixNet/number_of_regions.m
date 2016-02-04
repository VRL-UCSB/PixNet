%% Niloufar Pourian
%Finding the number of regions in each image in the DB

load PriA.mat
DB_size=%TO BE SET%;
for i=1:size(PriA,2)
    num_regions(i)=size(PriA{1,i},1);
    
end
save('num_regions_test','num_regions','-v7.3');







