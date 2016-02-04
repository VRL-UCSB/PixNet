%%%%%%%%creating a matrix with all border information

DB_size=size(PriA,2);  %4952
%% find the num of regions
load PriA.mat
for i=1:size(PriA,2)
    num_regions(i)=size(PriA{1,i},1);
    
end
max_num_regions=max(num_regions);

save('num_regions_test','num_regions','-v7.3');
dlmwrite('num_regions_test.txt',num_regions,'delimiter',' ','newline','pc');

dlmwrite('max_num_regions_test.txt',max_num_regions,'delimiter',' ','newline','pc');

%% find A, F, sup_loc, loc matrix 
A=hist_hard_Q;
for i=1:DB_size
    num_regions=size(A{1,i},1);
    for j=1:num_regions
        if sum(A{1,i}(j,:))~=0
            A{1,i}(j,:)=A{1,i}(j,:)/sum(A{1,i}(j,:));
        end       
    end
end

save('A.mat','A')

%% creating the matrix that contains all the BOW vectors corresponding to each region---ALSO, For each superpixel#, saving (image#,segment#)
F=[];
k=1;
for i=1:DB_size
   i
   num_regions=size(A{1,i},1);
   start=size(F,1)+1;
   for j=1:num_regions
      F=[F;A{1,i}(j,:)];
      sup_loc{k}=[i,j-1];
      k=k+1;
   end
   finish=size(F,1);
   %loc corresponds to the location(index) of nodes of image i in the the community graph
   loc{i}=start:finish;  
end

save('F.mat','F');
dlmwrite('F_train.txt',F,'delimiter',' ','newline','pc');
save('loc_test.mat','loc');
save('sup_loc_est.mat','sup_loc');

dlmwrite('F_test.txt',F,'delimiter',' ','newline','pc');
%% making loc in the form of a 2D array--for C purposes
loc_mat=int32(zeros(DB_size,max_num_regions));
for i=1:size(loc,2)
   loc_mat(i,1:size(loc{1,i},2))=int32(loc{1,i}(1,:)); 
end
dlmwrite('loc_test.txt',loc_mat,'delimiter',' ','precision', 8, 'newline','pc');

%% finding a matrix representation for sup_loc--for C purposes
for i=1:size(sup_loc,2)
   sup_loc2(i,:)=int32(sup_loc{1,i}(1,:)); 
end
dlmwrite('sup_loc_test.txt',sup_loc2,'delimiter',' ','precision', 8, 'newline','pc');


%% Putting all PriA's together--creating the border matrix
%%finding the max number of regions

border=[];
for i=1:DB_size
   for j=1:size(PriA{i},1) 
        index=find(PriA{i}(j,:)~=0);  %finding the neighbors to node j of image i               
        attach=zeros(1,max_num_regions-size(index,2));
        border=[border;[index attach]];
    end
end
dlmwrite('border_test.txt',border,'delimiter',' ','newline','pc');

