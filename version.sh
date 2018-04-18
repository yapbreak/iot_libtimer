#!/bin/sh

PKGNAME="libtimer"
REPORT="yapbreak+gitlab@yapbreak.fr"
TARNAME="$PKGNAME"
URL="https://gitlab.yapbreak.fr/iot/libtimer"

if [ "x$1" = "x" ]; then
    gittag=$(git describe --tags --always)
else
    gittag="$1"
fi

if [ "x$gittag" = "x" ]; then
    gittag="0000000"
fi

if echo $gittag | grep "[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*.*" > /dev/null 2>&1; then
    major=$(echo $gittag | sed 's|\([0-9][0-9]*\)\..*|\1|')
    minor=$(echo $gittag | sed 's|[0-9][0-9]*\.\([0-9][0-9]*\)\..*|\1|')
    build=$(echo $gittag | sed 's|[0-9][0-9]*\.[0-9][0-9]*\.\([0-9][0-9]*\).*|\1|')

    revision=$build
    age=$minor
    current=$(( major + minor ))
else
    current=0
    age=0
    revision=0
fi

echo "Prepare $PKGNAME version $(( current - age )).${age}.${revision}"

cp configure.pre configure.ac
sed -i "s|AC_INIT(.*)|AC_INIT([${PKGNAME}], [${gittag}], [${REPORT}], [${TARNAME}], [${URL}])|" configure.ac
sed -i "s|___CURRENT___|$current|g" configure.ac
sed -i "s|___REVISION___|$revision|g" configure.ac
sed -i "s|___AGE___|$age|g" configure.ac
