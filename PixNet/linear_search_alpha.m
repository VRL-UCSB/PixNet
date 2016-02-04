%%%Date: 09/12/2014
%%%Perform linear search to find best alpha---repeat all to find the best

clear all

% Database= to be set
% seg_level= to be set
% m= to be set
% k= to be set
% L='Lonly_shared_labels'; 
% search='exhaustive';
% C= to be set
% set='train'; 
% seg_set='';  


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%solve for F for different values of alpha_l%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load(['label_graph.mat']);
load(['comm_graph.mat']);

%%%%%%%%%%loading sup_loc
sup_loc=dlmread(['C:\Users\Niloufar\Desktop\SemanticMapping-P3\',Database,'\q',num2str(seg_level),'/sup_loc_',set,'.txt'],'');
%%loading ndx--choose either i or ii
%%%(i)
% hist=dlmread(['c:\users\niloufar\desktop\SemanticMapping-P3\',Database,'\q',num2str(seg_level),'\q',num2str(seg_level),'_k',num2str(k),'\',num2str(L),'\',search,'\hist_train_node_level.txt'],'');
%%I choose to use arg max---basically associtaing each region to the community with highest association likelihood
% [temp ndx]=max(hist,[],2);  
%%%(ii)
% load communities net_part
ndx=net_part; clear net_part;


%%loading label
load(['image_level_label.mat']);


% mu = to be set
% lamda = to be set


num_labels=size(label_graph,1);
num_comm=size(comm_graph,1);

F_all=[];
counter=0;
for alpha_l=0:0.005:1
    
    %%%%Define Y
    Y=zeros(num_comm, num_labels);
    for i=1:num_comm

        %%find what nodes have fallen in community i
        nodes=find(ndx==i);
        %%get the image numbers assocaited to nodes in community i
        images_in_comm=sup_loc(nodes,1);
        %%eliminate repitative image numbers
        images_in_comm=unique(images_in_comm);
        %%find the labels associated to those image numbers
        sub_label=label(images_in_comm,:);
        Y(i,:)=sum(sub_label,1);

        %%L1 normalize each row
        if sum(Y(i,:))~=0
            Y(i,:)=Y(i,:)/sum(Y(i,:));
        end

        %%Binarize each vector Y_i using a threshold
        Y(Y<=alpha_l)=0;
        Y(Y>alpha_l)=1;   
    end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% For partially labeled dataset---
    %%Here, we concatinate a column vector to matrix 
    Y=[Y zeros(num_comm,1)];

    for i=1:num_comm
        %%if community i is not asscociated with any of the labels, assign that
        %%to the "undefined" label.
       if sum(Y(i,:))==0 
          Y(i,num_labels+1)=1; 
       end
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%Solve for F_Final
    diag_vals=sum(label_graph,2);
    D_L=diag(diag_vals);
    
    diag_vals=sum(comm_graph,2);
    D_C=diag(diag_vals);

    A_L=label_graph;
    A_C=comm_graph;

    
    %%--for VOC only
    D_L=[D_L; zeros(1,num_labels)];
    D_L=[D_L zeros(num_labels+1,1)];
    A_L=[A_L; zeros(1,num_labels)];
    A_L=[A_L zeros(num_labels+1,1)];
   
    
    term=D_L.^(-0.5) * A_L * D_L.^(-0.5);
    term(isnan(term)==1)=0;
    L_1=D_L - term;
    L_2=D_C -A_C;


    A=mu*ones(size(L_2))-lamda * L_2;
    B= L_1;
    C=mu * Y;

    F=lyap(A, B, -C);


    [val ind]=max(F,[],2);
    clear val
    F_final=ind;
    
    %% save solutions for F_final for different alpha_l
    counter=counter+1;
    F_all(:,counter)=F_final;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


end

save('F_all_step_0_005.mat','F_all');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%Create P_image for each F value corresponding to alpha_l%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

F=F_all(:,17);

%%%%choose either i or ii
% %%i)mapping segments to communities
% %%I choose to use arg max---basically associtaing each region to the community with highest association likelihood
% [temp ndx]=max(hist,[],2);  
%%ii) using ndx of ncut
load(['net_part_',num2str(C),'.mat']);
ndx=net_part; clear net_part;

% load loc
%load seg results
%load the indeces of the train_val set
ind=1:size(loc,1);
num_images=size(ind,2);

%load image size info

PT_image=cell(num_images,1);
for i=1:num_images
    PT=zeros(S(i,:));
    
    seg_image=imread([seg_folder seg_list(ind(i)).name]);
    uniq_seg_image=unique(seg_image);
    num_seg=size(unique(seg_image),1);
    
    for j=1:num_seg
       PT(seg_image==j-1)=F( ndx (loc(i,j)) );         
    end
    
    PT_image{i}=PT;
end

save('PT_image_train_hist_F.mat','PT_image','-v7.3');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%Compute the accuracy and choose alpha_l with highest accuracy%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load pix_label.mat
load image_size.mat
load PT_image

JI_all=[];
for alpha_l=0:0.05:1
    
    J=[];
    for i=1:num_class

        pos_GT=0;
        pos_sys=0;
        pos_GT_sys=0;

        for j=1:num_images

            %%find totall number of pos in GT for class i 
            %binarize the image
            GT_image=zeros(S(ind(j),:));
            GT_image(sub2ind(S(ind(j),:),pix_label{k,ind(j)}))=1;
            %vectorize the image
            GT_image=GT_image(:);
            pos_GT=pos_GT+sum(GT_image);

            %%find total number of pos in sys for class i
            P_image=PT_image{j};
            P_image(P_image~=i)=0;
            P_image=P_image(:);
            pos_sys=pos_sys+sum(P_image);

            %%find overlapping ones for class i in image j and GT (tp)
            pos_GT_sys=pos_GT_sys + sum(GT_image .* P_image);

        end

        J(i)= (pos_GT_sys) / (pos_sys + pos_GT - pos_GT_sys) ;

    end
    JI = 1/num_class * sum(J);

    JI_all(alpha_l)=JI;
    
end
[val ind]=max(JI_all);
alpha=0:0.05:1;
best_alpha_l=alpha(ind);
best_JI=val;
