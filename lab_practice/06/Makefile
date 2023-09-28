PRG = cvdemo2 rgbd bgrm
PKG = `pkg-config --cflags --libs opencv libfreenect` -lfreenect_sync

all:	$(PRG)

%:	%.cc
	c++ $^ $(PKG) -o $@

clean:
	-rm $(PRG)

