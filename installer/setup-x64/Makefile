CFLAGS=-nologo -arch x64 -ext WixUtilExtension
LFLAGS=-nologo -ext WixUtilExtension -spdb -sw1076

TARGET=digital_clock_4-x64.msi

OBJS = clock_files.wixobj main.wixobj qt_files.wixobj skins.wixobj textures.wixobj

.SUFFIXES: .wxs .wixobj

.wxs.wixobj::
	candle $(CFLAGS) $<

all: $(OBJS)
	light $(LFLAGS) -o $(TARGET) $(OBJS)

clean:
	del $(OBJS)
	del $(TARGET)

$(OBJS):
