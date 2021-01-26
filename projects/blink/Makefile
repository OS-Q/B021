SDCC=sdcc
SDLD=sdld
LDFLAGS=--out-fmt-ihx

CFLAGS= -I ./include

FLASH=stm8flash

OBJ_DIR= ./src/
OBJECTS=init.rel main.rel

SYMBOLS=$(OBJECTS:.rel=.sym)
ASMS=$(OBJECTS:.rel=.asm)
LISTS=$(OBJECTS:.rel=.lst)
RSTS=$(OBJECTS:.rel=.rst)

TARGET=main.ihx
OUTPUT=main.hex

MAP=$(TARGET:.ihx=.map)
LK=$(TARGET:.ihx=.lk)
CDB=$(TARGET:.ihx=.cdb)

BOARD=stm8s103f3
PROGRAMMER=stlinkv2

.PHONY: all clean flash

all: $(OUTPUT)

%.rel: $(OBJ_DIR)%.c
	$(SDCC) -mstm8 -lstm8 $(CFLAGS) -c $<

$(TARGET): $(OBJECTS)
	$(SDCC) -mstm8 -lstm8 $(LDFLAGS) -o $@ $(OBJECTS)

$(OUTPUT): $(TARGET)
	packihx $(TARGET) > $(OUTPUT)

flash: $(TARGET)
	$(FLASH) -c $(PROGRAMMER) -p $(BOARD) -w $(TARGET)

clean:
	rm -f $(OBJECTS) $(SYMBOLS) $(ASMS) $(LISTS) $(RSTS)
	rm -f $(TARGET) $(MAP) $(LK) $(CDB)

.PHONY: all clean flash
