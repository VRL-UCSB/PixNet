%%08/15/2013
%This is to compute features using vlfeat (computes PHOW features)

toolbox='F:\school\toolbox\vlfeat-0.9.16\toolbox\';
addpath(toolbox)
run('vl_setup') 

% --------------------------------------------------------------------
%                                                          AUTORIGHTS
% --------------------------------------------------------------------

conf.calDir = 'PATH to DB' ;
conf.dataDir = 'PATH TO DB' ;
% conf.numTrain = 15 ;
% conf.numTest = 15 ;
% conf.numClasses = 102 ;
conf.numWords =500 ;
conf.numSpatialX = [1 2 4]; %[1 3 2]
conf.numSpatialY = [1 2 4]; %[1 1 2]
% conf.Sizes=[4 5 6 7 9 11 13 16];
conf.quantizer = 'kdtree' ;
% conf.svm.C = 10 ;
% conf.svm.solver = 'pegasos' ;
% conf.svm.biasMultiplier = 1 ;
conf.phowOpts = {'Step', 3} ; %try step=1 for x1,y1
conf.clobber = true ;
conf.tinyProblem = false ;
conf.prefix = 'baseline' ;
conf.randSeed = 1 ;

conf.vocabPath = fullfile(conf.dataDir, [conf.prefix '-vocab.mat']) ;
conf.histPath = fullfile(conf.dataDir, [conf.prefix '-hists.mat']) ;
conf.modelPath = fullfile(conf.dataDir, [conf.prefix '-model.mat']) ;
conf.resultPath = fullfile(conf.dataDir, [conf.prefix '-result']) ;

randn('state',conf.randSeed) ;
% rand('state',conf.randSeed) ;
% vl_twister('state',conf.randSeed) ;

% --------------------------------------------------------------------
%                                                    Model parameters
% --------------------------------------------------------------------

model.phowOpts = conf.phowOpts ;
model.numSpatialX = conf.numSpatialX ;
model.numSpatialY = conf.numSpatialY ;
model.quantizer = conf.quantizer ;
model.vocab = [] ;
% model.Size=conf.Sizes;

% --------------------------------------------------------------------
%                                                           Setup data
% --------------------------------------------------------------------
trainFolder='PATH to train_DB';
trainFile = dir(fullfile(trainFolder, '*.jpg'))';

testFolder='PATH to test_DB';
testFile = dir(fullfile(testFolder, '*.jpg'))';


% --------------------------------------------------------------------
%                                                     Train vocabulary
% --------------------------------------------------------------------

%%%%%%Picking images from different classes for the creation of the codebook
load class.mat
%poicking 20 images from each class
ind=[];
for i=1:size(class,2)
    temp_ind=find(class(:,i)==1)';
    ind=[ind vl_colsubset(temp_ind, 30)];
end

ind=unique(ind);

%%%maybe pick some neg images

ind=[ind,1:100]; 

%%%%%%

if ~exist(conf.vocabPath) || conf.clobber

  % Get some PHOW descriptors to train the dictionary
  descrs = {} ;

  parfor ii = 1:length(ind)
    im = imread([trainFolder trainFile(ind(ii)).name]) ;
    im = standarizeImage(im) ;
    [drop, descrs{ii}] = vl_phow(im, model.phowOpts{:}) ;
  end

  descrs = vl_colsubset(cat(2, descrs{:}), 50e4) ;
  descrs = single(descrs) ;
  
  save('descrs.mat','descrs');
  % Quantize the descriptors to get the visual words
  vocab = vl_kmeans(descrs, conf.numWords, 'verbose', 'algorithm', 'elkan') ;
  save('baseline-vocab-k1000.mat','vocab')
  
%   save(conf.vocabPath, 'vocab') ;
else
  load(conf.vocabPath) ;
end

model.vocab = vocab ;

if strcmp(model.quantizer, 'kdtree')
  model.kdtree = vl_kdtreebuild(vocab) ;
end

% --------------------------------------------------------------------
%                                           Compute spatial histograms
% --------------------------------------------------------------------

if ~exist(conf.histPath) || conf.clobber
  
  hists = {} ;
  parfor ii = 1:size(trainFile,2)  %size(testFile,2)
    im = imread([trainFolder trainFile(ii).name]); %[testFolder testFile(ii).name]
    hists{ii} = getImageDescriptor(model, im);
  end
  hists = cat(2, hists{:}) ;
  save('baseline-train-hists-k1000_step3-x1-y1.mat', 'hists') ;
  
  
  hists = {} ;
  parfor ii = 1:size(testFile,2)  %size(testFile,2)
    im = imread([testFolder testFile(ii).name]); %[testFolder testFile(ii).name]
    hists{ii} = getImageDescriptor(model, im);
  end
  hists = cat(2, hists{:}) ;
  save('baseline-test-hists-k1000_step3-x1-y1.mat', 'hists') ;

  save('baseline-test-hists-k1000_step3-x1-y1.mat', 'hists') ;

  
  
else
  load(conf.histPath) ;
end

% --------------------------------------------------------------------
%                                                  Compute feature map
% --------------------------------------------------------------------

psix = vl_homkermap(hists, 1, 'kchi2', 'gamma', 0.5) ; %0.7

% --------------------------------------------------------------------
%                                                            Train SVM
% --------------------------------------------------------------------

if ~exist(conf.modelPath) || conf.clobber
  switch conf.svm.solver
    case 'pegasos'
      lambda = 1 / (conf.svm.C *  length(selTrain)) ;
      w = [] ;
      % for ci = 1:length(classes)
      parfor ci = 1:length(classes)
        perm = randperm(length(selTrain)) ;
        fprintf('Training model for class %s\n', classes{ci}) ;
        y = 2 * (imageClass(selTrain) == ci) - 1 ;
        data = vl_maketrainingset(psix(:,selTrain(perm)), int8(y(perm))) ;
        [w(:,ci) b(ci)] = vl_svmpegasos(data, lambda, ...
                                        'MaxIterations', 50/lambda, ...
                                        'BiasMultiplier', conf.svm.biasMultiplier) ;
      end
    case 'liblinear'
      svm = train(imageClass(selTrain)', ...
                  sparse(double(psix(:,selTrain))),  ...
                  sprintf(' -s 3 -B %f -c %f', ...
                          conf.svm.biasMultiplier, conf.svm.C), ...
                  'col') ;
      w = svm.w' ;
  end

  model.b = conf.svm.biasMultiplier * b ;
  model.w = w ;

  save(conf.modelPath, 'model') ;
else
  load(conf.modelPath) ;
end

% --------------------------------------------------------------------
%                                                Test SVM and evaluate
% --------------------------------------------------------------------

% Estimate the class of the test images
scores = model.w' * psix + model.b' * ones(1,size(psix,2)) ;
[drop, imageEstClass] = max(scores, [], 1) ;

% Compute the confusion matrix
idx = sub2ind([length(classes), length(classes)], ...
              imageClass(selTest), imageEstClass(selTest)) ;
confus = zeros(length(classes)) ;
confus = vl_binsum(confus, ones(size(idx)), idx) ;

% Plots
figure(1) ; clf;
subplot(1,2,1) ;
imagesc(scores(:,[selTrain selTest])) ; title('Scores') ;
set(gca, 'ytick', 1:length(classes), 'yticklabel', classes) ;
subplot(1,2,2) ;
imagesc(confus) ;
title(sprintf('Confusion matrix (%.2f %% accuracy)', ...
              100 * mean(diag(confus)/conf.numTest) )) ;
print('-depsc2', [conf.resultPath '.ps']) ;
save([conf.resultPath '.mat'], 'confus', 'conf') ;



