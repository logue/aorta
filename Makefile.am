AUTOMAKE_OPTIONS=foreign

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

aorta_LDADD = \
	-lsquish

Aorta_LDADD = $(aorta_LDADD) aorta-resources.o
Aorta_SOURCES = $(aorta_SOURCES)

aorta-resources.o: aorta.rc
	@WX_RESCOMP@ -o aorta-resources.o -I$(srcdir) $(srcdir)/aorta.rc
