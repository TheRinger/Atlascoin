#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

ATLASD=${ATLASD:-$SRCDIR/atlasd}
ATLASCLI=${ATLASCLI:-$SRCDIR/atlas-cli}
ATLASTX=${ATLASTX:-$SRCDIR/atlas-tx}
ATLASQT=${ATLASQT:-$SRCDIR/qt/atlas-qt}

[ ! -x $ATLASD ] && echo "$ATLASD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
ATLVER=($($ATLASCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for atlasd if --version-string is not set,
# but has different outcomes for atlas-qt and atlas-cli.
echo "[COPYRIGHT]" > footer.h2m
$ATLASD --version | sed -n '1!p' >> footer.h2m

for cmd in $ATLASD $ATLASCLI $ATLASTX $ATLASQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${ATLVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${ATLVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
