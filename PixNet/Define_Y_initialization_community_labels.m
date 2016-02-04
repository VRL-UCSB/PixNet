%%%Define initialization of community labels--Y

clear all

% Database=to be set;
% seg_level=to be set;
% k=to be set;
% L='Lonly_shared_labels'; %%2;
% search='exhaustive';
% C= to be set;

%% %For fully-labeled dataset---
% load communities
ndx=net_part;
num_comm=size(unique(ndx),1);  
% load label_train
num_labels=size(label,2); 

% sup_loc_mat=dlmread(['sup_loc_train.txt'],'');

% alpha_l= NEEDS TO BE SET ----in linear_search function, we find its best value

Y=zeros(num_comm, num_labels);

for i=1:num_comm
    
    %%find what nodes have fallen in community i
    nodes=find(ndx==i);
    %%get the image numbers assocaited to nodes in community i
    images_in_comm=sup_loc_mat(nodes,1);
    %%eliminate repitative image numbers
    images_in_comm=unique(images_in_comm);
    %%find the labels associated to those image numbers
    sub_label=label(images_in_comm,:); 
    Y(i,:)=sum(sub_label,1);  
    
    %%L1 normalize each row
    Y(i,:)=Y(i,:)/sum(Y(i,:));
    
    %%Binarize each vector Y_i using a threshold
    Y(Y<=alpha_l)=0;
    Y(Y>alpha_l)=1;   
end

save('Y.mat','Y')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% %For partially labeled dataset---

%%Here, we concatinate a column vector to matrix 
Y=[Y zeros(num_comm,1)];

for i=1:num_comm
    %%if community i is not asscociated with any of the labels, assign that
    %%to the "undefined" label.
   if sum(Y(i,:))==0 
      Y(i,num_comm+1)=1; 
   end
end
save('Y.mat','Y')
