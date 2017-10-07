if [ -z "$1" ]; then
  echo "No Arguments Supplied"
else
  #update the Version in README.md
  sed -i 's/^Version.*/Version '"$1"'/' README.md

  #update the text in initrd/files/version.txt
  sed -i '1s/.*/'"$1"'/' initrd/files/version.txt
  
fi
