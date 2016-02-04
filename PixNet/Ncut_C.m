function net_part=Ncut_C(row, col, weight, N, C, S)

%C is number of detected communities
%S is the number of nodes that are connected together
%N is the total number of nodes in the network

M=sparse(N,N);

row2=row(1:S);
col2=col(1:S);
weight2=weight(1:S);

clear row col weight


row2=row2+ones(1,S);
col2=col2+ones(1,S);

tic;
index=sub2ind(size(M),row2,col2);
[index2 i]=unique(index);
M(index2)=weight2(i);

code_folder='E:/school/Code/Ncut_9/Ncut_9';
cd(code_folder)

W=M+M';  
time1=toc

tic;
[NcutDiscrete,NcutEigenvectors,NcutEigenvalues] = ncutW(W, C);
time2=toc

for i=1:size(NcutDiscrete,1)
   ndx(i)=find(NcutDiscrete(i,:)==1); 
end
ndx=ndx';

net_part=ndx;

end

%row=dlmread('C:\Users\Niloufar\Desktop\C++\knnSearch_updated\knnSearch_updated\knnSearch\row.txt','');
%dlmwrite('net_part.txt',net_part,'delimiter',' ','newline','pc');