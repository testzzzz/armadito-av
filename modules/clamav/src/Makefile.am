AUTOMAKE_OPTIONS=subdir-objects

modulesdir=$(libdir)/armadito/modules
modules_LTLIBRARIES=clamav.la

clamav_la_SOURCES=clamav.c os/linux/osdeps.c os/osdeps.h

clamav_la_LDFLAGS= -module -avoid-version -export-symbols "$(srcdir)/clamav.sym"
clamav_la_CFLAGS= @LIBARMADITO_CFLAGS@
clamav_la_LIBADD= @LIBARMADITO_LIBS@
clamav_la_CFLAGS+= @LIBCLAMAV_CFLAGS@
clamav_la_LIBADD+= @LIBCLAMAV_LIBS@
#clamav_la_CFLAGS+= @GLIB2_CFLAGS@
#clamav_la_LIBADD+= @GLIB2_LIBS@

install-exec-hook:
	-rm -f "$(DESTDIR)$(modulesdir)"/clamav.la "$(DESTDIR)$(modulesdir)"/clamav.a

install-data-hook:
	-rm -f "$(DESTDIR)$(modulesdir)"/clamav.la "$(DESTDIR)$(modulesdir)"/clamav.a

EXTRA_DIST=clamav.sym
