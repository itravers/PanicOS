args="";
for entry in "$search_dir"initrd/files/*
do
 args="$args $entry"
 args="$args "
 args=$args`basename "$entry"`
done

echo $args

./initrd/initrd_generator $args



