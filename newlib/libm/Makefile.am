## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = cygnus

if NEWLIB_HW_FP
MATHDIR = mathfp
else
MATHDIR = math
endif

SUBDIRS = $(MATHDIR) common machine

libm_la_LDFLAGS = -Xcompiler -nostdlib

if USE_LIBTOOL
SUBLIBS = $(MATHDIR)/lib$(MATHDIR).$(aext) common/libcommon.$(aext) $(LIBM_MACHINE_LIB)
noinst_LTLIBRARIES = libm.la
libm_la_SOURCES =
libm_la_LIBADD = $(SUBLIBS)
else
SUBLIBS = $(MATHDIR)/lib.$(aext) common/lib.$(aext) $(LIBM_MACHINE_LIB)
noinst_LIBRARIES = libm.a
libm.a: $(SUBLIBS)
	rm -f $@
	rm -rf tmp
	mkdir tmp
	cd tmp; \
	  for i in $(SUBLIBS); do \
	    $(AR) x ../$$i; \
	done; \
	$(AR) $(AR_FLAGS) ../$@ *.o
	$(RANLIB) $@
	rm -rf tmp
endif

$(SUBLIBS):

info_TEXINFOS = libm.texinfo

# TENSILICA: add new dependencies for Tensilica-specific files
EXTRA_TEXI_FILES = \
	tensilica-version.texi \
	tensilica-address.texi \
	tensilica-legal-info.texi \
	tensilica-legal-tex.texi

XTENSA_SRC_DIR = $(srcdir)/../../../..
XTENSA_DOC_DIR = $(XTENSA_SRC_DIR)/Software-doc

tensilica-version.texi: $(XTENSA_DOC_DIR)/issue-date $(XTENSA_SRC_DIR)/version
	majorversion=`sed -e 's/\(.*\)\..*/\1/' < $(XTENSA_SRC_DIR)/version`; \
	echo "@set XTENSAVERSION $$majorversion" > $@
	issuedate=`cat $(XTENSA_DOC_DIR)/issue-date`; \
	echo "@set ISSUEDATE $$issuedate" >> $@

tensilica-address.texi tensilica-legal-info.texi tensilica-legal-tex.texi: \
	% : $(XTENSA_DOC_DIR)/%
	rm -f $@
	cp $< $@

libm.info: targetdep.tex math/stmp-def $(EXTRA_TEXI_FILES)

libm.dvi libm.html libm.pdf: targetdep.tex math/stmp-def $(EXTRA_TEXI_FILES)

stmp-targetdep: force
	rm -f tmp.texi
	targetdoc=`pwd`/tmp.texi; \
	for d in $(SUBDIRS); do \
	  if test "$$d" != "."; then \
	    (cd $$d && $(MAKE) doc) || exit 1; \
	  fi; \
	done
	$(SHELL) $(newlib_basedir)/../move-if-change tmp.texi targetdep.tex
	touch $@

targetdep.tex: stmp-targetdep ; @true

math/stmp-def: stmp-targetdep ; @true

.PHONY: force
force:

CLEANFILES = tmp.texi targetdep.tex stmp-targetdep

ACLOCAL_AMFLAGS = -I ..
CONFIG_STATUS_DEPENDENCIES = $(newlib_basedir)/configure.host
