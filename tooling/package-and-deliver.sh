#!/bin/bash

echo "Test project build"

make clean && make

if [ $? -ne 0 ] ; then
    echo "Build failed!"
    exit 1
else
    echo "Build OK!"
fi


echo "Format code"
make format

# Test if there are changed files that are not commited

if [ -n "$(git status --porcelain)" ] ; then
    echo "Uncommited files detected"
    git status
    exit 1
else
    echo "OK"
fi


echo "Currently set tags on this project"
git tag
echo -n "Are the required tags added? (Y/n)"
read ANSWER
if [ "$ANSWER" == "n" ]; then
    echo "Please add required tags"
    exit 1
fi


echo "Packaging the project"

make clean && make

TEMP_DIR=$(mktemp -d)
cp bin/atmega2560-user-code.ihx $TEMP_DIR
make clean
git archive --format=tar.gz -o $TEMP_DIR/$(git describe --abbrev=6 --dirty --always --tags --long).tar.gz HEAD
mv $TEMP_DIR/* bin/
rm -rf $TEMP_DIR

echo "Project packaging succeeded"
exit 0
