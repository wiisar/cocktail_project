LDFLAGS+= # Les libs supplementaires
CFLAGS+=-D_XOPEN_SOURCE=700 -Werror -Wextra -Wall -Wno-error=unused-parameter
# -Wno-error=discarded-qualifiers
ILLEGAL=strcat # Some valid egrep regex
DEPS=enonce.c provided.h
OBJS=provided.o
