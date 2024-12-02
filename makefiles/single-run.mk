SHELL := /bin/bash

include config/ask1.mk.conf
include config/ask2.mk.conf
include config/ask3.mk.conf
include config/ask4.mk.conf

ASK_NUMBERS = 1 2 3 4

all: $(addsuffix b, $(addprefix ask, $(ASK_NUMBERS)))

$(addsuffix b, $(addprefix ask, $(ASK_NUMBERS))):
	$(MAKE) INCLUDE=config/$(@:b=).mk.conf -f makefiles/template.mk

# Target: clean
clean:
	@for n in $(ASK_NUMBERS); do \
		$(MAKE) clean INCLUDE=config/ask$$n.mk.conf -f makefiles/template.mk; \
	done

BUILD_DIR = build
RESOURCES_DIR = src/main/resources

# run

$(addprefix ask, $(ASK_NUMBERS)):
	@if [ -z "$(INPUT_FILE_$(subst ask,,$@))" ]; then \
		$(BUILD_DIR)/$(@:ask=%)/main $(ASK$(subst ask,,$@)_CLINE_ARGS); \
	else \
		$(BUILD_DIR)/$(@:ask=%)/main $(ASK$(subst ask,,$@)_CLINE_ARGS) < "$(INPUT_FILE_$(subst ask,,$@))"; \
	fi



VALGRIND_FLAGS = --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no

$(addsuffix v, $(addprefix ask, $(ASK_NUMBERS))):
	@if [ -z "$(INPUT_FILE_$(@:ask=%))" ]; then \
		valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(@:v=)/main $(ASK$(subst ask,,$(@:v=))_CLINE_ARGS); \
	else \
		valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(@:b=)/main $(ASK$(subst ask,,$(@:v=))_CLINE_ARGS) < "$(INPUT_FILE_$(@:ask%v=%))"; \
	fi

