# contrib/complex_test/Makefile

MODULE_big = complex_test
OBJS = complex_test.o

EXTENSION = complex_test
# 必须 --1.1.sql 结尾
DATA = complex_test--1.1.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
#PGXS := $(shell $(pg_config) --pgxs)
include $(PGXS)
else
subdir = contrib/complex_test
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
