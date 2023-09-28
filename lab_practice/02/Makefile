PRG = camera convert detect point block noise
PKG = `pkgconf --cflags --libs opencv`

all:	$(PRG)

%:	%.cc
	c++ $^ $(PKG) -o $@

clean:
	-rm $(PRG)

