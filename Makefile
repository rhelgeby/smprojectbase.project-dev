
SOURCEDIR=src
SMINCLUDES=env/include
BUILDDIR=build
SPCOMP=env/linux/bin/spcomp
VERSIONDUMP=./updateversion.sh

vpath %.sp $(SOURCEDIR)
vpath %.smx $(BUILDDIR)

SOURCEFILES=$(SOURCEDIR)/*.sp
OBJECTS=$(patsubst %.sp, %.smx, $(notdir $(wildcard $(SOURCEFILES))))

all: prepare $(OBJECTS)

prepare: prepare_newlines prepare_builddir

prepare_newlines:
	@echo "Removing windows newlines"
	@find $(SOURCEDIR)/project -name \*.inc -exec dos2unix -p '{}' \;
	@find $(SOURCEDIR)  -name \*.sp -exec dos2unix -p '{}' \;

prepare_builddir:
	@echo "Creating build directory"
	@mkdir -p $(BUILDDIR)

%.smx: %.sp
	$(VERSIONDUMP)
	$(SPCOMP) -i$(SOURCEDIR) -i$(SOURCEDIR)/include -i$(SMINCLUDES) -o$(BUILDDIR)/$@ $<

clean:
	@echo "Removing build directory"
	@rm -fr $(BUILDDIR)

