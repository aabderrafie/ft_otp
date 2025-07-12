# Makefile for ft_otp Bonus Features

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDES = -Iqr_generator/include -Igui/include -I../mandatory/include
LIBS = -lssl -lcrypto

# QR Code target
QR_SOURCES = qr_generator/src/qr_main.cpp \
             qr_generator/src/qr_generator.cpp \
             ../mandatory/src/ft_otp.cpp

QR_TARGET = ft_otp_qr

# GUI target  
GUI_SOURCES = gui/src/gui_main.cpp \
              gui/src/simple_gui.cpp \
              ../mandatory/src/ft_otp.cpp

GUI_TARGET = ft_otp_gui

all: qr gui

qr: $(QR_TARGET)

gui: $(GUI_TARGET)

$(QR_TARGET): $(QR_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QR_SOURCES) -o $@ $(LIBS)

$(GUI_TARGET): $(GUI_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(GUI_SOURCES) -o $@ $(LIBS)

clean:
	rm -f $(QR_TARGET) $(GUI_TARGET)

re: clean all

.PHONY: all qr gui clean re
