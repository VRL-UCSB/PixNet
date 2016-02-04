
% --------------------------------------------------------------------
%                                                   Add VLFEAT library
% --------------------------------------------------------------------
toolbox='E:\school\toolbox\vlfeat-0.9.16\toolbox\';
addpath(toolbox)
run('vl_setup') 

% --------------------------------------------------------------------
%                                                           Setup data
% --------------------------------------------------------------------
knn=5;  %For LLC coding
scale=[4,6,8,10];

border_Folder='E:\school\Segmentation\ExtractFeatures\VOC2007\train\q600_m0.7\';
%LLC_Folder='E:\school\Code\CVPR10-LLC\';
% load E:/school/Segmentation/ExtractFeatures/VOC2007/phow_features/baseline-vocab-k1000.mat
% load E:/school/Segmentation/ExtractFeatures/VOC2007/q600_m0.7/Border/neighborPoints_uint16.mat


%% ------------------DataBase---------------------------------------
load E:/school/Segmentation/ExtractFeatures/TREC/baseline-vocab-k50.mat

Folder='PATH To Database';
File = dir(fullfile(Folder, '*.jpg'))';

seg_Folder='PATH To Segmented Database';
seg_File = dir(fullfile(seg_Folder, '*.gif'))';

save_Folder='PATH to be saved';


% --------------------------------------------------------------------
%                                                          AUTORIGHTS
% --------------------------------------------------------------------
conf.numWords = 200 ;
conf.numSpatialX = [1]; %[1 3 2]
conf.numSpatialY = [1]; %[1 1 2]
conf.quantizer = 'kdtree' ;
conf.phowOpts = {'Step', 3} ; %try step=1 for x1,y1
conf.clobber = true ;
conf.tinyProblem = false ;
conf.prefix = 'baseline' ;
conf.randSeed = 1 ;

% --------------------------------------------------------------------
%                                                    Model parameters
% --------------------------------------------------------------------
model.phowOpts = conf.phowOpts ;
model.numSpatialX = conf.numSpatialX ;
model.numSpatialY = conf.numSpatialY ;
model.quantizer = conf.quantizer ;
model.vocab = [] ;

% --------------------------------------------------------------------
%                                                     Train vocabulary
% --------------------------------------------------------------------
model.vocab = vocab ;

if strcmp(model.quantizer, 'kdtree')
  model.kdtree = vl_kdtreebuild(vocab) ;
end

% --------------------------------------------------------------------
%                                                    Extract Features
% --------------------------------------------------------------------
hist_hard_Q={};
% hist_soft_Q={};
tic;
parfor ii = 1:size(File,2)    
    %% ************get the boundry information************
    cd(seg_Folder)
    seg_image=imread(seg_File(1,ii).name); 
    
    cd(border_Folder)
    [PriA,BoundryPoints,boundryPoints]=build_graph_singleLevel_2([seg_Folder seg_File(1,ii).name]);
            
    %% ************read in the image************    
    im = imread([Folder File(ii).name]);

    im = im2single(im) ;
    if size(im,1) > 480
        im = imresize(im, [480 NaN]) ;
    end
    
    width = size(im,2) ;
    height = size(im,1) ;
    %% get PHOW features for the entire image
    [frames, descrs] = vl_phow(im, model.phowOpts{:}) ;

%     desc{ii} = descrs;
%     frame{ii}=uint16([frames(1,:); frames(2,:); frames(4,:)]);
    
    %% ************find which region each image falls into************ 
    num_nodes=size(unique(seg_image),1);

    temp=unique(seg_image);
%     for count=1:num_nodes
%        seg_image(seg_image==temp(count))=count-1; 
%     end
    
    for j=1:num_nodes   
       
        desc_region=[];        
        %%regional
%         index2=find(seg_image==j-1); 
        index2=find(seg_image==temp(j));
        %%boundry 
        total_NP=[];
        total_NP=cat(1,BoundryPoints{j,:});
        if size(total_NP,1)~=0  %i.e. For i=329 is zero.
            index3=sub2ind(size(seg_image),total_NP(:,1),total_NP(:,2));
            index3=unique(index3,'rows');
            %%regional+boundry
            index2=unique([index2;index3]);
        end
        z=[];
        for k=1:size(scale,2)
            %get the features with scale(k)
            pick=find(frames(4,:)==scale(k));
            shift4scale=pick(1);
            %get the location of features
            index1=sub2ind(size(seg_image),double(frames(2,pick)),double(frames(1,pick)))';     
            %%comparison of feature locations and regional+boundry points
            indexMerge=[index1;index2];
            indexMergeUnique=unique(indexMerge);
            n=hist(double(indexMerge),double(indexMergeUnique));
            commonIndex=indexMergeUnique(n>1);    %n>4  
            %%adding the features to their corresponding regions                   
            loc=find(ismember(index1,commonIndex)>0)+shift4scale-1;
            z=[z;loc]; 
            desc_region=[desc_region;descrs(:,loc)'];            
%             for m=1:size(commonIndex,1)
%                 loc=find(index1==commonIndex(m))+shift4scale-1;
%                 desc_region=[desc_region;descrs(:,loc)'];
%             end 
        
        end
        %% ************Quantization of the regional data************
        numWords = size(model.vocab, 2) ;
        
        desc_region=desc_region';
        
        if size(desc_region,1)~=0
        
        
            switch model.quantizer
                 case 'vq'
                    [drop, binsa] = min(vl_alldist(model.vocab, single(desc_region)), [], 1) ;
                 case 'kdtree'
                     binsa = double(vl_kdtreequery(model.kdtree, model.vocab, ...
                                          single(desc_region), ...
                                          'MaxComparisons', 15)) ;
            end

            for i = 1:length(model.numSpatialX)
                 binsx = vl_binsearch(linspace(1,width,model.numSpatialX(i)+1), frames(1,z)) ;
                 binsy = vl_binsearch(linspace(1,height,model.numSpatialY(i)+1), frames(2,z)) ;

                 % combined quantization
                bins = sub2ind([model.numSpatialY(i), model.numSpatialX(i), numWords], ...
                         binsy,binsx,binsa) ;
                hist1 = zeros(model.numSpatialY(i) * model.numSpatialX(i) * numWords, 1) ;
                hist1 = vl_binsum(hist1, ones(size(bins)), bins) ;
                hists = single(hist1 / sum(hist1)) ;
            end

    %         hist1 = cat(1,hists{:}) ;
            hist1 = hist1 / sum(hist1) ;
            hist_region=single(hist1);
                  
        else       
            hist_region=zeros(1,numWords);
        end
        
        hist_hard_Q{ii}(j,:)=single(hist_region);
    end
        
  
%         %%***hard-Quantization
%         if size(desc_region,1)~=0
%             switch model.quantizer
%               %%using hard-Q NN
%               case 'vq'
%                 [drop, binsa] = min(vl_alldist(model.vocab, single(desc_region)), [], 1) ;
%               %%using hard-Q ANN
%               case 'kdtree'
%                 binsa = double(vl_kdtreequery(model.kdtree, model.vocab, ...
%                                               single(desc_region)', ...
%                                               'MaxComparisons', 25)) ;
%             end
%             
%             hist_region=hist(binsa,numWords);
%             hist_region = hist_region / sum(hist_region) ;
%         else
%             hist_region=zeros(1,numWords);
%         end
%         hist_hard_Q{ii}(j,:)=single(hist_region);
%         
        %%***soft-Quantization (LLC)
        
        %soft assignment using #knn nearset points
%         hist_region=zeros(numWords,1);
%         if size(desc_region,1)~=0
%             cd('E:\school\Segmentation\ExtractFeatures\VOC2007\phow_features\')
%             llc_codes = LLC_coding_appr(vocab', desc_region, knn);        
%             %max-pooling
%             hist_region = max(llc_codes, [], 1);   
%             hist_region = hist_region./sqrt(sum(hist_region.^2));
%         end
% 
%         hist_soft_Q{ii}(j,:)=single(hist_region);
%     end
    
end

time_taken=toc

cd(save_Folder)
% save('Regional_phow_test.mat','hist_soft_Q','hist_hard_Q')