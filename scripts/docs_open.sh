#!/bin/bash

cd "$(dirname "$0")" || exit

./docs_generate.sh

cd .. || exit

OS=$(uname -s)
case "$OS" in
  Linux*)               OPENCMD="xdg-open" ;;
  Darwin*)              OPENCMD="open" ;;
  CYGIN*|MINGW*|MSYS*)  OPENCMD="start" ;;
  *)                    OPENCMD="" ;;
esac

if [ -n "$OPENCMD" ]; then
  $OPENCMD docs/html/index.html
else
  echo "Cannot define open command. Open html file by yourself: WriteX/docs/html/index.html"
fi