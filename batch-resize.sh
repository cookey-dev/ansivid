#!/bin/sh
task() {
    node -e "const j=require('jimp');(async()=>{(await j.read('frames/$*')).resize(process.stdout.columns-1,process.stdout.rows).write('frames/$*');})();"
    printf "Processed $*\n"
}
N="0"
if [[ "$*" == "" ]]; then N=$(nproc);
else N=$*; fi

(
for f in $(ls ./frames); do 
   ((i=i%N)); ((i++==0)) && wait
   task "$f" & 
done
)
