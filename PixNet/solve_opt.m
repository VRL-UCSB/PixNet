%%%Solve Optimization


load label_graph
load comm_graph

D_L=sum(label_graph,2);
D_C=sum(comm_graph,2);

L_1=D_L - D_L^(-0.5) * A_L * D_L^(-0.5);
L_2=D_C -A_C;


A=L_1+mu*ones(size(L_1));
B=lamda * L_2;
C=mu * Y;

F=lyap(A, B, -C);

[ind val]=max(F,[],2);
clear val
F_final=ind;

