CPP = g++
#CFLAGS = -Wall -Werror -O2 -std=c++11 `pkg-config opencv --cflags`
CFLAGS = -Wall -Werror -ggdb -std=c++11 `pkg-config opencv --cflags`
LFLAGS = -lboost_program_options `pkg-config opencv --libs`

SRC_PATH = ./src
BIN_PATH = ./bin
EXE_NAME = vbm4d

all: $(BIN_PATH) $(BIN_PATH)/$(EXE_NAME)

$(BIN_PATH):
	mkdir $(BIN_PATH)

$(BIN_PATH)/$(EXE_NAME): $(BIN_PATH)/main.o $(BIN_PATH)/Settings.o $(BIN_PATH)/FrameBuffer.o $(BIN_PATH)/Block.o $(BIN_PATH)/Volume.o $(BIN_PATH)/Group.o
	$(CPP) $(BIN_PATH)/*.o $(LFLAGS) -o $(BIN_PATH)/$(EXE_NAME)

$(BIN_PATH)/main.o: $(SRC_PATH)/main.cpp $(SRC_PATH)/settings/Settings.h
	$(CPP) $(CFLAGS) -c $< -o $@

$(BIN_PATH)/%.o: $(SRC_PATH)/settings/%.cpp $(SRC_PATH)/settings/*.h
	$(CPP) $(CFLAGS) -c $< -o $@

$(BIN_PATH)/%.o: $(SRC_PATH)/blockmatching/%.cpp $(SRC_PATH)/blockmatching/*.h
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm $(BIN_PATH)/*.o
	rm $(BIN_PATH)/$(EXE_NAME)
