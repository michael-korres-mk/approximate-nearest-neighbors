# build config
TARGET = ann
DEPENDENCIES = "$(SRC_DIR)/utility/*.cpp"

# arguments
ANN_K = 100
ANN_L = 250
ANN_R = 60
ANN_A = 1.2
DATASET = siftsmall

# command line arguments
ANN_CLINE_ARGS =
	-k $(ANN_K)
	-L $(ANN_L)
	-R $(ANN_R)
	-a $(ANN_A)
	-bv $(DATASET)/$(DATASET)_base.fvecs
	-gv $(DATASET)/$(DATASET)_groundtruth.ivecs
	-qv $(DATASET)/$(DATASET)_query.fvecs
