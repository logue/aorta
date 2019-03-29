AUTOMAKE_OPTIONS=foreign

ACLOCAL_AMFLAGS = -I m4

if MAKE_WINDOWS
bin_PROGRAMS = Aorta
else
bin_PROGRAMS = aorta
endif

EXTRA_DIST = \
	"Aorta File.icns" Aorta.icns Aorta.ico aorta.rc \
	Aorta-Info.plist Aorta.xcodeproj/project.pbxproj \
	Makefile README VERSION COPYING

aorta_SOURCES = aorta.cpp aorta.h \
	DDSOptionsDialog.cpp DDSOptionsDialog.h \
	Filter.cpp Filter.h FloatImage.cpp FloatImage.h \
	imagdds.cpp imagdds.h image_ext.cpp image_ext.h

aorta-resources.o: $(srcdir)/aorta.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@

Aorta_LDADD = aorta-resources.o
Aorta_SOURCES = $(aorta_SOURCES)