%%%Community-driven graph


clear all

% Database= to be set
% seg_level = to be set;
% m = to be set;
% k = to be set;
% L='Lonly_shared_labels'; 
% search='exhaustive';
% C= to be set;

% load communities
% load p_cc

%%introduce the comm_graph
num_nodes=size(unique(net_part),1);

comm_graph=zeros(num_nodes,num_nodes);
comm_graph=p_cc;

save('comm_graph.mat','comm_graph')


