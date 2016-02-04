%% Written by: Niloufar Pourian
% This is to create a graph for a segmented image at a particular level


function [A,neighborPoints,boundryPoints]=build_graph_singleLevel_2(imageName)
    %segmented image
    image=(imread(imageName));   
    [r c]=size(image);


    %number of nodes in graph or regions in the image
    N_nodes=size(unique(image),1);

    %This is to make image have values from 0 to (N_node-1)     
    temp=unique(image);
    for count=1:N_nodes
       image(image==temp(count))=count-1; 
    end
    
    %A is the adjacency matrix
    A=zeros(N_nodes,N_nodes);

    boundryPoints=cell(N_nodes);

    [FX,FY]=gradient(double(image));
    newImage=sqrt(FX.*FX+FY.*FY);
    newImage(newImage>0)=255;
    [locX,locY]=find(newImage~=0);



    dist=1;
    for i=1:size(locX,1)

        if locX(i)+dist<=r && locX(i)-dist>=1
            check1=image(locX(i)+dist,locY(i));
            check2=image(locX(i)-dist,locY(i));
            if check1 ~= check2
               A(check1+1,check2+1)=1;
               A(check2+1,check1+1)=1;

                   if size(boundryPoints{check1+1,check2+1},1)==0
                       boundryPoints{check1+1,check2+1}=[locX(i) locY(i)];%i;
                       boundryPoints{check2+1,check1+1}=[locX(i) locY(i)];%i;
                   else
                       boundryPoints{check1+1,check2+1}=[boundryPoints{check1+1,check2+1};[locX(i) locY(i)]];%i];
                       boundryPoints{check2+1,check1+1}=[boundryPoints{check2+1,check1+1};[locX(i) locY(i)]];%i];
                   end

            end
        end

       
        if locY(i)+dist<=c && locY(i)-dist>=1 
            check1=image(locX(i),locY(i)+dist);
            check2=image(locX(i),locY(i)-dist);

            if check1 ~= check2
               A(check1+1,check2+1)=1;
               A(check2+1,check1+1)=1;

               if size(boundryPoints{check1+1,check2+1},1)==0
                   boundryPoints{check1+1,check2+1}=[locX(i) locY(i)];%i;
                   boundryPoints{check2+1,check1+1}=[locX(i) locY(i)];%i;
               else
                   boundryPoints{check1+1,check2+1}=[boundryPoints{check1+1,check2+1};[locX(i) locY(i)]];%i];
                   boundryPoints{check2+1,check1+1}=[boundryPoints{check2+1,check1+1};[locX(i) locY(i)]];%i];
               end           
            end
        end   
         
    end

    %This is to remove the repeated locations
    for i=1:N_nodes
        for j=i+1:N_nodes
            boundryPoints{i,j}=unique(boundryPoints{i,j},'rows');  
            boundryPoints{j,i}=unique(boundryPoints{j,i},'rows'); 
        end
    end


  %% This is to get the neighborhood region 
  neighborPoints=cell(N_nodes);
   for i=1:N_nodes
        for j=i+1:N_nodes
            temp=boundryPoints{i,j};
            if size(temp,1)~=0
            index=sub2ind(size(image),temp(:,1),temp(:,2));
   
            indexN=[index-5;index-4;index-3;index-2;index-1;index;index+1;...
                index+2;index+3;index+4;index+5;...  
                index-5*r-5;index-5*r-4;index-5*r-3;index-5*r-2;index-5*r-1;...
                index-5*r;index-5*r+1;index-5*r+2;index-5*r+3;index-5*r+4;...
                index-5*r+5;...
                index-4*r-5;index-4*r-4;index-4*r-3;index-4*r-2;index-4*r-1;...
                index-4*r;index-4*r+1;index-4*r+2;index-4*r+3;index-4*r+4;...
                index-4*r+5;...
                index-3*r-5;index-3*r-4;index-3*r-3;index-3*r-2;index-3*r-1;...
                index-3*r;index-3*r+1;index-3*r+2;index-3*r+3;index-3*r+4;...
                index-3*r+5;...
                index-2*r-5;index-2*r-4;index-2*r-3;index-2*r-2;index-2*r-1;...
                index-2*r;index-2*r+1;index-2*r+2;index-2*r+3;index-2*r+4;...
                index-2*r+5;...
                index-r-5;index-r-4;index-r-3;index-r-2;index-r-1;...
                index-r;index-r+1;index-r+2;index-r+3;index-r+4;index-r+5;...
                index+r-5;index+r-4;index+r-3;index+r-2;index+r-1;...
                index+r;index+r+1;index+r+2;index+r+3;index+r+4;index+r+5;...
                index+2*r-5;index+2*r-4;index+2*r-3;index+2*r-2;index+2*r-1;...
                index+2*r;index+2*r+1;index+2*r+2;index+2*r+3;index+2*r+4;...
                index+2*r+5;...
                index+3*r-5;index+3*r-4;index+3*r-3;index+3*r-2;index+3*r-1;...
                index+3*r;index+3*r+1;index+3*r+2;index+3*r+3;index+3*r+4;...
                index+3*r+5;...
                index+4*r-5;index+4*r-4;index+4*r-3;index+4*r-2;index+4*r-1;...
                index+4*r;index+4*r+1;index+4*r+2;index+4*r+3;index+4*r+4;...
                index+4*r+5;...
                index+5*r-5;index+5*r-4;index+5*r-3;index+5*r-2;index+5*r-1;...
                index+5*r;index+5*r+1;index+5*r+2;index+5*r+3;index+5*r+4;...
                index+5*r+5];
                
           
             
            indexN=max(indexN,1);
            indexN=min(indexN,r*c);
            
            [rN,cN]=ind2sub(size(image),indexN);
            neighborPoints{i,j}(:,1)=rN;
            neighborPoints{i,j}(:,2)=cN;
            neighborPoints{i,j}(:,3)=i;
            neighborPoints{i,j}(:,4)=j;
            
            neighborPoints{j,i}(:,1)=rN;
            neighborPoints{j,i}(:,2)=cN;
            neighborPoints{j,i}(:,3)=i;
            neighborPoints{j,i}(:,4)=j;
            end 
            
        end
   end
   for i=1:N_nodes
        for j=i+1:N_nodes
            neighborPoints{i,j}=unique(neighborPoints{i,j},'rows'); 
            neighborPoints{j,i}=unique(neighborPoints{j,i},'rows'); 
        end
    end
   

end        