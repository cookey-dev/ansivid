#!/bin/sh
task() {
    node -e "const j=require('jimp');(async()=>{(await j.read('frames/$*')).resize(process.stdout.columns-1,process.stdout.rows).write('frames/$*');})();"
    printf "Processed $*\n"
}
for f in $(ls ./frames); do
   task "$f"
done