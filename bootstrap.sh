#!/bin/sh

git shortlog -sne > AUTHORS
./version.sh || exit 127
aclocal || exit 126
autoreconf --install --force --symlink || exit 125
# To use gettext translation system
#intltoolize --copy --force --automake

echo "Now run ./configure && make && sudo make install"
