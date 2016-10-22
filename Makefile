CPP=g++
CXXFLAGS=-std=c++11
SRC=${shell ls */*.cc 2> /dev/null | xargs}

.PHONY: bin dist-clean

bin: $(SRC:.cc=)

dist-clean:
	@$(RM) \
	$(SRC:.cc=)
