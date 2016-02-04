%%%Computing the importance of the connection between two paticular
%%%communities

% load M and Communities

num_comm=100;
num_sup=size(M,1);
c2=zeros(num_comm,num_comm);

for i=1:num_sup
   index=find(M(i,:)~=0);
   c=ndx(i);
   for j=1:num_comm
       if size(index,2)~=0
            for k=1:size(index,2)
                if ndx(index(k))==j
                     c2(c,j)=c2(c,j)+1; 
                     c2(j,c)=c2(c,j); 
                end
            end
       end
   end
end

%%%
p_cc=zeros(num_comm,num_comm);
for i=1:num_comm
    
    sum_j=sum(c2(i,:));
    for j=1:num_comm
       if sum_j==0
           p_cc(i,j)=0;
       else
           p_cc(i,j)=c2(i,j)/sum_j;
       end
    end
       
end
%%%
save('p_cc.mat','p_cc','c2');
p_cc=single(p_cc);
dlmwrite('pcc_comm.txt',p_cc,'delimiter',' ','newline','pc');
dlmwrite('network_partition.txt',ndx,'delimiter',' ','newline','pc');