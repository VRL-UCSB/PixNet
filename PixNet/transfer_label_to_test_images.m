%%%%transfer labels to test images


%%We have F which is the label of the communities
%%Each pixel is within a segment
%%Each segment is within a community
%%Each community has a label

%mapping seg_test to communities
hist_test=dlmread('hist_node_test.txt','');
%%I choose to use arg max
%%basically associtaing each region to the community with highest association likelihood
[temp ndx]=max(hist_test,[],2);  
%loc
load loc_test.mat
%seg results
seg_folder='';
seg_list = dir(fullfile(seg_folder, '*.gif'))';

%ind_test
load ind_test.mat
%image size info
load image_size.mat
S=S(ind_test,:);

PT_image=cell(num_image,1);
for i=1:num_images
    PT=zeros(S(i,:));
    
    seg_image=imread([seg_folder seg_lis(i).name]);
    num_seg=size(unique(seg_image,2));
    
    for j=1:num_seg
       PT(seg_image==j)=F( ndx (loc{i,j} ) );         
    end
    
    PT_image{i}=PT;
end

save('PT_image.mat','PT_image');



