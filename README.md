# PixNet
-------------------------

Weakly supervised graph based semantic segmentation by learning communities of image-parts

-------------------------


For the initial segmentation step, use the JSEG segmentation. Available at http://old.vision.ece.ucsb.edu/segmentation/jseg/software/

 * phow-bow.m : Uses VLFEAT to compute the PHOW - bag of words features from the dataset. Most of the code is taken from phow_caltech101.m example of VLFEAT.
	
	Uses -> standarizeImage.m: Resizes the images
	     -> getImageDescriptor.m: Computes the descriptor histograms from the input image
	     
 * regional_phow.m: Uses VLFEAT to compute the features on the segmented regions. Requires the images to be segmented beforehand. Should be used on training and test data separately.
	
	Uses -> build_graph_singleLevel_2.m: Creates a graph for a segmented image at a particular level
	
 * all_border.m: Creates the border matrix on segmented images. Should be used on training and test data separately.
 
 * C++ part -> Main folder: Run the main.cpp to read the outputs of previous matlab scripts and create the communities.
 
        Requires OpenCV, Matlab Engine and NCUT codes. NCUT available at http://www.timotheecour.com/software/ncut/ncut.html
			Uses Matlab engine to call Ncut method. Uses Ncut_C.m to call the method.
			Arguments: 
			argv[1] : N1, total number of regions in training set.  argv[2] : N2, total number of regions in test set.
			argv[3]: M, number of words.
			argv[4]: DB1, number of images in training set.
			argv[5]: num_class, number of classes in 
			argv[6]: L, class similarity parameter
			argv[7]: seg_level, argv[8]: DB_name , argv[9]: Feature -> Address manipulation strings
			argv[10]: max_region1, maximum number of regions in one image in training set.
			argv[11]: N_conn,  weighted network parameter, number of connected nodes in the network
			argv[12]: num_comm, number of communities
			argv[13]: knn_consider
			argv[14]: DB2, number of images in testing set.
			argv[15]: max_region2, maximum number of regions in one image in testing set.
			
 * Define_Y_initialization_community_labels.m : 
    to find alpha_l, use the method -> linear_search_alpha.m
    
 * transfer_label_to_test_images.m: Assigns labels to the detected communities on test images
 
 
---
Extras

    solve_opt.m
    CommRelation.m
    number_of_regions.m
    comm_driven_graph.m
